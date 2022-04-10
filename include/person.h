#ifndef PERSON_H
#define PERSON_H

#include <string>

#include "algebra.h"

enum class InfectionStatus { GREEN, RED, BLUE };

std::string infection_status_to_colour(InfectionStatus);

class Person {
public:
  Person(Vector2d, Vector2d, double, InfectionStatus);
  Person(double, double, double, double, double, InfectionStatus);
  Person(Person&&) = default;
  ~Person() = default;

  Person& operator=(Person&&) = default;

  Vector2d position() const noexcept;
  void position(Vector2d) noexcept;
  Vector2d velocity() const noexcept;
  void velocity(Vector2d) noexcept;
  double radius() const noexcept;
  double time_of_infection() const noexcept;
  void time_of_infection(double) noexcept;
  InfectionStatus infection_status() const noexcept;
  void infection_status(InfectionStatus) noexcept;

  void draw() const;
  void move(double, double);

  static bool is_in_infection_range(const Person&, const Person&);

private:
  Vector2d m_position, m_velocity;
  double m_radius, m_time_of_infection;
  InfectionStatus m_infection_status;

  Person(const Person&) = default;
  Person& operator=(const Person&) = default;
};

#endif // PERSON_H
