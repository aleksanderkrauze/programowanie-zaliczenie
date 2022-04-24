#ifndef CITY_H
#define CITY_H

#include <vector>
#include <ostream>

#include "config.h"
#include "person.h"
#include "vector2d.h"

class City {
public:
  City(const double, const double, const double, const double);
  City(City&&) = default;
  ~City() = default;

  City& operator=(City&&) = default;

  void add_person(Person&&);
  const std::vector<Person>& people() const noexcept;

  void run_simulation(const Config&);
  void run_frame();
  void write_state(std::ostream&) const;

  static bool is_in_bound(const Vector2d&, const double);
  static City from_config(const Config&);

private:
  double m_city_size, m_time, m_dt, m_recovery_time, m_current_time;
  std::vector<Person> m_people;

  City(const City&) = default;
  City& operator=(const City&) = default;

  void update_recovering() noexcept;
  void infection() noexcept;
  void move() noexcept;
};

#endif // CITY_H
