#ifndef PLOTTER_H
#define PLOTTER_H

#include<vector>
#include "Person.h"

using namespace std;

//////////////////////////////////////////////
// Klasa Plotter do 
//////////////////////////////////////////////

class Plotter{

 public:

  Plotter() = default;

  
  void plot( vector<Person> & objects, unsigned int timeStep);

  void makeAnimation() const;

  void clean(); 
  
};


#endif
