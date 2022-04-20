#include <cmath> // M_PI const, sqrt
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "exceptions.h"
#include "line2d.h"
#include "person.h"
#include "vector2d.h"

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */
Person::Person(const Vector2d position, const Vector2d velocity,
               const double radius, const InfectionStatus status)
    : m_position{position}, m_velocity{velocity}, m_radius{radius},
      m_time_of_infection{0.0}, m_infection_status{status} {
  if (radius <= 0) {
    throw RequiredPositiveDoubleValueException("radius", radius);
  }
}

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */
Person::Person(const Vector2d position, const Vector2d velocity,
               const double radius)
    : Person{position, velocity, radius, Person::InfectionStatus::GREEN} {}

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equal to zero
 */
Person::Person(const double x, const double y, const double vx, const double vy,
               const double radius, const InfectionStatus status)
    : Person{{x, y}, {vx, vy}, radius, status} {}

/**
 * # Exceptions
 * - Throws std::invalid_argument when @radius is less than or equalt to zero
 */

Person::Person(const double x, const double y, const double vx, const double vy,
               const double radius)
    : Person{{x, y}, {vx, vy}, radius, Person::InfectionStatus::GREEN} {}

Vector2d Person::position() const noexcept { return this->m_position; }

void Person::position(const Vector2d position) noexcept {
  this->m_position = position;
}

Vector2d Person::velocity() const noexcept { return this->m_velocity; }

void Person::velocity(const Vector2d velocity) noexcept {
  this->m_velocity = velocity;
}

double Person::radius() const noexcept { return this->m_radius; }

double Person::time_of_infection() const noexcept {
  return this->m_time_of_infection;
}

void Person::time_of_infection(const double time) noexcept {
  this->m_time_of_infection = time;
}

Person::InfectionStatus Person::infection_status() const noexcept {
  return this->m_infection_status;
}

void Person::infection_status(const Person::InfectionStatus status) noexcept {
  this->m_infection_status = status;
}

/**
 * # Exceptions:
 * - Throws OutOfCityBoundsException when starting position would be outside
 * city bounds with given @city_size
 * - Throws RequiredPositiveDoubleValueException when @dt or @city_size are not
 * greater than 0
 */
