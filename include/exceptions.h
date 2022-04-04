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

#endif // EXCEPTIONS_H
