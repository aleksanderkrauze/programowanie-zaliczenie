#include <iostream>
#include <utility>

#include "city.h"
#include "exceptions.h"
#include "person.h"
#include "plotter.h"
#include "vector2d.h"

int main(int argc, char* argv[]) {
  Person p1 = {{0.1, 0.2}, {0.1, 0.2}, 0.02};
  Person p2 = {{0.1, 0.1}, {0.5, 0.3}, 0.05};
  Person p3 = {{0.2, 0.1}, {0.3, 0.6}, 0.03, Person::InfectionStatus::RED};

  // City c = {100, 0.02, 0.25, 0.5};
  // c.add_person(std::move(p1));
  // c.add_person(std::move(p2));
  // c.add_person(std::move(p3));

  // const auto& p = c.people();
  // plotter::plot(p, 0, 0.25);

  std::vector<Person> people{};
  people.push_back(std::move(p1));
  people.push_back(std::move(p2));
  people.push_back(std::move(p3));

  const auto dt = 0.01;
  const auto size = 0.25;
  const auto iter = 1000;

  int i = 0;
  std::size_t index = 0;

  for (; i < iter; i++) {
    std::cout << i << std::endl;
    plotter::plot(people, i, size);

    for (auto& p : people) {
      index++;
      const auto position = p.position();
      const auto velocity = p.velocity();

      try {
        p.move(dt, size);
      } catch (std::runtime_error& e) {
        std::cerr << "Caught runtime error: " << e.what() << std::endl;

        std::cerr << "Person no. " << index << std::endl;
        std::cerr << "Person's state before calling move():" << std::endl;
        std::cerr << "position = " << position << " velocity = " << velocity
                  << std::endl;
        std::cerr << "Current person's state:" << std::endl;
        std::cerr << "position = " << p.position()
                  << " velocity = " << p.velocity() << std::endl;

        return 1;
      }
    }
    index = 0;
  }

  return 0;
}
