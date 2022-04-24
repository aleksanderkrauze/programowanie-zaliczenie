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
  // This is a dirty hack, but it seems to work.
  // Potentaily move it outside this function.
  char env[] = "MPLBACKEND=agg";
  putenv(env);

  // This access and modification is safe, because this program uses only one
  // thread (main).
  if (!py_interpreter_guard) {
    py_interpreter_guard.emplace(py::scoped_interpreter{});
  }

  auto plt = matplotlibcpp17::pyplot::import();

  auto [fig, ax] = plt.subplots();
  ax.set_aspect(Args(1));
  ax.set_xlim(Args(0.0, plot_size));
  ax.set_ylim(Args(0.0, plot_size));

  for (const auto& p : people) {
    const auto [x, y] = p.position().tuple();
    std::ostringstream colour;
    colour << p.infection_status();

    auto circle = matplotlibcpp17::patches::Circle(
      Args(Args(x, y), p.radius()),
      Kwargs("alpha"_a = 0.5, "color"_a = colour.str()));
    ax.add_artist(Args(circle.unwrap()));
  }

  const int frameNumberWidth = 4;
  std::ostringstream ostr;
  ostr << "plots/frame_" << std::setfill('0') << std::setw(frameNumberWidth)
       << iteration_number << ".png";
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
