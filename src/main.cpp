#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tclap/CmdLine.h"

#include "city.h"
#include "config.h"
#include "exceptions.h"

int main(int argc, char* argv[]) {
  Config config;

  try {
    TCLAP::CmdLine cmd = {"Simulation of a pandemic", ' ', "0.1"};

    TCLAP::ValueArg<Config::SimulationType> type_arg = {
      "",
      "type",
      "Type of simulation. Possible values are: test, random, file.",
      true,
      Config::SimulationType::TEST,
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

    // Can be called only after config.simulation_type has been set.
    const auto get_or_throw = [&config](const auto& arg) {
      if (arg.isSet()) {
        return arg.getValue();
      }

      std::ostringstream s;
      s << "--" << arg.getName()
        << " must be specified when simulation type is: "
        << config.simulation_type;
      throw ArgumentsParsingException(s.str());
    };

    // Can be called only after config was full initialised
    const auto warn_unused = [](const auto& arg, const auto& value) {
      if (arg.isSet()) {
        std::ostringstream s;
        s << "Warning: argument --" << arg.getName()
          << " will be ignored. Using value `" << value << "` instead";

        std::cerr << s.str() << std::endl;
      }
    };
    const auto warn_unused_short = [](const auto& arg) {
      if (arg.isSet()) {
        std::ostringstream s;
        s << "Warning: argument --" << arg.getName() << " will be ignored.";

        std::cerr << s.str() << std::endl;
      }
    };

    switch (type_arg.getValue()) {
    case Config::SimulationType::TEST:
      config.simulation_type = Config::SimulationType::TEST;
      config.city_size = 0.25;
      config.n_people = 3;
      config.time = 2.0;
      config.dt = 0.02;
      config.recovery_time = 0.5;

      warn_unused(city_size_arg, config.city_size);
      warn_unused(n_people_arg, config.n_people);
      warn_unused(time_arg, config.time);
      warn_unused(dt_arg, config.dt);
      warn_unused(recovery_time_arg, config.recovery_time);
      warn_unused_short(input_file_arg);
      break;
    case Config::SimulationType::RANDOM:
      config.simulation_type = Config::SimulationType::RANDOM;
      config.city_size = 1.0;
      config.n_people = get_or_throw(n_people_arg);
      config.time = get_or_throw(time_arg);
      config.dt = get_or_throw(dt_arg);
      config.recovery_time = get_or_throw(recovery_time_arg);

      warn_unused(city_size_arg, config.city_size);
      warn_unused_short(input_file_arg);
      break;
    case Config::SimulationType::FILE:
      config.simulation_type = Config::SimulationType::FILE;
      config.city_size = get_or_throw(city_size_arg);
      // Set it to something
      config.n_people = 0;
      config.time = get_or_throw(time_arg);
      config.dt = get_or_throw(dt_arg);
      config.recovery_time = get_or_throw(recovery_time_arg);
      config.input_file = input_file_arg.getValue();

      warn_unused_short(n_people_arg);
      break;
    }
    config.save_initial_state = save_initial_state_switch.getValue();
    config.save_final_state = save_final_state_switch.getValue();
    config.save_frames = save_frames_switch.getValue();
  } catch (const TCLAP::ArgException& e) {
    std::cerr << "Parsing error: " << e.error() << " for argument " << e.argId()
              << std::endl;

    return 1;
  } catch (const EnumClassException& e) {
    std::cerr << "Parsing error: " << e.what() << std::endl;

    return 1;
  } catch (const ArgumentsParsingException& e) {
    std::cerr << "Parsing error: " << e.what() << std::endl;

    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Caught unexpected error: " << e.what() << std::endl;

    return 1;
  }

  try {
    auto city = City::from_config(config);

    const auto save_state = [&city](const auto filename) {
      std::cout << "Saving City's state to " << filename << std::endl;

      std::ofstream file;
      file.exceptions(std::fstream::failbit | std::fstream::badbit);
      try {
        file.open(filename);
        city.write_state(file);
        file.close();
      } catch (const std::fstream::failure&) {
        if (file.is_open()) {
          file.close();
        }

        std::ostringstream s;
        s << "Couldn't write to file " << filename;
        throw IOException(s.str());
      } catch (const std::exception&) {
        file.close();
        throw;
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
    std::cerr << "Error: " << e.what() << std::endl;

    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Caught unexpected error: " << e.what() << std::endl;

    return 1;
  }

  return 0;
}
