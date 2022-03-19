#ifndef PLOTTER_H
#define PLOTTER_H

#include <cstdint>
#include <vector>

// Abstract class that allows for drawing onto plots.
// In future should be replaced with a concept.
class Drawable {
	virtual void draw() const = 0;
	virtual ~Drawable() = 0;
};

namespace plotter {
	///////////////////////////////////////////////
	// Rysowanie stanu miasta (położenia wszystkich osób) w danym kroku czasowym
	// (timeStep). Zapisywanie rysunku do pliku o nazwie plots/frame_xxxx.png
	// gdzie xxxx oznacza numer klatki (kolejnego kroku czasowego)
	///////////////////////////////////////////////
	void plot(std::vector<Drawable>&, std::uint32_t);

	///////////////////////////////////////////////
	// Tworzenie animowanego pliku w formacie gif ze wszystkich dostępnych
	// stop klatek zapisanych w plikach frame_xxxx.png
	// Użycie polecenia system i programu convert (dostępne na komputerach OKWF).
	///////////////////////////////////////////////
	void makeAnimation();

	///////////////////////////////////////////////
	// Usuwanie starych rysunków i animacji
	///////////////////////////////////////////////
	void clean();
}

#endif // PLOTTER_H
