#pragma once

#include <vector>
#include <map>
#include "Punkt.h"
#include "Osobnik.h"

typedef std::pair<Osobnik*, Osobnik*> osobnik_para;
typedef std::multimap<int, Osobnik*> osobnik_mapa;
typedef std::pair<int, Osobnik*> mapa_elem;

class Komiwojazer
{
private:
	void Mutuj(Osobnik&); // mutacja osobnika
	void Oblicz(Osobnik&); // oblicz koszt trasy pomiedzy wszystkimi punktami
	osobnik_para Krzyzuj(Osobnik&, Osobnik&); // krzyzowanie dwoch osobnikow i powstaje dwoje dzieci
	int Dystans(const Punkt& a, const Punkt& b) const; // odleglosc pomiedzy miastami

	std::vector<int> indeksy; // pomocnicza tablica z indeksami miast (potrzebne do losowania indeksow miast)
	void LosujIndeksy(); // wymieszanie tablicy z indeksami
	void LosujPopulacje(int); // losowanie poczatkowej populacji
public:
	void Start(int populacja, int selekcja, int krzyzowanie, int mutacja, int pokolenia);
	osobnik_mapa populacja; // lista populacji
	std::vector<Punkt*> miasta; // lista punktow miast
	Osobnik* Najlepszy(); // najlepszy osobnik (najlepsza trasa)
	Komiwojazer();
	~Komiwojazer();
};

