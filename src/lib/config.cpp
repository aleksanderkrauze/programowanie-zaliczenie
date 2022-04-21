#include <istream>
#include <ostream>
#include <exception>
#include <string>

#include "config.h"
#include "exceptions.h"

std::ostream& operator<<(std::ostream& os, const Config::SimulationType type) {
  switch(type) {
  case Config::SimulationType::TEST:
    os << "test";
    break;
  case Config::SimulationType::RANDOM:
    os << "random";
    break;
  case Config::SimulationType::FILE:
    os << "file";
    break;
  default:
    throw std::runtime_error("Unknown simulation type");
  }
  
  return os;
}

std::istream& operator>>(std::istream& is, Config::SimulationType& type) {
  std::string buff;
  is >> buff;
  
  if(buff == "test") {
    type = Config::SimulationType::TEST;
  } else if(buff == "random") {
    type = Config::SimulationType::RANDOM;
  } else if(buff == "file") {
    type = Config::SimulationType::FILE;
  } else {
    throw EnumClassException("simulation type", buff);
  }
  
  return is;
}