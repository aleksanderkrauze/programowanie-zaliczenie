#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>

// #include "matplotlibcpp.h"
#include "matplotlibcpp17/patches.h"
#include "matplotlibcpp17/pyplot.h"

#include "config.h"
#include "person.h"
#include "plotter.h"

// namespace plt = matplotlibcpp;

namespace plotter {

static std::optional<py::scoped_interpreter> py_interpreter_guard =
  std::nullopt;

void plot(const std::vector<Person>& people,
          const std::uint32_t iteration_number, const double plot_size,
          const Config& config) {
  // plt::backend("Agg");
  // plt::figure_size(1000, 1000);
  // plt::xlim(0.0, plot_size);
  // plt::ylim(0.0, plot_size);
  // plt::set_aspect_equal();

  // This is a dirty hack, but it seems to work.
  // Potentaily move it outside this function.
  char env[] = "MPLBACKEND=agg";
  putenv(env);

  // This access and modification is safe, because this program uses only one
  // (main) thread.
  if (!py_interpreter_guard) {
    py_interpreter_guard.emplace(py::scoped_interpreter{});
  }

  auto plt = matplotlibcpp17::pyplot::import();

  auto [fig, ax] = plt.subplots();
  ax.set_aspect(Args(1));
  ax.set_xlim(Args(0.0, plot_size));
  ax.set_ylim(Args(0.0, plot_size));

  const double radius_to_pixel = 200;
  for (const auto& p : people) {
    const auto [x, y] = p.position().tuple();
    const std::vector<double> xs = {x};
    const std::vector<double> ys = {y};

    const auto radius = p.radius() * radius_to_pixel;
    const auto circle_area = M_PI * radius * radius;

    std::ostringstream colour;
    colour << p.infection_status();

    // plt::scatter(xs, ys, circle_area, {{"color", colour.str()}});
    // plt.scatter(Args(xs, ys), Kwargs("s"_a = circle_area, "c"_a =
    // colour.str()));

    auto circle = matplotlibcpp17::patches::Circle(
      Args(Args(x, y), p.radius()),
      Kwargs("alpha"_a = 0.5, "color"_a = colour.str()));
    ax.add_artist(Args(circle.unwrap()));
  }

  const int frameNumberWidth = 4;
  std::ostringstream ostr;
  ostr << "plots/frame_" << std::setfill('0') << std::setw(frameNumberWidth)
       << iteration_number << ".png";
  // plt::save(ostr.str());
  // plt::close();
  plt.savefig(Args(ostr.str()));
  plt.cla();
}

void makeAnimation() {
  std::cout << "Plotter: making animation" << std::endl;
  std::string command = "cd plots; convert frame_*.png animation.gif";
  system(command.c_str());
  std::cout << "Done." << std::endl;
}

void clean() { system("cd plots; rm -f animation.gif; rm -f frame*.png"); }

} // namespace plotter
