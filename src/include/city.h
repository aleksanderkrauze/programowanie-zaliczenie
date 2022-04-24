#ifndef CITY_H
#define CITY_H

#include <ostream>
#include <vector>

#include "config.h"
#include "person.h"
#include "vector2d.h"

class City {
public:
  City(const double, const double, const double, const double);

  City(const City&) = delete;
  City& operator=(const City&) = delete;
  City(City&&) = default;
  City& operator=(City&&) = delete;
  ~City() = default;

  void add_person(Person&&);
  const std::vector<Person>& people() const noexcept;

  void run_simulation(const Config&);
  void run_frame();
  void write_state(std::ostream&) const;

  static bool is_in_bound(const Vector2d&, const double);
  static City from_config(const Config&);

private:
  const double m_city_size, m_time, m_dt, m_recovery_time;
  double m_current_time;
  std::vector<Person> m_people;

  void update_recovering() noexcept;
  void infection() noexcept;
  void move() noexcept;

  void add_test_people() noexcept;
  void add_random_people(const Config&) noexcept;
  void add_from_file_people(const Config&);
};

#endif // CITY_H
