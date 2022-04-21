#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

#include "person.h"

class SimulationBaseException : public std::exception {
public:
  SimulationBaseException() = default;
  explicit SimulationBaseException(const char*) noexcept;
  explicit SimulationBaseException(const std::string&) noexcept;
  ~SimulationBaseException() = default;

  virtual const char* what() const noexcept override;

protected:
  std::string m_msg;
};

class OutOfCityBoundsException : public SimulationBaseException {
public:
  explicit OutOfCityBoundsException(const char*) noexcept;
  explicit OutOfCityBoundsException(const std::string&) noexcept;
  OutOfCityBoundsException(const Person&, const double) noexcept;
};

class RequiredPositiveDoubleValueException : public SimulationBaseException {
public:
  explicit RequiredPositiveDoubleValueException(const char*,
                                                const double) noexcept;
  explicit RequiredPositiveDoubleValueException(const std::string&,
                                                const double) noexcept;
};

class Line2dException : public SimulationBaseException {
public:
  Line2dException() noexcept;
};

class EnumClassException : public SimulationBaseException {
public:
  EnumClassException(const char*, const char*) noexcept;
  EnumClassException(const char*, const std::string&) noexcept;
};

class ArgumentsParsingException : public SimulationBaseException {
public:
  ArgumentsParsingException(const char*) noexcept;
  ArgumentsParsingException(const std::string&) noexcept;
};

#endif // EXCEPTIONS_H
