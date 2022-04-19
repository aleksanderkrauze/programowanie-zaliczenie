#ifndef PERSON_H
#define PERSON_H

#include <ostream>
#include <string>

#include "vector2d.h"

class Person {
public:
  enum class InfectionStatus { GREEN, RED, BLUE };

  Person(const Vector2d, const Vector2d, const double, const InfectionStatus);
  Person(const Vector2d, const Vector2d, const double);
  Person(const double, const double, const double, const double, const double,
         const InfectionStatus);
  Person(const double, const double, const double, const double, const double);

  Person(Person&&) = default;
  Person& operator=(Person&&) = default;
  ~Person() = default;

  Vector2d position() const noexcept;
  void position(const Vector2d) noexcept;
  Vector2d velocity() const noexcept;
  void velocity(const Vector2d) noexcept;
  double radius() const noexcept;
  double time_of_infection() const noexcept;
  void time_of_infection(const double) noexcept;
  Person::InfectionStatus infection_status() const noexcept;
  void infection_status(const Person::InfectionStatus) noexcept;

  void move(const double, const double);

  static bool is_in_infection_range(const Person&, const Person&);

private:
  Vector2d m_position, m_velocity;
  double m_radius, m_time_of_infection;
  InfectionStatus m_infection_status;

  Person(const Person&) = default;
  Person& operator=(const Person&) = default;
};

std::ostream& operator<<(std::ostream&, const Person::InfectionStatus);

#endif // PERSON_H
