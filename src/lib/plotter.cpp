#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

#include "matplotlibcpp.h"
#include "person.h"
#include "plotter.h"

namespace plt = matplotlibcpp;

namespace plotter {
void plot(const std::vector<Person>& people,
          const std::uint32_t iteration_number, const double plot_size) {
  plt::xlim(0.0, plot_size);
  plt::ylim(0.0, plot_size);
  plt::xlabel("x");
  plt::ylabel("y");

  const double radius_to_pixel = 200;
  for (const auto& p : people) {
    const auto [_x, _y] = p.position().tuple();
    const std::vector<double> x = {_x};
    const std::vector<double> y = {_y};

    const auto radius = p.radius() * radius_to_pixel;
    const auto circle_area = M_PI * radius * radius;

    const auto colour =
      Person::infection_status_to_colour(p.infection_status());

    matplotlibcpp::scatter(x, y, circle_area, {{"color", colour}});
  }

  const int frameNumberWidth = 4;
  std::ostringstream ostr;
  ostr << "plots/frame_" << std::setfill('0') << std::setw(frameNumberWidth)
       << iteration_number << ".png";
  plt::save(ostr.str());
  plt::close();
}

void makeAnimation() {
  std::cout << "Plotter: making animation" << std::endl;
  std::string command = "cd plots; convert frame_*.png animation.gif";
  system(command.c_str());
  std::cout << "Done." << std::endl;
}

void clean() { system("cd plots; rm -f animation.gif; rm -f frame*.png"); }
} // namespace plotter
