#include <iostream>
#include <time.h>
#include <windows.h>
#include <io.h>
#include "pojazdy.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void Nowa_gra(double* wynik, double* worldTime, Droga** droga, MojPojazd* moj_pojazd, WrogiPojazd** wrogi_pojazd, 
	CywilnyPojazd** cywilny_pojazd, ExtraBron** extra_bron, int kolor_czarny, int kolor_czerwony,
	int ciemny_szary, int kolor_zolty, SDL_Surface* screen);
int Zatrzymaj_rozgrywke(SDL_Event* event, int czerwony, int niebieski, SDL_Surface* screen,
	SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer);
char Koniec_gry(SDL_Event* event, int czerwony, int niebieski, SDL_Surface* screen,
	SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer, double wynik, double czas);
int Zapisz_gre(double* wynik, double* worldTime, Droga** droga, MojPojazd* moj_pojazd, WrogiPojazd** wrogi_pojazd, CywilnyPojazd** cywilny_pojazd,
	ExtraBron** extra_bron, int czerwony, int niebieski, SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer);

int main(int argc, char** argv) {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, wynik;
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Spy Hunter, Hubert Malinowski");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	char text[128], text2[128], text3[128], text4[128];
	sprintf(text3, "strzalki - poruszanie, esc - zamkniecie programu, n - nowa gra");
	sprintf(text4, "p - pauza, s - zapisz stan gry, l - wczytaj gre, f - zakoncz gre");

	const int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	const int ciemny_szary = SDL_MapRGB(screen->format, 0x33, 0x33, 0x33);
	const int zielony = SDL_MapRGB(screen->format, 0x32, 0x66, 0x47);
	const int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	const int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	const int zolty = SDL_MapRGB(screen->format, 0xE3, 0xA3, 0x29);

	t1 = SDL_GetTicks();
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;

	Droga* droga = NULL;
	MojPojazd moj_pojazd;
	WrogiPojazd* wrogi_pojazd = NULL;
	CywilnyPojazd* cywilny_pojazd = NULL;
	ExtraBron* extra_bron = NULL;
	Nowa_gra(&wynik, &worldTime, &droga, &moj_pojazd, &wrogi_pojazd, &cywilny_pojazd, &extra_bron, czarny, czerwony, ciemny_szary, zolty, screen);
	int stary_wynik = wynik;
	while (!quit) {
		t2 = SDL_GetTicks();

		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		if (worldTime > 5 && worldTime < 6) moj_pojazd.Wprowadz_zycia();

		if ((int)wynik % 100 > 0 && (int)wynik % 100 < 51 && wynik > stary_wynik) {
			moj_pojazd.Dodaj_zycie();
			stary_wynik = wynik - ((int)wynik % 100) + 60;
		}

		SDL_FillRect(screen, NULL, zielony);

		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		char zycia[20];
		if (moj_pojazd.Zycia() > 990) strcpy(zycia, "bez limitu");
		else sprintf(zycia, "%i", moj_pojazd.Zycia());
		
		// tekst informacyjny 
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "Hubert Malinowski, 193088, czas trwania = %.1lf s,  %.0lf klatek / s", worldTime, fps);
		sprintf(text2, "wynik = %.0f, zycia = %s", wynik, zycia);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		DrawString(screen, screen->w / 2 - strlen(text2) * 8 / 2, 25, text2, charset);

		if (droga->Rysuj_droge() == false) {
			Droga* temp = droga;
			droga = droga->Next();
			delete(temp);
		}
		moj_pojazd.RysujPojazd(screen);

		if (extra_bron == NULL && ((int)worldTime + 3) % 20 == 0)
			extra_bron = new ExtraBron(czerwony, czarny, droga->X() + rand() % droga->Szerokosc(), 41);
		else if (extra_bron != NULL && extra_bron->Y() >= SCREEN_HEIGHT) {
			delete(extra_bron);
			extra_bron = NULL;
		}
		else if (extra_bron != NULL) {
			if (moj_pojazd.Sprawdz_czy_zdobyty(extra_bron)) {
				delete(extra_bron);
				extra_bron = NULL;
			}
			else
				extra_bron->RysujBron(screen, droga);
		}

		if (wrogi_pojazd == NULL && (cywilny_pojazd == NULL || cywilny_pojazd->Y() > SCREEN_HEIGHT / 2) && (((int)worldTime + 3) % 10 == 0))
			wrogi_pojazd = new WrogiPojazd(czarny, zolty, droga->X(), 41);
		else if (wrogi_pojazd != NULL && wrogi_pojazd->Y() >= SCREEN_HEIGHT) {
			delete(wrogi_pojazd);
			wrogi_pojazd = NULL;
		}
		else if (wrogi_pojazd != NULL) {
			if (moj_pojazd.Zwroc_Pocisk() != NULL && moj_pojazd.Zwroc_Pocisk()->Sprawdz_czy_trafil(wrogi_pojazd)) {
					delete(wrogi_pojazd);
					wrogi_pojazd = NULL;
					moj_pojazd.Usun_Pocisk();
					wynik += 50;
			}
			else
				wrogi_pojazd->RysujPojazd(screen, droga);
		}

		if (cywilny_pojazd == NULL && (wrogi_pojazd == NULL || wrogi_pojazd->Y() > SCREEN_HEIGHT / 2) && (((int)worldTime + 8) % 10 == 0))
			cywilny_pojazd = new CywilnyPojazd(niebieski, czarny, droga->X(), 41);
		else if (cywilny_pojazd != NULL && cywilny_pojazd->Y() >= SCREEN_HEIGHT) {
			delete(cywilny_pojazd);
			cywilny_pojazd = NULL;
		}
		else if (cywilny_pojazd != NULL) {
			if (moj_pojazd.Zwroc_Pocisk() != NULL && moj_pojazd.Zwroc_Pocisk()->Sprawdz_czy_trafil(cywilny_pojazd)) {
					delete(cywilny_pojazd);
					cywilny_pojazd = NULL;
					moj_pojazd.Usun_Pocisk();
					wynik -= 50;
			}
			else
				cywilny_pojazd->RysujPojazd(screen, droga);
		}

		if (moj_pojazd.Sprawdz_czy_na_drodze(droga) == 10) moj_pojazd.Zniszcz_pojazd();
		else wynik += moj_pojazd.Sprawdz_czy_na_drodze(droga);

		DrawRectangle(screen, 4, SCREEN_HEIGHT - 40, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		DrawString(screen, screen->w / 2 - strlen(text3) * 8 / 2, SCREEN_HEIGHT - 36, text3, charset);
		DrawString(screen, screen->w / 2 - strlen(text4) * 8 / 2, SCREEN_HEIGHT - 18, text4, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (wrogi_pojazd != NULL && moj_pojazd.Sprawdz_zderzenie(wrogi_pojazd)) {
			wynik -= 10;
			delete(wrogi_pojazd);
			wrogi_pojazd = NULL;
		}
		if (cywilny_pojazd != NULL && moj_pojazd.Sprawdz_zderzenie(cywilny_pojazd)) {
			wynik -= 50;
			delete(cywilny_pojazd);
			cywilny_pojazd = NULL;
		}

		if (wynik < 0) wynik = 0;
		if (moj_pojazd.Zycia() <= 0) {
			char zn = Koniec_gry(&event, czerwony, niebieski, screen, charset, scrtex, renderer, wynik, worldTime);
			if (zn == 'n') {
				Nowa_gra(&wynik, &worldTime, &droga, &moj_pojazd, &wrogi_pojazd, &cywilny_pojazd, &extra_bron, czarny, czerwony, ciemny_szary, zolty, screen);
				stary_wynik = wynik;
			}
			else quit = 1;
		}
		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_RIGHT) moj_pojazd.Skrec_w_prawo(10);
				else if (event.key.keysym.sym == SDLK_LEFT) moj_pojazd.Skrec_w_lewo(10);
				else if (event.key.keysym.sym == SDLK_UP) moj_pojazd.Jedz_do_przodu(10);
				else if (event.key.keysym.sym == SDLK_DOWN) moj_pojazd.Jedz_do_tylu(10);
				else if (event.key.keysym.sym == SDLK_SPACE) moj_pojazd.Wystrzel_pocisk();
				else if (event.key.keysym.sym == 'n') {
					Nowa_gra(&wynik,&worldTime, &droga, &moj_pojazd, &wrogi_pojazd, &cywilny_pojazd, &extra_bron, czarny, czerwony, ciemny_szary, zolty, screen);
					stary_wynik = wynik;
				}
				else if (event.key.keysym.sym == 'p') 
					t1 = Zatrzymaj_rozgrywke(&event, czerwony, niebieski, screen, charset, scrtex, renderer);
				else if (event.key.keysym.sym == 'f') {
					char zn = Koniec_gry(&event, czerwony, niebieski, screen, charset, scrtex, renderer, wynik, worldTime);
					if (zn == 'n') {
						Nowa_gra(&wynik, &worldTime, &droga, &moj_pojazd, &wrogi_pojazd, &cywilny_pojazd, &extra_bron, czarny, czerwony, ciemny_szary, zolty, screen);
						stary_wynik = wynik;
					}
					else quit = 1;
				}
				else if (event.key.keysym.sym == 's')
					t1 = Zapisz_gre(&wynik, &worldTime, &droga, &moj_pojazd, &wrogi_pojazd, &cywilny_pojazd, &extra_bron, czerwony, niebieski, screen, charset, scrtex, renderer);
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		frames++;
	};

	// zwolnienie powierzchni 
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
};

