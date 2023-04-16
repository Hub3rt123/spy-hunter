#include "pojazdy.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

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

MojPojazd::~MojPojazd() {
	if (pocisk != NULL) {
		delete(pocisk);
		pocisk = NULL;
	}
}

void MojPojazd::RysujPojazd(SDL_Surface* screen) {
	if (pocisk != NULL && pocisk->Rysuj_pocisk(screen) == false) {
			delete(pocisk);
			pocisk = NULL;
	}

	for (int i = 0; i < height; i++)
		DrawLine(screen, x + 5, y + i, width - 10, 1, 0, kolor);
	for (int i = 0; i < 5; i++) 
		DrawLine(screen, x + i, y, height / 3, 0, 1, kolor_kola);
	for (int i = 5; i > 0; i--)
		DrawLine(screen, x + width - i, y, height / 3, 0, 1, kolor_kola);
	for (int i = 0; i < 5; i++)
		DrawLine(screen, x + i, y + height * 2 / 3, height / 3, 0, 1, kolor_kola);
	for (int i = 5; i > 0; i--)
		DrawLine(screen, x + width - i, y + height * 2 / 3, height / 3, 0, 1, kolor_kola);
}

void MojPojazd::Skrec_w_lewo(int delta_x) {
	if (x - delta_x >= 0) x -= delta_x;
}

void MojPojazd::Skrec_w_prawo(int delta_x) {
	if (x + delta_x + width <= SCREEN_WIDTH) x += delta_x;
}

void MojPojazd::Jedz_do_przodu(int delta_y) {
	if (y + delta_y >= 100) y -= delta_y;
}

void MojPojazd::Jedz_do_tylu(int delta_y) {
	if (y + delta_y + height <= SCREEN_HEIGHT - height) y += delta_y;
}


double MojPojazd::Sprawdz_czy_na_drodze(Droga* droga) {
	if (droga->Y() >= y) droga = droga->Next();

	if (droga->X() > x + width || droga->X() + droga->Szerokosc() < x) return 10;
	else if (droga->X() > x || droga->X() + droga->Szerokosc() < x + width) return 0;
	else return 0.01;
}

bool MojPojazd::Sprawdz_zderzenie(Pojazd* wrogi_pojazd) {
	if ((x <= wrogi_pojazd->X() + wrogi_pojazd->width && y <= wrogi_pojazd->Y() + wrogi_pojazd->height &&
		 x >= wrogi_pojazd->X() && y >= wrogi_pojazd->Y()) || // lewy gorny rog
		(x <= wrogi_pojazd->X() + wrogi_pojazd->width && y + height >= wrogi_pojazd->Y() &&
		x >= wrogi_pojazd->X() && y + height <= wrogi_pojazd->Y() + wrogi_pojazd->height) || // lewy dolny rog
		(x + width >= wrogi_pojazd->X() && y <= wrogi_pojazd->Y() + wrogi_pojazd->height &&
		x + width <= wrogi_pojazd->X() + wrogi_pojazd->width && y >= wrogi_pojazd->Y()) || // prawy gorny rog
		(x + width >= wrogi_pojazd->X() && y + height >= wrogi_pojazd->Y() &&
		 x + width <= wrogi_pojazd->X() && y + height <= wrogi_pojazd->Y() + wrogi_pojazd->height)) { // prawy dolny rog
		zycia--;
		return true;
	}
	return false;
}

bool MojPojazd::Sprawdz_czy_zdobyty(ExtraBron* extra_bron) {
	if ((extra_bron->X() <= x + width && extra_bron->Y() <= y + height && extra_bron->X() >= x && extra_bron->Y() >= y) || // prawy dolny rog
		(extra_bron->X() <= x + width && extra_bron->Y() + extra_bron->height >= y &&
			extra_bron->X() >= x  && extra_bron->Y() + extra_bron->height <= y + height) || // prawy gorny rog
		(extra_bron->X() + extra_bron->width >= x && extra_bron->Y() <= y + height &&
			extra_bron->X() + extra_bron->width <= x + width && extra_bron->Y() >= y) || // lewy dolny rog
		(extra_bron->X() + extra_bron->width >= x && extra_bron->Y() + extra_bron->height >= y &&
			extra_bron->X() + extra_bron->width <= x && extra_bron->Y() + extra_bron->height <= y + height)) { // lewy gorny rog
		extra_pocisk += 5;
		return true;
	}
	return false;
}

