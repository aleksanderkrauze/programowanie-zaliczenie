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
  s << "Person's coordinates " << p.position()
    << " are outside of City's bounds: [0, " << city_size << "]";

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
  const std::string& arg_name, const double value) noexcept
    : RequiredPositiveDoubleValueException{arg_name.c_str(), value} {}

/* **********************************************
 * Line2dException
 * *********************************************/
Line2dException::Line2dException() noexcept {
  this->m_msg = "Line2d error: cannot create Line2d (Ax + By + C = 0) with "
                "both A and B beeing 0.";
}

/* **********************************************
 * EnumClassException
 * *********************************************/
EnumClassException::EnumClassException(const char* enum_name,
                                       const char* value) noexcept {
  std::ostringstream os;
  os << "Unknown " << enum_name << " value: " << value;

  this->m_msg = os.str();
}

EnumClassException::EnumClassException(const char* enum_name,
                                       const std::string& value) noexcept
    : EnumClassException{enum_name, value.c_str()} {}

/* **********************************************
 * ArgumentsParsingException
 * *********************************************/
ArgumentsParsingException::ArgumentsParsingException(const char* msg) noexcept
    : SimulationBaseException{msg} {}

ArgumentsParsingException::ArgumentsParsingException(
  const std::string& msg) noexcept
    : SimulationBaseException{msg} {}

/* **********************************************
 * IOException
 * *********************************************/
IOException::IOException(const char* msg) noexcept
    : SimulationBaseException{msg} {}

IOException::IOException(const std::string& msg) noexcept
    : SimulationBaseException{msg} {}