void Nowa_gra(double* wynik,double* worldTime, Droga** droga, MojPojazd* moj_pojazd, WrogiPojazd** wrogi_pojazd,
	CywilnyPojazd** cywilny_pojazd, ExtraBron** extra_bron, int kolor_czarny, int kolor_czerwony, int ciemny_szary, int kolor_zolty, SDL_Surface* screen) {
	*wynik = 0;
	*worldTime = 0;
	if (moj_pojazd != NULL) moj_pojazd->~MojPojazd();
	*moj_pojazd = MojPojazd(kolor_czerwony, kolor_czarny, kolor_zolty);
	if (*wrogi_pojazd != NULL) {
		delete(*wrogi_pojazd);
		*wrogi_pojazd = NULL;
	} 
	if (*cywilny_pojazd != NULL) {
		delete(*cywilny_pojazd);
		*cywilny_pojazd = NULL;
	}
	if (*extra_bron != NULL) {
		delete(*extra_bron);
		*extra_bron = NULL;
	}
	while (*droga != NULL) {
		Droga* temp = *droga;
		*droga = (*droga)->Next();
		delete(temp);
	}
	Droga* temp = new Droga(SCREEN_WIDTH * 3 / 8, 41, SCREEN_WIDTH / 4, SCREEN_HEIGHT - 41, ciemny_szary, screen);
	*droga = temp;
}
int Zatrzymaj_rozgrywke(SDL_Event* event, int czerwony, int niebieski, SDL_Surface* screen,
	SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer) {
	char text[128];
	sprintf(text, "Wcisnij klawisz 'p', aby wznowic rozgrywke");
	DrawRectangle(screen, 40, (SCREEN_HEIGHT - 50) / 2, SCREEN_WIDTH - 80, 50, czerwony, niebieski);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, (SCREEN_HEIGHT - 25) / 2, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	while (SDL_PollEvent(event) != 1 || event->type != SDL_KEYDOWN || event->key.keysym.sym != 'p') {}
	int t = SDL_GetTicks();
	return t;
}

