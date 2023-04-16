#include "pocisk.h"

bool Pocisk::Rysuj_pocisk(SDL_Surface* screen) {
	if (i > 0 && y >= 44) {
		y -= 2;
		for (int j = 0; j < width; j++)
			DrawLine(screen, x + j, y, height, 0, 1, kolor);
		i--;
		return true;
	}
	else
		return false;
}

bool Pocisk::Sprawdz_czy_trafil(Pojazd* pojazd) {
	if ((x <= pojazd->X() + pojazd->width && y <= pojazd->Y() + pojazd->height &&
		x >= pojazd->X() && y >= pojazd->Y()) || // lewy gorny rog
		(x <= pojazd->X() + pojazd->width && y + height >= pojazd->Y() &&
			x >= pojazd->X() && y + height <= pojazd->Y() + pojazd->height) || // lewy dolny rog
		(x + width >= pojazd->X() && y <= pojazd->Y() + pojazd->height &&
			x + width <= pojazd->X() + pojazd->width && y >= pojazd->Y()) || // prawy gorny rog
		(x + width >= pojazd->X() && y + height >= pojazd->Y() &&
			x + width <= pojazd->X() && y + height <= pojazd->Y() + pojazd->height)) { // prawy dolny rog
		return true;
	}
	return false;
}