#ifndef PLOTTER_H
#define PLOTTER_H

#include <cstdint>
#include <vector>

#include "config.h"
#include "person.h"

namespace plotter {

void plot(const std::vector<Person>&, const std::uint32_t, const double);

} // namespace plotter

#endif // PLOTTER_H
