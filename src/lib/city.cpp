#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <random>
#include <sstream>
#include <utility>
#include <variant>
#include <vector>

#include "city.h"
#include "config.h"
#include "exceptions.h"
#include "line2d.h"
#include "person.h"
#include "plotter.h"
#include "vector2d.h"

/**
 * # Exceptions
 * - Throws RequiredPositiveDoubleValueException when @city_size is less than or
 * equal to zero
 * - Throws RequiredPositiveDoubleValueException when @time is less than or
 * equal to zero
 * - Throws RequiredPositiveDoubleValueException when @dt is less than or equal
 * to zero
 * - Throws RequiredPositiveDoubleValueException when @recovery_time is less
 * than or equal to zero
 */
City::City(const double city_size, const double time, const double dt,
           const double recovery_time)
    : m_city_size{city_size}, m_time{time}, m_dt{dt},
      m_recovery_time{recovery_time}, m_current_time{0.0}, m_people{} {
  if (city_size <= 0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
  }
  if (time <= 0) {
    throw RequiredPositiveDoubleValueException("time", time);
  }
  if (dt <= 0) {
    throw RequiredPositiveDoubleValueException("dt", dt);
  }
  if (recovery_time <= 0) {
    throw RequiredPositiveDoubleValueException("recovery_time", recovery_time);
  }
}

/**
 * # Exceptions
 * - Throws OutOfCityBoundsException when @person's position is outside City's
 * size bounds
 */
void City::add_person(Person&& person) {
  if (!City::is_in_bound(person.position(), this->m_city_size)) {
    throw OutOfCityBoundsException(person, this->m_city_size);
  }

  this->m_people.push_back(std::move(person));
}

const std::vector<Person>& City::people() const noexcept {
  return this->m_people;
}

void City::run_simulation(const Config& config) {
  const auto total_frames =
    static_cast<int>(std::ceil(this->m_time / this->m_dt));

  const auto plot = [this, &config](const auto frame_no) -> void {
    if (config.save_frames) {
      plotter::plot(this->m_people, frame_no, this->m_city_size);
    }
  };

  plot(0);

  for (auto frame = 1; frame <= total_frames; ++frame) {
    this->run_frame();
    plot(frame);

    const auto p = static_cast<int>(100 * static_cast<double>(frame) /
                                    static_cast<double>(total_frames));
    std::cout << "\033[2K\r"
              << "Created frame no. " << frame << " [" << p << "%]";
    std::cout.flush();
  }
  std::cout << "\n";
}

void City::run_frame() {
  this->move();
  this->infection();
  this->update_recovering();
  this->m_current_time += this->m_dt;
}

void City::write_state(std::ostream& os) const {
  for (const auto& p : this->m_people) {
    os << p << "\n";
  }
}

/**
 * # Exceptions
 * - Throws RequiredPositiveDoubleValueException when @city_size is less than or
 * equal to zero
 */