char Koniec_gry(SDL_Event* event, int czerwony, int niebieski, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, double wynik, double czas) {
	char text[128], text2[128], text3[128], text4[128];
	sprintf(text, "Gra skonczona");
	sprintf(text2, "Twoj wynik to %i punktow,", long(wynik) + 1);
	long minuty = czas / 60;
	long sekundy = (long)czas % 60;
	sprintf(text3, "a czas %i minut i %i sekund,", (long)minuty, (long)sekundy);
	sprintf(text4, "wcisnij ESC, aby zakonczyc, lub n, aby rozpoczac gre od nowa");
	DrawRectangle(screen, 40, (SCREEN_HEIGHT - 56) / 2, SCREEN_WIDTH - 80, 56, czerwony, niebieski);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, (SCREEN_HEIGHT - 48) / 2, text, charset);
	DrawString(screen, screen->w / 2 - strlen(text2) * 8 / 2, (SCREEN_HEIGHT - 24) / 2, text2, charset);
	DrawString(screen, screen->w / 2 - strlen(text3) * 8 / 2, (SCREEN_HEIGHT + 0) / 2, text3, charset);
	DrawString(screen, screen->w / 2 - strlen(text4) * 8 / 2, (SCREEN_HEIGHT + 24) / 2, text4, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	while (1) {
		if (SDL_PollEvent(event) == 1 && event->type == SDL_KEYDOWN && event->key.keysym.sym == 'n')  return 'n';
		else if (SDL_PollEvent(event) == 1 && event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)  return 'f'; 
	}
}

