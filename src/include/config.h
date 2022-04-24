#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

struct Config {
  enum class SimulationType { TEST, RANDOM, FILE };
  Config::SimulationType simulation_type;

  double city_size;
  std::uint32_t n_people;
  double time;
  double dt;
  double recovery_time;
  // This is used only if simulation_type is FILE
  std::string input_file;

  bool save_initial_state;
  bool save_final_state;
  bool save_frames;
};

std::ostream& operator<<(std::ostream&, const Config::SimulationType);
std::istream& operator>>(std::istream&, Config::SimulationType&);

#endif // CONFIG_H