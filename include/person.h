#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
public:
  enum class InfectionStatus { GREEN, RED, BLUE };

  Person(const double, const double, const double, const double, const double,
         const InfectionStatus);
  Person(Person&&) = default;
  ~Person() = default;

  Person& operator=(Person&&) = default;

  double x() const noexcept;
  void x(double) noexcept;
  double y() const noexcept;
  void y(double) noexcept;
  double vx() const noexcept;
  void vx(double) noexcept;
  double vy() const noexcept;
  void vy(double) noexcept;
  double radius() const noexcept;
  double time_of_infection() const noexcept;
  void time_of_infection(double) noexcept;
  Person::InfectionStatus infection_status() const noexcept;
  void infection_status(Person::InfectionStatus) noexcept;

  void draw() const;
  void move(const double, const double);

  static bool is_in_infection_range(const Person&, const Person&);
  static std::string infection_status_to_colour(const Person::InfectionStatus);

private:
  double m_x, m_y, m_vx, m_vy, m_radius, m_time_of_infection;
  InfectionStatus m_infection_status;

  Person(const Person&) = default;
  Person& operator=(const Person&) = default;
};

#endif // PERSON_H
