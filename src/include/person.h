#ifndef PERSON_H
#define PERSON_H

#include <istream>
#include <optional>
#include <ostream>
#include <string>

#include "vector2d.h"

// Since Person doesn't have an empty constuctor we can't use
// std::istream& operator>> to construct a Person. To fix this this struct is
// defined. Person has a constructor that takes this structure and for this
// structure there is defined proper operator>>.
struct _PersonData;

class Person final {
public:
  enum class InfectionStatus { GREEN, RED, BLUE };

  Person(const Vector2d, const Vector2d, const double, const InfectionStatus);
  Person(const Vector2d, const Vector2d, const double);
  Person(const double, const double, const double, const double, const double,
         const InfectionStatus);
  Person(const double, const double, const double, const double, const double);
  Person(const _PersonData&);

  Person(const Person&) = delete;
  Person& operator=(const Person&) = delete;
  Person(Person&&) = default;
  Person& operator=(Person&&) = delete;
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

  void register_next_infection_status(const Person::InfectionStatus) noexcept;
  void apply_next_infection_status() noexcept;
  void move(const double, const double);

  static bool is_in_infection_range(const Person&, const Person&);

private:
  Vector2d m_position, m_velocity;
  const double m_radius;
  double m_time_of_infection;
  Person::InfectionStatus m_infection_status;
  std::optional<Person::InfectionStatus> m_next_infection_status;
};

struct _PersonData {
  double x, y, vx, vy, radius;
  Person::InfectionStatus status;
};

std::ostream& operator<<(std::ostream&, const Person::InfectionStatus);
std::istream& operator>>(std::istream&, Person::InfectionStatus&);
std::ostream& operator<<(std::ostream&, const Person&);
std::istream& operator>>(std::istream&, _PersonData&);

#endif // PERSON_H
