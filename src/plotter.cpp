#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

#include "matplotlibcpp.h"
#include "plotter.h"

namespace plt = matplotlibcpp;

namespace plotter {
	void plot(std::vector<Drawable>& objects, std::uint32_t timeStep) {
	  // double xLimHist = xx;
	  // double yLimHist = yy;
	  
	  // plt::xlim(0., xLimHist);
	  // plt::ylim(0., yLimHist);  
	  // plt::xlabel("x");
	  // plt::ylabel("y");
	
	  // double radius;
	  // double circleArea;
	
	  // std::vector<double> x, y;
	  // double radiusToPixel = 200;
		// for(auto& it: objects){
			// radius = it.getRadius()*radiusToPixel;
			// circleArea = M_PI*radius*radius;    
			// x.push_back(it.getX());
			// y.push_back(it.getY());
			// plt::scatter(x,y,circleArea, {{"color",it.getColor()}}); 
			// x.clear();
			// y.clear();
		// }
	
	  // std::ostringstream ostr;
	  // int frameNumberWidth = 4;
	  // ostr<< "plots/frame_"<< std::setfill('0') << std::setw(frameNumberWidth) <<timeStep<<".png";
	  // plt::save(ostr.str());
	  // plt::close();
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
