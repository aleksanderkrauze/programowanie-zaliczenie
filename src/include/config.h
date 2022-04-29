#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <istream>
#include <optional>
#include <ostream>
#include <string>
#include <variant>

enum class SimulationType { TEST, RANDOM, FILE };

struct _ConfigBasePeople {
  std::uint32_t n_people;
};
struct _ConfigBaseTime {
  double time, dt, recovery_time;
};

// Actual config struct used by user
struct ConfigTest {};
struct ConfigRandom : public _ConfigBasePeople, public _ConfigBaseTime {};
struct ConfigFile : public _ConfigBasePeople, public _ConfigBaseTime {
  double city_size;
  std::string input_file;
};

struct Config {
  std::variant<ConfigTest, ConfigRandom, ConfigFile> data;
  bool save_initial_state;
  bool save_final_state;
  bool save_frames;

  // functions
  SimulationType simulation_type() const;
};

std::ostream& operator<<(std::ostream&, const SimulationType);
std::istream& operator>>(std::istream&, SimulationType&);

#endif // CONFIG_H