int Zapisz_gre(double* wynik, double* worldTime, Droga** droga, MojPojazd* moj_pojazd, WrogiPojazd** wrogi_pojazd, CywilnyPojazd** cywilny_pojazd, 
	ExtraBron** extra_bron, int czerwony, int niebieski, SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer){
	char nazwa_pliku[50];
	time_t rawtime;
	tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(nazwa_pliku, 50, "zapisane/%F-%H-%M-%S.txt", timeinfo);
	FILE* plik;
	plik = fopen(nazwa_pliku, "w");
	//wynik
	char wynik_plik[20];
	int wynik_size = sprintf_s(wynik_plik, "%f ", *wynik);
	fwrite(wynik_plik, sizeof(char), wynik_size, plik);
	//czas
	char worldTime_plik[20];
	int worldTime_size = sprintf_s(worldTime_plik, "%f ", *worldTime);
	fwrite(worldTime_plik, sizeof(char), worldTime_size, plik);
	//droga
	char droga_plik[50];
	int droga_size = sprintf_s(droga_plik, "%d %d %d %d %d %d ", (*droga)->X(), (*droga)->Y(), (*droga)->Szerokosc(), (*droga)->Dlugosc(), (*droga)->I(), (*droga)->J());
	fwrite(droga_plik, sizeof(char), droga_size, plik);
	//droga->next
	if ((*droga)->Next() == NULL) fwrite("NULL", sizeof(char), sizeof(char) * 5, plik);
	else {
		char droga_plik[50];
		int droga_size = sprintf_s(droga_plik, "%d %d %d %d %d %d ", (*droga)->Next()->X(), (*droga)->Next()->Y(),
			(*droga)->Next()->Szerokosc(), (*droga)->Next()->Dlugosc(), (*droga)->Next()->I(), (*droga)->Next()->J());
		fwrite(droga_plik, sizeof(char), droga_size, plik);
	}
	// moj pojazd
	char moj_pojazd_plik[50];
	int moj_pojazd_size = sprintf_s(moj_pojazd_plik, "%d %d %d %d ", moj_pojazd->X(), moj_pojazd->Y(), moj_pojazd->Zycia(), moj_pojazd->Extra_pocisk());
	fwrite(moj_pojazd_plik, sizeof(char), moj_pojazd_size, plik);
	//wrogi pojazd
	if (*wrogi_pojazd == NULL) fwrite("NULL", sizeof(char), sizeof(char) * 5, plik);
	else {
		char wrogi_pojazd_plik[50];
		int wrogi_pojazd_size = sprintf_s(wrogi_pojazd_plik, "%d %d %d %d ", (*wrogi_pojazd)->X(), (*wrogi_pojazd)->Y(), (*wrogi_pojazd)->Delta_x(), (*wrogi_pojazd)->Delta_y());
		fwrite(wrogi_pojazd_plik, sizeof(char), wrogi_pojazd_size, plik);
	}
	//cywilny pojazd
	if (*cywilny_pojazd == NULL) fwrite("NULL", sizeof(char), sizeof(char) * 5, plik);
	else {
		char cywilny_pojazd_plik[50];
		int cywilny_pojazd_size = sprintf_s(cywilny_pojazd_plik, "%d %d %d %d ", (*cywilny_pojazd)->X(), (*cywilny_pojazd)->Y(), (*cywilny_pojazd)->Delta_x(), (*cywilny_pojazd)->Delta_y());
		fwrite(cywilny_pojazd_plik, sizeof(char), cywilny_pojazd_size, plik);
	}
	//extra bron
	if (*extra_bron == NULL) fwrite("NULL", sizeof(char), sizeof(char) * 5, plik);
	else {
		char extra_bron_plik[50];
		int extra_bron_size = sprintf_s(extra_bron_plik, "%d %d %d ", (*extra_bron)->X(), (*extra_bron)->Y(), (*extra_bron)->Delta_y());
		fwrite(extra_bron_plik, sizeof(char), extra_bron_size, plik);
	}
	fclose(plik);
	char text[128];
	sprintf(text, "Gra pomyslnie zapisana");
	DrawRectangle(screen, 40, (SCREEN_HEIGHT - 30) / 2, SCREEN_WIDTH - 80, 30, czerwony, niebieski);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, (SCREEN_HEIGHT - 15) / 2, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	Sleep(1500);
	int t = SDL_GetTicks();
	return t;
}