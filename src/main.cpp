#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tclap/CmdLine.h"

#include "city.h"
#include "config.h"
#include "exceptions.h"

Config get_config(const int, const char*[]);

int main(const int argc, const char* argv[]) {
  Config config;

  try {
    config = get_config(argc, argv);
  } catch (const TCLAP::ArgException& e) {
    std::cerr << "Parsing error: " << e.error() << " for argument " << e.argId()
              << "\n";

    return 1;
  } catch (const EnumClassException& e) {
    std::cerr << "Parsing error: " << e.what() << "\n";

    return 1;
  } catch (const ArgumentsParsingException& e) {
    std::cerr << "Parsing error: " << e.what() << "\n";

    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Caught unexpected error while parsing CLI arguments: "
              << e.what() << "\n";

    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception while parsing CLI arguments"
              << "\n";

    return 1;
  }

  try {
    auto city = City::from_config(config);

    const auto save_state = [&city](const auto& filename) -> void {
      std::cout << "Saving City's state to " << filename << "\n";

      std::ofstream file;
      file.exceptions(std::fstream::failbit | std::fstream::badbit);
      try {
        file.open(filename);
        city.write_state(file);
      } catch (const std::fstream::failure&) {
        std::ostringstream s;
        s << "Couldn't write to file " << filename;
        throw IOException(s.str());
      }
    };

    if (config.save_initial_state) {
      save_state("initial_state.txt");
    }

    city.run_simulation(config);

    if (config.save_final_state) {
      save_state("final_state.txt");
    }
  } catch (const SimulationBaseException& e) {
    std::cerr << "Error: " << e.what() << "\n";

    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Caught unexpected error: " << e.what() << "\n";

    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception"
              << "\n";

    return 1;
  }

  return 0;
}

Config get_config(const int argc, const char* argv[]) {
  Config config;

  TCLAP::CmdLine cmd = {"Simulation of a pandemic", ' ', "0.1"};

  TCLAP::ValueArg<SimulationType> type_arg = {
    "",
    "type",
    "Type of simulation. Possible values are: test, random, file.",
    true,
    SimulationType::TEST,
    "enumeration",
    cmd};
  TCLAP::ValueArg<double> city_size_arg = {
    "s", "size", "Size of a city", false, 0.0, "float", cmd};
  TCLAP::ValueArg<std::uint32_t> n_people_arg = {
    "n", "nPeople", "Number of people", false, 0, "number", cmd};
  TCLAP::ValueArg<double> time_arg = {
    "t", "time", "Time of simulation", false, 0.0, "float", cmd};
  TCLAP::ValueArg<double> dt_arg = {"",      "dt", "Value of dt", false, 0.0,
                                    "float", cmd};
  TCLAP::ValueArg<double> recovery_time_arg = {
    "r",
    "recoveryTime",
    "Time for a person to recover from a infection",
    false,
    0.0,
    "float",
    cmd};
  TCLAP::ValueArg<std::string> input_file_arg = {
    "i",
    "input",
    "File from which initial configuration is loaded when --type is file. "
    "Defaults to input_configuration.txt",
    false,
    "input_configuration.txt",
    "FILE",
    cmd};
  TCLAP::SwitchArg save_initial_state_switch = {
    "", "saveInitialState",
    "When turned on the initial city state will be "
    "saved to a file `initial_state.txt`",
    cmd};
  TCLAP::SwitchArg save_final_state_switch = {
    "", "saveFinalState",
    "When turned on the final city state will be "
    "saved to a file `final_state.txt`",
    cmd};
  TCLAP::SwitchArg save_frames_switch = {
    "", "save",
    "When turned on simulation frames will be saved in the plots/ directory",
    cmd};

  cmd.parse(argc, argv);

  // Can be called only after config.data has been set
  const auto get_or_throw = [&config](const auto& arg) {
    if (arg.isSet()) {
      return arg.getValue();
    }

    std::ostringstream s;
    s << "--" << arg.getName() << " must be specified when simulation type is: "
      << config.simulation_type();
    throw ArgumentsParsingException(s.str());
  };

  const auto warn_unused = [](const auto& arg) -> void {
    if (arg.isSet()) {
      std::ostringstream s;
      s << "Warning: argument --" << arg.getName() << " will be ignored.";

      std::cerr << s.str() << "\n";
    }
  };

  const auto simulation_type = type_arg.getValue();
  if (simulation_type == SimulationType::TEST) {
    ConfigTest data;
    config.data.emplace<ConfigTest>(data);

    warn_unused(city_size_arg);
    warn_unused(n_people_arg);
    warn_unused(time_arg);
    warn_unused(dt_arg);
    warn_unused(recovery_time_arg);
    warn_unused(input_file_arg);
  } else if (simulation_type == SimulationType::RANDOM) {
    ConfigRandom data;
    data.n_people = get_or_throw(n_people_arg);
    data.time = get_or_throw(time_arg);
    data.dt = get_or_throw(dt_arg);
    data.recovery_time = get_or_throw(recovery_time_arg);

    config.data.emplace<ConfigRandom>(data);

    warn_unused(city_size_arg);
    warn_unused(input_file_arg);
  } else if (simulation_type == SimulationType::FILE) {
    ConfigFile data;
    data.city_size = get_or_throw(city_size_arg);
    data.time = get_or_throw(time_arg);
    data.dt = get_or_throw(dt_arg);
    data.recovery_time = get_or_throw(recovery_time_arg);
    data.input_file = input_file_arg.getValue();

    config.data.emplace<ConfigFile>(data);

    warn_unused(n_people_arg);
  } else {
    throw std::runtime_error("Unexcpected Simulation Type variant");
  }

  config.save_initial_state = save_initial_state_switch.getValue();
  config.save_final_state = save_final_state_switch.getValue();
  config.save_frames = save_frames_switch.getValue();

  return config;
}