void MojPojazd::Wystrzel_pocisk() {
	if (pocisk == NULL) {
		pocisk = new Pocisk(x + width / 2 - 2, y, kolor_pocisku);
		if (extra_pocisk > 0) {
			pocisk->i = 180;
			extra_pocisk--;
		}
	}
}

void WrogiPojazd::RysujPojazd(SDL_Surface* screen, Droga* droga) {
	for (int i = 0; i < height - delta_y && y < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + 5, y + i, width - 10, 1, 0, kolor);
	for (int i = 0; i < 5 && y < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + i, y, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 5; i > 0 && y < SCREEN_HEIGHT; i--)
		DrawLine(screen, x + width - i, y, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 0; i < 5 && y + height * 2 / 3 < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + i, y + height * 2 / 3, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 5; i > 0 && y + height * 2 / 3 < SCREEN_HEIGHT; i--)
		DrawLine(screen, x + width - i, y + height * 2 / 3, height / 3 - delta_y, 0, 1, kolor_kola);
	if (delta_y > 0 && y <= 41 + height) delta_y--;
	
	this->Poruszaj_pojazdem(droga);
}

void WrogiPojazd::Poruszaj_pojazdem(Droga* droga) {
	if (droga != NULL) {
		if (droga->Y() > y) droga = droga->Next();
		if (rand() % 3 == 0) {
			if (x + delta_x < droga->X()) delta_x = 1;
			else if (x + delta_x + width > droga->X() + droga->Szerokosc()) delta_x = -1;
			x += delta_x;
		}
		int temp = rand() % 6;
		if (temp == 0) y++;
		if (temp == 0 && y + height >= SCREEN_HEIGHT) delta_y++;
	}
}

void CywilnyPojazd::RysujPojazd(SDL_Surface* screen, Droga* droga) {
	for (int i = 0; i < height - delta_y && y < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + 5, y + i, width - 10, 1, 0, kolor);
	for (int i = 0; i < 5 && y < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + i, y, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 5; i > 0 && y < SCREEN_HEIGHT; i--)
		DrawLine(screen, x + width - i, y, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 0; i < 5 && y + height * 2 / 3 < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + i, y + height * 2 / 3, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 5; i > 0 && y + height * 2 / 3 < SCREEN_HEIGHT; i--)
		DrawLine(screen, x + width - i, y + height * 2 / 3, height / 3 - delta_y, 0, 1, kolor_kola);
	if (delta_y > 0 && y <= 41 + height) delta_y--;

	this->Poruszaj_pojazdem(droga);
}

void CywilnyPojazd::Poruszaj_pojazdem(Droga* droga) {
	if (droga != NULL) {
		if (droga->Y() > y) droga = droga->Next();
		if (rand() % 6 == 0) {
			if (x + delta_x < droga->X()) delta_x = 1;
			else if (x + delta_x + width > droga->X() + droga->Szerokosc()) delta_x = -1;
			x += delta_x;
		}
		int temp = rand() % 8;
		if (temp == 0) y++;
		if (temp == 0 && y + height >= SCREEN_HEIGHT) delta_y++;
	}
}

void ExtraBron::RysujBron(SDL_Surface* screen, Droga* droga) {
	for (int i = 0; i < height - delta_y && y < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + 2, y + i, width - 4, 1, 0, kolor);
	for (int i = 0; i < 2 && y < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + i, y, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 2; i > 0 && y < SCREEN_HEIGHT; i--)
		DrawLine(screen, x + width - i, y, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 0; i < 2 && y + height * 2 / 3 < SCREEN_HEIGHT; i++)
		DrawLine(screen, x + i, y + height * 2 / 3, height / 3 - delta_y, 0, 1, kolor_kola);
	for (int i = 2; i > 0 && y + height * 2 / 3 < SCREEN_HEIGHT; i--)
		DrawLine(screen, x + width - i, y + height * 2 / 3, height / 3 - delta_y, 0, 1, kolor_kola);
	if (delta_y > 0 && y <= 41 + height) delta_y--;
	else if (delta_y == 0 && y < SCREEN_HEIGHT - height) y++;
	else if (delta_y >= 0 && y >= SCREEN_HEIGHT - height && y < SCREEN_HEIGHT) { y++; delta_y++; }

}