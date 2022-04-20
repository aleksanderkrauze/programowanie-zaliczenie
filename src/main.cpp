#include <iostream>

#include "tclap/CmdLine.h"

int main(int argc, char* argv[]) {
  try {
    TCLAP::CmdLine cmd{"Simulation of a pandemic", ' ', "0.1"};

    cmd.parse(argc, argv);

  } catch (const TCLAP::ArgException& e) {
    std::cerr << "Parsing error: " << e.error() << " for argument " << e.argId()
              << std::endl;
  }

  return 0;
}
