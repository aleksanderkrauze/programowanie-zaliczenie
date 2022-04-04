#include <sstream>
#include <string>

#include "exceptions.h"
#include "person.h"

/* **********************************************
 * SimulationBaseException
 * *********************************************/
SimulationBaseException::SimulationBaseException(const char* msg) noexcept
    : m_msg{msg} {}

SimulationBaseException::SimulationBaseException(
  const std::string& msg) noexcept
    : m_msg{msg} {}

const char* SimulationBaseException::what() const noexcept {
  return this->m_msg.c_str();
}

/* **********************************************
 * OutOfCityBoundsException
 * *********************************************/
OutOfCityBoundsException::OutOfCityBoundsException(const char* msg) noexcept
    : SimulationBaseException{msg} {}

OutOfCityBoundsException::OutOfCityBoundsException(
  const std::string& msg) noexcept
    : SimulationBaseException{msg} {}

OutOfCityBoundsException::OutOfCityBoundsException(
  const Person& p, const double city_size) noexcept {
  std::ostringstream s;
  s << "Person's coordinates (x, y) = (" << p.x() << ", " << p.y()
    << ") are outside of City's bounds: [0, " << city_size << "]";

  this->m_msg = s.str();
}

/* **********************************************
 * RequiredPositiveDoubleValueException
 * *********************************************/
RequiredPositiveDoubleValueException::RequiredPositiveDoubleValueException(
  const char* arg_name, const double value) noexcept {
  std::ostringstream s;
  s << "Argument `" << arg_name
    << "` can have only positive values (greater that 0), but it's value was: "
    << value;

  this->m_msg = s.str();
}

RequiredPositiveDoubleValueException::RequiredPositiveDoubleValueException(
  const std::string& arg_name, const double value) noexcept {
  std::ostringstream s;
  s << "Argument `" << arg_name
    << "` can have only positive values (greater that 0), but it's value was: "
    << value;

  this->m_msg = s.str();
}