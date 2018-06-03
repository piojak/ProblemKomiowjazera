#pragma once

#include <vector>

class Osobnik
{
public:
	int ocena; // punkty oceny trasy
	std::vector<int> trasa; // lista z indeksami miast
	Osobnik();
	~Osobnik();
};