bool City::is_in_bound(const Vector2d& p, const double city_size) {
  if (city_size <= 0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
  }

  const Line2d right_edge{{city_size, 0.0}, {0.0, 1.0}};
  const Line2d top_edge{{city_size, city_size}, {-1.0, 0.0}};
  const Line2d left_edge{{0.0, city_size}, {0.0, -1.0}};
  const Line2d bottom_edge{{0.0, 0.0}, {1.0, 0.0}};

  // We must check position in relation to lines, because due to floating
  // point arithmetic we could be *slightly* outside, but is reality on the
  // edge.
  return !(right_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
         !(top_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
         !(left_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
         !(bottom_edge.point_position(p) == Line2d::PointPosition::RIGHT);
}

City City::from_config(const Config& config) {
  if (std::holds_alternative<ConfigTest>(config.data)) {
    const auto city_size = 0.25;
    const auto time = 2.0;
    const auto dt = 0.02;
    const auto recovery_time = 0.5;

    City city{city_size, time, dt, recovery_time};
    city.add_test_people();

    return city;
  } else if (std::holds_alternative<ConfigRandom>(config.data)) {
    const auto city_size = 1.0;
    const auto config_data = std::get<ConfigRandom>(config.data);

    City city{city_size, config_data.time, config_data.dt,
              config_data.recovery_time};
    city.add_random_people(config_data.n_people);

    return city;
  } else if (std::holds_alternative<ConfigFile>(config.data)) {
    const auto config_data = std::get<ConfigFile>(config.data);

    City city{config_data.city_size, config_data.time, config_data.dt,
              config_data.recovery_time};
    city.add_from_file_people(config_data.input_file);

    return city;
  } else {
    throw std::runtime_error("Unknown simulation type");
  }
}

void City::update_recovering() noexcept {
  for (auto& person : this->m_people) {
    if (person.infection_status() == Person::InfectionStatus::RED) {
      if (person.time_of_infection() + this->m_recovery_time <=
          this->m_current_time) {
        person.infection_status(Person::InfectionStatus::BLUE);
      }
    }
  }
}

void City::infection() noexcept {
  const size_t size = this->m_people.size();
  const auto check_and_register_infection = [this](const auto& infected_person,
                                                   auto& person) -> void {
    if (person.infection_status() == Person::InfectionStatus::GREEN &&
        Person::is_in_infection_range(infected_person, person)) {
      person.register_next_infection_status(Person::InfectionStatus::RED);
      person.time_of_infection(this->m_current_time);
    }
  };

  for (size_t i = 0; i < size; ++i) {
    const auto& current_person = this->m_people[i];
    if (current_person.infection_status() == Person::InfectionStatus::RED) {
      // all pople before current_person
      for (size_t j = 0; j < i; ++j) {
        check_and_register_infection(current_person, this->m_people[j]);
      }

      // all people after current_person
      for (size_t j = i + 1; j < size; ++j) {
        check_and_register_infection(current_person, this->m_people[j]);
      }
    }
  }

  for (auto& p : this->m_people) {
    p.apply_next_infection_status();
  }
}

void City::move() noexcept {
  for (auto& p : this->m_people) {
    p.move(this->m_dt, this->m_city_size);
  }
}

void City::add_test_people() noexcept {
  Person p1{{0.1, 0.2}, {0.1, 0.2}, 0.02, Person::InfectionStatus::GREEN};
  Person p2{{0.1, 0.1}, {0.5, 0.3}, 0.05, Person::InfectionStatus::GREEN};
  Person p3{{0.2, 0.1}, {0.3, 0.6}, 0.03, Person::InfectionStatus::RED};

  this->add_person(std::move(p1));
  this->add_person(std::move(p2));
  this->add_person(std::move(p3));
}

void City::add_random_people(const std::uint32_t n_people) noexcept {
  const unsigned seed =
    std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator{seed};
  std::uniform_real_distribution<double> position_distribution{0.0, 1.0};
  std::uniform_real_distribution<double> velocity_distribution{-0.5, 0.5};
  std::uniform_real_distribution<double> radius_distribution{0.01, 0.05};

  const auto get_position = [&generator, &position_distribution]() -> Vector2d {
    const auto x = position_distribution(generator);
    const auto y = position_distribution(generator);

    return Vector2d{x, y};
  };

  const auto get_velocity = [&generator, &velocity_distribution]() -> Vector2d {
    const auto x = velocity_distribution(generator);
    const auto y = velocity_distribution(generator);

    return Vector2d{x, y};
  };

  const auto get_radius = [&generator, &radius_distribution]() -> double {
    return radius_distribution(generator);
  };

  const auto get_person = [&get_position, &get_velocity,
                           &get_radius](const auto status) {
    const auto position = get_position();
    const auto velocity = get_velocity();
    const auto radius = get_radius();

    return Person{position, velocity, radius, status};
  };

  const int n_red = n_people * 0.1;
  const int n_green = n_people - n_red;

  for (auto i = 0; i < n_green; ++i) {
    this->add_person(get_person(Person::InfectionStatus::GREEN));
  }
  for (auto i = 0; i < n_red; ++i) {
    this->add_person(get_person(Person::InfectionStatus::RED));
  }
}

void City::add_from_file_people(const std::string& filename) {
  std::cout << "Loading configuration from file " << filename << "\n";

  std::ifstream file;
  file.exceptions(std::fstream::failbit | std::fstream::badbit);
  try {
    file.open(filename);

    _PersonData data;
    std::string line;
    while (getline(file, line)) {
      std::istringstream is{line};
      is >> data;

      this->add_person(Person{data});
    }
  } catch (const std::fstream::failure&) {
    // XXX: This is a **very** dirty hack, but it seams to work
    if (file.eof() && file.fail()) {
      // We got to the end of file, that means that *probably* that was the
      // cause for this exception. In which case we igore it.
    } else {
      std::ostringstream s;
      s << "Couldn't read City's configuration from file " << filename;
      throw IOException(s.str());
    }
  }
}