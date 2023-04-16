#ifndef DROGA_H
#define DROGA_H
 
#include "rysowanie.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Droga
{
private:
	int x;
	int y;
	int szerokosc;
	int dlugosc;
	Uint32 kolor;
	Droga* next;
	SDL_Surface* screen;
	int i = 0;
	int j;
public:
	Droga(int xx, int yy, int sz, int dl, Uint32 kk, SDL_Surface* scr)
		: x(xx), y(yy), szerokosc(sz), dlugosc(dl), next(NULL), kolor(kk), screen(scr), j(dl) {}
	Droga(int xx, int yy, int sz, int dl, Uint32 kk, SDL_Surface* scr, int ii, int jj)
		: x(xx), y(yy), szerokosc(sz), dlugosc(dl), next(NULL), kolor(kk), screen(scr), j(jj), i(ii) {}
	Droga() : x(0), y(0), szerokosc(0), dlugosc(0), next(NULL), kolor(0), screen(0), j(0) {}
	Droga* Next() { return next; }
	bool Rysuj_droge();
	void Stworz_droge();
	int X() { return x; }
	int Y() { return y; }
	int I() { return i; }
	int J() { return j; }
	int Dlugosc() { return dlugosc; }
	int Szerokosc() { return szerokosc; }
};

#endif