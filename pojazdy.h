#ifndef POJAZDY_H
#define POJAZDY_H

#include "rysowanie.h"
#include "droga.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Pojazd;
class ExtraBron;

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

class Pojazd
{
public:
	const static int height = 60;
	const static int width = 37;
protected:
	int x;
	int y;
	Uint32 kolor;
	Uint32 kolor_kola;
public:
	Pojazd(int xx, int yy, int k, int kk) : x(xx), y(yy),
			kolor(k), kolor_kola(kk) {}
	int X() { return x; }
	int Y() { return y; }
};

class MojPojazd : public Pojazd
{
private:
	Uint32 kolor_pocisku;
	Pocisk* pocisk;
	int zycia;
	int extra_pocisk;
public:
	MojPojazd(int k = 0, int kk = 0, int kp = 0, int xx = (SCREEN_WIDTH - MojPojazd::width) / 2, int yy = SCREEN_HEIGHT - 160, int z = 1000, int e = 0)
		: Pojazd(xx, yy, k, kk), kolor_pocisku(kp), pocisk(NULL), zycia(z), extra_pocisk(e) {}
	~MojPojazd();
	int Extra_pocisk() { return extra_pocisk; }
	void RysujPojazd(SDL_Surface* screen);
	void Skrec_w_lewo(int delta_x);
	void Skrec_w_prawo(int delta_x);
	void Jedz_do_przodu(int delta_y);
	void Jedz_do_tylu(int delta_y);
	double Sprawdz_czy_na_drodze(Droga* droga);
	bool Sprawdz_zderzenie(Pojazd* wrogi_pojazd);
	bool Sprawdz_czy_zdobyty(ExtraBron* extra_bron);
	void Wystrzel_pocisk();
	Pocisk* Zwroc_Pocisk() { return pocisk; }
	void Usun_Pocisk() { if (pocisk != NULL) pocisk->i = 0; }
	void Wprowadz_zycia() { zycia = 3; }
	void Dodaj_zycie() { zycia++; }
	void Zniszcz_pojazd() { zycia = 0; }
	int Zycia() { return zycia; }
};

class WrogiPojazd : public Pojazd
{
private:
	int delta_y;
	int delta_x;
public:
	WrogiPojazd(int k = 0, int kk = 0, int xx = 0, int yy = 0, int dy = 40, int dx = 1) : Pojazd(xx, yy, k, kk), delta_y(dy), delta_x(dx) {}
	int Delta_y() { return delta_y; }
	int Delta_x() { return delta_x; }
	void RysujPojazd(SDL_Surface* screen, Droga* droga);
	void Poruszaj_pojazdem(Droga* droga);
};

class CywilnyPojazd : public Pojazd
{
private:
	int delta_y;
	int delta_x;
public:
	CywilnyPojazd(int k = 0, int kk = 0, int xx = 0, int yy = 0, int dy = 40, int dx = 1) : Pojazd(xx, yy, k, kk), delta_y(dy), delta_x(dx) {}
	int Delta_y() { return delta_y; }
	int Delta_x() { return delta_x; }
	void RysujPojazd(SDL_Surface* screen, Droga* droga);
	void Poruszaj_pojazdem(Droga* droga);
};

class ExtraBron
{
public:
	const int width = 18;
	const int height = 30;
private:
	int x;
	int y;
	int kolor;
	int kolor_kola;
	int delta_y;
public:
	ExtraBron(int k, int kk, int xx, int yy, int dy = 30) : x(xx), y(yy), kolor(k), kolor_kola(kk), delta_y(dy) {}
	void RysujBron(SDL_Surface* screen, Droga* droga);
	int Y() { return y; }
	int X() { return x; }
	int Delta_y() { return delta_y; }
};
#endif