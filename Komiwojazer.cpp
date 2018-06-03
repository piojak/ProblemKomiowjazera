#include "Komiwojazer.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;


Komiwojazer::Komiwojazer()
{
	srand(time(NULL));
}


Komiwojazer::~Komiwojazer()
{
	for (osobnik_mapa::iterator it = this->populacja.begin(); it != this->populacja.end(); ++it)
	{
		delete it->second;
	}
}


void Komiwojazer::Start(int populacja, int selekcja, int pk, int pm, int pokolenia)
{
	
	for (int i = 0; i < this->miasta.size(); ++i)
	{
		this->indeksy.push_back(i);
	}

	
	this->LosujPopulacje(populacja);

	for (int i = 0; i < pokolenia; ++i) 
	{
		vector<osobnik_para> dzieciLista; 	// pomocnicza lista z potomkami powstalymi w krzyzowaniu

		// iterujemy po wszystkich osobnikach w populacji
		for (osobnik_mapa::iterator it = this->populacja.begin(); it != this->populacja.end(); ++it)
		{
			Osobnik *os1 = it->second;
			int p = rand() % 100 + 1;
			// jesli wylosujemy liczbe mniejsza niz parametr "krzyzowanie"
			if (p <= pk)
			{
				osobnik_mapa::iterator it2;
				do
				{
					int los = rand() % this->populacja.size();
					it2 = this->populacja.begin();
					advance(it2, los);
				} while (it == it2); // nie moze byc ten sam!

				dzieciLista.push_back(this->Krzyzuj(*os1, *(it2->second)));
			}

			if (p <= pm) // jesli wylosujemy liczbe mniejsza niz parametr "mutacja"
			{
				this->Mutuj(*os1); // zmutuj osobnika
			}
		}

		for (int j = 0; j < dzieciLista.size(); ++j) // wstawienie dzieci do glownej populacji
		{
			osobnik_para dzieci = dzieciLista.at(j);
			this->populacja.insert(mapa_elem(dzieci.first->ocena, dzieci.first));
			this->populacja.insert(mapa_elem(dzieci.second->ocena, dzieci.second));
		}

		// jesli w populacji jest wiecej osobnikow niz przewiduje selekcja
		if (this->populacja.size() > selekcja)
		{
			int count = this->populacja.size() - selekcja;
			osobnik_mapa::iterator it = this->populacja.begin();
			advance(it, selekcja); // przesuniecie pozycje na mapie na pozycje rowna parametru "selekcja"
			while (count--)
			{
				delete it->second;
				it = this->populacja.erase(it);
			}
		}

	}
}

void Komiwojazer::LosujPopulacje(int n) // losowanie poczatkowej populacji
{
	for (int i = 0; i < n; ++i) 
	{
		this->LosujIndeksy(); // przemieszanie indeksow miast
		Osobnik* o = new Osobnik();
		for (int j = 0; j < this->miasta.size(); ++j)
		{
			o->trasa.push_back(this->indeksy.at(j)); // tworzenie trasy
		}
		this->Oblicz(*o); // ocena trasy
		this->populacja.insert(mapa_elem(o->ocena, o));
	}
}

void Komiwojazer::LosujIndeksy()
{
	random_shuffle(this->indeksy.begin(), this->indeksy.end()); // przemieszanie tablicy z indeksami miast
}

Osobnik* Komiwojazer::Najlepszy() // najlepszy osobnik
{
	if (this->populacja.size() == 0)
	{
		return NULL;
	}
	return this->populacja.begin()->second; // osobnik
}

// operator krzyzowania osobnikow
osobnik_para Komiwojazer::Krzyzuj(Osobnik& r1, Osobnik& r2)
{

	Osobnik* dz1 = new Osobnik();
	Osobnik* dz2 = new Osobnik();
	// bierzemy polowe trasy od rodzica
	for (int i = 0; i < this->miasta.size() / 2; ++i)
	{
		dz1->trasa.push_back(r2.trasa.at(i));
		dz2->trasa.push_back(r1.trasa.at(i));
	}

	// bierzemy druga polowe trasy od drugiego rodzica 
	while (dz1->trasa.size() != this->miasta.size() && dz2->trasa.size() != this->miasta.size())
	{
		for (int i = 0; i < this->miasta.size(); ++i)
		{
			if (find(dz1->trasa.begin(), dz1->trasa.end(), r1.trasa.at(i)) == dz1->trasa.end()) // (z uwzglednieniem ze numery miast sie nie moga powtarzac)
			{
				dz1->trasa.push_back(r1.trasa.at(i));
			}
			if (find(dz2->trasa.begin(), dz2->trasa.end(), r2.trasa.at(i)) == dz2->trasa.end())
			{
				dz2->trasa.push_back(r2.trasa.at(i));
			}
		}
	}
	this->Oblicz(*dz1);
	this->Oblicz(*dz2);
	return make_pair(dz1, dz2); // zwroc oboje dzieci
}

// mutacja osobnika
void Komiwojazer::Mutuj(Osobnik& o)
{
	this->LosujIndeksy(); // przemieszanie indeksow
	swap(o.trasa[this->indeksy.at(0)], o.trasa[this->indeksy.at(1)]); // podmieniamy dwa miasta ze soba
}

// ocena trasy
void Komiwojazer::Oblicz(Osobnik& o)
{
	int ocena = 0;
	for (int i = 0; i < o.trasa.size() - 1; ++i) // petla po wszystkich miastach
	{
		Punkt* a = this->miasta[o.trasa.at(i)];
		Punkt* b = this->miasta[o.trasa.at(i + 1)];
		ocena += this ->Dystans(*a, *b); // odleglosc euklidesowa pomiedzy dwoma punktami
	}
	o.ocena = ocena;
}

// prosty wzor na odleglosc euklidesowa pomiedzy dwoma punktami
int Komiwojazer::Dystans(const Punkt& a, const Punkt& b) const
{
	return (int)(round(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2))) + 0.51);
}