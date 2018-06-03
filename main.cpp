// TSP_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Punkt.h"
#include "Komiwojazer.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	Komiwojazer* komiwojazer = new Komiwojazer();

	ifstream plik("TSP20.txt");
	if (plik.is_open())
	{
		string line;
		int ln = 0;
		while (getline(plik, line))
		{
			++ln;
			if (ln < 3) continue;
			
			int x, y;
			istringstream iss(line);
			iss >> x >> y;

			Punkt* p = new Punkt(x, y);

			komiwojazer->miasta.push_back(p);
		}
	}
	

	int populacja_poczatkowa = 1000; 
	int selekcja = 600; 
	int mutacja_p = 40; 
	int krzyzowanie_p = 80; 
	int pokolenia = 20; 


	
	komiwojazer->Start(populacja_poczatkowa, selekcja, krzyzowanie_p, mutacja_p, pokolenia);
	if (komiwojazer->Najlepszy() == NULL)
	{
		cout << "Brak wyników" << '\n';
	}
	else
	{
		cout << "Najlepszy wynik:\t" << komiwojazer->Najlepszy()->ocena << '\n';
		for (int i = 0; i < komiwojazer->Najlepszy()->trasa.size(); ++i)
		{
			cout << komiwojazer->Najlepszy()->trasa.at(i) << ' ';
		
		} 
		cout << endl;
	}
	
	delete komiwojazer;
	system("pause");
	return 0;
}

