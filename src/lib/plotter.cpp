#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

#include "config.h"
#include "matplotlibcpp.h"
#include "person.h"
#include "plotter.h"

namespace plt = matplotlibcpp;

namespace plotter {

void plot(const std::vector<Person>& people,
          const std::uint32_t iteration_number, const double plot_size) {
  plt::backend("Agg");
  plt::figure_size(1000, 1000);
  plt::xlim(0.0, plot_size);
  plt::ylim(0.0, plot_size);
  plt::set_aspect_equal();

  const double radius_to_pixel = 200;
  for (const auto& p : people) {
    const auto [x, y] = p.position().tuple();
    const std::vector<double> xs = {x};
    const std::vector<double> ys = {y};

    const auto radius = p.radius() * radius_to_pixel;
    const auto circle_area = M_PI * radius * radius;

    std::ostringstream colour;
    colour << p.infection_status();

    plt::scatter(xs, ys, circle_area, {{"color", colour.str()}});
  }

  const int frameNumberWidth = 4;
  std::ostringstream ostr;
  ostr << "plots/frame_" << std::setfill('0') << std::setw(frameNumberWidth)
       << iteration_number << ".png";
  plt::save(ostr.str());
  plt::close();
}

} // namespace plotter