void Person::move(const double dt, const double city_size) {
  if (dt <= 0.0) {
    throw RequiredPositiveDoubleValueException("dt", dt);
  }
  if (city_size <= 0.0) {
    throw RequiredPositiveDoubleValueException("city_size", city_size);
  }

  const Line2d right_edge{{city_size, 0.0}, {0.0, 1.0}};
  const Line2d top_edge{{city_size, city_size}, {-1.0, 0.0}};
  const Line2d left_edge{{0.0, city_size}, {0.0, -1.0}};
  const Line2d bottom_edge{{0.0, 0.0}, {1.0, 0.0}};

  Vector2d translation = this->m_velocity * dt;

  // Returns vector's y compound.
  const auto vertical = [](const auto& v) { return v.y(); };
  // Returns vector's x compound.
  const auto horizontal = [](const auto& v) { return v.x(); };

  // Returns true <=> given @val is inside [0, city_size].
  const auto compound_in_city_bounds = [city_size](const auto val) {
    return (0 <= val) && (val <= city_size);
  };

  // Returns true <=> point @p is inside city with size @city_size.
  const auto is_in_city = [&](const auto& p) {
    // We must check position in relation to lines, because due to floating
    // point arithmetic we could be *slightly* outside, but is reality on the
    // edge.
    return !(right_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
           !(top_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
           !(left_edge.point_position(p) == Line2d::PointPosition::RIGHT) &&
           !(bottom_edge.point_position(p) == Line2d::PointPosition::RIGHT);
  };

  // Returns intersection of given @edge and line passing through current
  // position and direction the same as vector's @v.
  const auto get_intersection = [this](const auto& edge, const auto& v) {
    const Line2d path{this->m_position, v};
    return Line2d::intersection(edge, path);
  };

  // Returns true <=> vector from @point_no_edge to @point_outside_city
  // intersects city.
  const auto vector_intersects_city = [&](const auto& point_on_edge,
                                          const auto& point_outside_city) {
    if (right_edge.point_position(point_on_edge) ==
        Line2d::PointPosition::MIDDLE) {
      return right_edge.point_position(point_outside_city) ==
             Line2d::PointPosition::LEFT;
    } else if (top_edge.point_position(point_on_edge) ==
               Line2d::PointPosition::MIDDLE) {
      return top_edge.point_position(point_outside_city) ==
             Line2d::PointPosition::LEFT;
    } else if (left_edge.point_position(point_on_edge) ==
               Line2d::PointPosition::MIDDLE) {
      return left_edge.point_position(point_outside_city) ==
             Line2d::PointPosition::LEFT;
    } else if (bottom_edge.point_position(point_on_edge) ==
               Line2d::PointPosition::MIDDLE) {
      return bottom_edge.point_position(point_outside_city) ==
             Line2d::PointPosition::LEFT;
    } else {
      throw std::runtime_error("Person::move(): vector_intersects_city: passed "
                               "point is not on eny edge");
    }
  };

  // Returns true <=> given @intersection satisfy following condidtions:
  // - intersection is not empty
  // - it is proper intersection for current @translation
  // - it's proper @compound is in bounds of [0, @city_size]
  const auto is_intersection_ok =
    [this, &translation, &compound_in_city_bounds, &is_in_city,
     &vector_intersects_city](const auto& intersection, const auto& compound) {
      if (!intersection) {
        return false;
      }

      const auto vector = intersection.value();

      // If point of intersection is the same as current position then
      // depending on the direction of translation vector (does it point inside
      // or outside the city) this is corrent point or not. Otherwise we check
      // if direction of translation vector and vector form out position to the
      // point of intersection point in the same direction (by checking if their
      // scalar product is greater then 0). If not, then it is not the point we
      // are looking for.
      const auto from_position_to_intersection = vector - this->m_position;
      if (from_position_to_intersection == Vector2d{}) {
        const auto point_outside_city = this->m_position + translation;
        if (is_in_city(point_outside_city)) {
          return false;
        } else {
          return !vector_intersects_city(this->m_position, point_outside_city);
        }
      } else if (translation * from_position_to_intersection < 0) {
        return false;
      }

      const auto val = compound(vector);
      return compound_in_city_bounds(val);
    };

  // Moves person to given @intersection_point and reflects it's velocity and
  // remaining @translation agains given @edge.
  const auto partial_move = [this, &translation, city_size](
                              const auto intersection_point, const auto& edge) {
    const auto to_edge = intersection_point - this->m_position;
    translation -= to_edge;
    this->m_position = intersection_point;

    // If intersection point is in the corner reverse speed, otherwise reflect
    // it from passed edge.
    if (intersection_point == Vector2d{0.0, 0.0} ||
        intersection_point == Vector2d{city_size, 0.0} ||
        intersection_point == Vector2d{city_size, city_size} ||
        intersection_point == Vector2d{0.0, city_size}) {
      translation *= -1;
      this->m_velocity *= -1;
    } else {
      translation.reflect(edge.normal());
      this->m_velocity.reflect(edge.normal());
    }
  };

  // Checking if starting position is inside city bounds
  if (!is_in_city(this->m_position)) {
    std::ostringstream msg;
    msg << "Called Person::move() with parameter city_size = " << city_size
        << " on person with position = " << this->m_position;
    throw OutOfCityBoundsException(msg.str());
  }

  // Set initial position to one that is illegal.
  Vector2d previous_position{-1, -1};

  // Moving Person
  while (true) {
    const Vector2d candidate_position = this->m_position + translation;
    if (is_in_city(candidate_position)) {
      this->m_position = candidate_position;
      break;
    }

    const auto right_interscetion = get_intersection(right_edge, translation);
    const auto top_interscetion = get_intersection(top_edge, translation);
    const auto left_interscetion = get_intersection(left_edge, translation);
    const auto bottom_interscetion = get_intersection(bottom_edge, translation);

    if (is_intersection_ok(right_interscetion, vertical)) {
      partial_move(right_interscetion.value(), right_edge);
    } else if (is_intersection_ok(top_interscetion, horizontal)) {
      partial_move(top_interscetion.value(), top_edge);
    } else if (is_intersection_ok(left_interscetion, vertical)) {
      partial_move(left_interscetion.value(), left_edge);
    } else if (is_intersection_ok(bottom_interscetion, horizontal)) {
      partial_move(bottom_interscetion.value(), bottom_edge);
    } else {
      throw std::runtime_error("Person::move(): unreachable condidtion");
    }

    // If position didn't change we encountered some weird runtime error.
    if (previous_position == this->m_position) {
      throw std::runtime_error("Person::move(): position didn't change");
    } else {
      previous_position = this->m_position;
    }
  }
}

bool Person::is_in_infection_range(const Person& p1, const Person& p2) {
  return Vector2d::distance(p1.m_position, p2.m_position) <=
         (p1.m_radius + p2.m_radius);
}

std::ostream& operator<<(std::ostream& os,
                         const Person::InfectionStatus status) {
  switch (status) {
  case Person::InfectionStatus::GREEN:
    os << "green";
    break;
  case Person::InfectionStatus::RED:
    os << "red";
    break;
  case Person::InfectionStatus::BLUE:
    os << "blue";
    break;
  default:
    throw std::runtime_error("Unexpected infection status");
    break;
  }

  return os;
}