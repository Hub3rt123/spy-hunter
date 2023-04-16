#include "droga.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

bool Droga::Rysuj_droge()
{
	if (j > 0 && i == 0) j--;
	else i++;
	if (y + i + dlugosc > SCREEN_HEIGHT) j++;
	DrawRectangle(screen, x, y + i, szerokosc, dlugosc - j, kolor, kolor);
	if (next != NULL) next->Rysuj_droge();
	else if (next == NULL && j == 0 && i == 0) this->Stworz_droge();

	if (dlugosc == j) return false;
	else return true;
}

void Droga::Stworz_droge()
{
	int losowa_delta_x = (rand() % (SCREEN_WIDTH / 6)) - SCREEN_WIDTH / 12;
	while (x + losowa_delta_x >= SCREEN_WIDTH * 2 / 3 || x + losowa_delta_x < 5) losowa_delta_x = (rand() % (SCREEN_WIDTH / 6)) - SCREEN_WIDTH / 12;
	int losowa_szerokosc = (rand() % (SCREEN_WIDTH / 3)) + SCREEN_WIDTH / 3;
	while (x + losowa_delta_x + losowa_szerokosc >= SCREEN_WIDTH - 5) 
		losowa_szerokosc = (rand() % (SCREEN_WIDTH / 3)) + SCREEN_WIDTH / 3;
	next = new Droga(x + losowa_delta_x, y, losowa_szerokosc, SCREEN_HEIGHT - 42, kolor, screen);
}

