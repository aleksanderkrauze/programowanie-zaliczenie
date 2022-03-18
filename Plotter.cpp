#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include "matplotlibcpp.h"
#include "Plotter.h"

namespace plt = matplotlibcpp;

///////////////////////////////////////////////
// Rysowanie stanu miasta (położenia wszystkich osób) w danym kroku czasowym
// (timeStep). Zapisywanie rysunku do pliku o nazwie plots/frame_xxxx.png
// gdzie xxxx oznacza numer klatki (kolejnego kroku czasowego)
///////////////////////////////////////////////
void Plotter::plot(vector<Person>  & objects, unsigned int timeStep){

  double xLimHist = xx;
  double yLimHist = yy;
  
  plt::xlim(0., xLimHist);
  plt::ylim(0., yLimHist);  
  plt::xlabel("x");
  plt::ylabel("y");

  double radius;
  double circleArea;

  std::vector<double> x, y;
  double radiusToPixel = 200;
  for(auto it: objects){
    radius = it.getRadius()*radiusToPixel;
    circleArea = M_PI*radius*radius;    
    x.push_back(it.getX());
    y.push_back(it.getY());
    plt::scatter(x,y,circleArea, {{"color",it.getColor()}}); 
    x.clear();
    y.clear();
  }

  std::ostringstream ostr;
  int frameNumberWidth = 4;
  ostr<< "plots/frame_"<< std::setfill('0') << std::setw(frameNumberWidth) <<timeStep<<".png";
  plt::save(ostr.str());
  plt::close();
}
///////////////////////////////////////////////
// Tworzenie animowanego pliku w formacie gif ze wszystkich dostępnych
// stop klatek zapisanych w plikach frame_xxxx.png
// Użycie polecenia system i programu convert (dostępne na komputerach OKWF).
///////////////////////////////////////////////
void Plotter::makeAnimation() const{

  std::cout<<"Plotter: making animation"<<std::endl;
  std::string command = "cd plots; convert frame_*.png animation.gif";
  system(command.c_str());
  std::cout<<"Done."<<std::endl;
}

///////////////////////////////////////////////
// Usuwanie starych rysunków i animacji
///////////////////////////////////////////////
void Plotter::clean(){
  system("cd plots; rm -f animation.gif; rm -f frame*.png");
} 
