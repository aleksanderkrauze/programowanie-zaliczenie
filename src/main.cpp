#include <iostream>
#include <utility>

#include "city.h"
#include "exceptions.h"
#include "person.h"
#include "plotter.h"

int main(int argc, char* argv[]) {
  Person p1 = {0.1, 0.2, 0.1, 0.2, 0.02, Person::InfectionStatus::GREEN};
  Person p2 = {0.1, 0.1, 0.5, 0.3, 0.05, Person::InfectionStatus::GREEN};
  Person p3 = {0.2, 0.1, 0.3, 0.6, 0.03, Person::InfectionStatus::RED};

  City c = {100, 0.02, 0.25, 0.5};
  c.add_person(std::move(p1));
  c.add_person(std::move(p2));
  c.add_person(std::move(p3));

  const auto& p = c.people();
  plotter::plot(p, 0, 0.25);

  return 0;
}
