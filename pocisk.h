#pragma once
#include "rysowanie.h"
#include "pojazdy.h"

class Pocisk
{
public:
	static const int height = 40;
	static const int width = 3;
	int i = 60;
private:
	int x;
	int y;
	Uint32 kolor;
public:
	Pocisk(int xx, int yy, int kk) : x(xx), y(yy), kolor(kk) {}
	int X() { return x; }
	int Y() { return y; }
	bool Rysuj_pocisk(SDL_Surface* screen);
	bool Sprawdz_czy_trafil(Pojazd* pojazd);
};