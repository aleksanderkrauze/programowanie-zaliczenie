#include <exception>
#include <istream>
#include <ostream>
#include <string>
#include <variant>

#include "config.h"
#include "exceptions.h"

SimulationType Config::simulation_type() const {
  const auto& variant = this->data;

  if (std::holds_alternative<ConfigTest>(variant)) {
    return SimulationType::TEST;
  } else if (std::holds_alternative<ConfigRandom>(variant)) {
    return SimulationType::RANDOM;
  } else if (std::holds_alternative<ConfigFile>(variant)) {
    return SimulationType::FILE;
  } else {
    throw std::runtime_error("Unknown simulation variant");
  }
}

std::ostream& operator<<(std::ostream& os, const SimulationType type) {
  switch (type) {
  case SimulationType::TEST:
    return os << "test";
  case SimulationType::RANDOM:
    return os << "random";
  case SimulationType::FILE:
    return os << "file";
  default:
    throw std::runtime_error("Unknown simulation type");
  }
}

std::istream& operator>>(std::istream& is, SimulationType& type) {
  std::string buff;
  is >> buff;

  if (buff == "test") {
    type = SimulationType::TEST;
  } else if (buff == "random") {
    type = SimulationType::RANDOM;
  } else if (buff == "file") {
    type = SimulationType::FILE;
  } else {
    throw EnumClassException("simulation type", buff);
  }

  return is;
}