#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <istream>
#include <ostream>

struct Config {
  enum class SimulationType { TEST, RANDOM, FILE };
  Config::SimulationType simulation_type;

  double city_size;
  std::uint32_t n_people;
  double time;
  double dt;
  double recovery_time;

  bool save_initial_state;
  bool save_final_state;
  bool save_frames;
  bool save_animation;
};

std::ostream& operator<<(std::ostream&, const Config::SimulationType);
std::istream& operator>>(std::istream&, Config::SimulationType&);

#endif // CONFIG_H