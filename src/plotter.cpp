#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

#include "matplotlibcpp.h"
#include "plotter.h"
#include "person.h"

namespace plt = matplotlibcpp;

namespace plotter {
	void plot(const std::vector<Person>& people, std::uint32_t iteration_number, double plot_size) {
		plt::xlim(0.0, plot_size);
		plt::ylim(0.0, plot_size);  
		plt::xlabel("x");
		plt::ylabel("y");
	
		for(const auto& p: people){
			p.draw();	
		}
	
		std::ostringstream ostr;
		int frameNumberWidth = 4;
		ostr << "plots/frame_" << std::setfill('0') << std::setw(frameNumberWidth) << iteration_number <<".png";
		plt::save(ostr.str());
		plt::close();
	}
	
	void makeAnimation() {
	  std::cout<<"Plotter: making animation"<<std::endl;
	  std::string command = "cd plots; convert frame_*.png animation.gif";
	  system(command.c_str());
	  std::cout<<"Done."<<std::endl;
	}
	
	void clean() {
	  system("cd plots; rm -f animation.gif; rm -f frame*.png");
	} 
}
