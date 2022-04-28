#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <filesystem>
#include <istream>
#include <optional>
#include <ostream>
#include <string>
#include <variant>

namespace fs = std::filesystem;

enum class SimulationType { TEST, RANDOM, FILE };

struct ConfigTest {};
struct ConfigRandom {
  std::uint32_t n_people;
  double time;
  double dt;
  double recovery_time;
};
struct ConfigFile {
  double city_size;
  std::uint32_t n_people;
  double time;
  double dt;
  double recovery_time;
  fs::path input_file;
};

struct Config {
  std::variant<ConfigTest, ConfigRandom, ConfigFile> data;
  bool save_initial_state;
  bool save_final_state;
  bool save_frames;

  // methods
  SimulationType simulation_type() const;
};

std::ostream& operator<<(std::ostream&, const SimulationType);
std::istream& operator>>(std::istream&, SimulationType&);

#endif // CONFIG_H