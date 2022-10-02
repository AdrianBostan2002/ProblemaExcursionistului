#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <queue>
#include <map>

class Node
{
public:
	Node* parent;
	int cost;
	int distanta_manhattan;
	int i, j;
	std::string pozitie;
};

void citeste_harta(std::vector<std::vector<int>>& harta)
{
	int lungime_harta, latime_harta;
	std::vector<int>lungime;
	int relief;
	std::ifstream fin;
	fin.open("input.in");
	fin >> latime_harta>>lungime_harta;
	for (int i = 0; i < latime_harta; i++)
	{
		for (int j = 0; j < lungime_harta; j++)
		{
			fin >> relief;
			lungime.push_back(relief);
		}
		harta.push_back(lungime);
		lungime.clear();
	}
	fin.close();
}

void afisare_harta(std::vector<std::vector<int>>harta)
{
	for (int i = 0; i < harta.size(); i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < harta[i].size(); j++)
		{
			std::cout << harta[i][j] << " ";
		}
	}
}

void citire_locatie_de_pornire_si_destinatie(std::pair<int, int>& A, std::pair<int, int>& B, std::vector<std::vector<int>>harta)
{
	std::cin >> A.first>>A.second;
	std::cin >> B.first >> B.second;
	if (!(A.first >= 0 && A.first <= harta.size()-1))
	{
		exit(-1);
	}
	if (!(B.first >= 0 && B.first <= harta.size()-1))
	{
		exit(-1);
	}
	if (!(A.second >= 0 && A.second <= harta[0].size()-1))
	{
		exit(-1);
	}
	if (!(B.second >= 0 && B.second <= harta[0].size()-1))
	{
		exit(-1);
	}
}

int distanta_manhattan(int ai, int aj, std::pair<int, int>B)
{
	return abs(ai - B.first) + abs(aj - B.second);
}

struct comparator
{
	bool operator()(Node* a, Node* b)
	{
		return (a->cost - a->distanta_manhattan) < (b->cost- b->distanta_manhattan);
	}
};

Node* creare_nod_nou(Node* pozitie_actuala, int i, int j, std::vector<std::vector<int>>harta, std::pair<int, int>B)
{
	Node* nod_nou = new Node;
	nod_nou->parent = pozitie_actuala;
	nod_nou->i = i;
	nod_nou->j = j;
	nod_nou->cost = pozitie_actuala->cost + (harta[pozitie_actuala->i][pozitie_actuala->j] - harta[i][j]) + 1;
	nod_nou->distanta_manhattan = distanta_manhattan(i, j, B);

	return nod_nou;
}

std::map<std::pair<int, int>, Node*> determinare_cel_mai_optim_drum_pana_la_destinatie(std::pair<int, int>A, std::pair<int, int>B, std::vector<std::vector<int>>harta)
{
	std::map<std::pair<int, int>, Node*>neduplicare;
	std::map<std::pair<int, int>, Node*>::iterator it;
	std::priority_queue<Node*, std::vector<Node*>, comparator>coada_de_prioritate_noduri;
	Node* pozitie_actuala = new Node;
	pozitie_actuala->parent = nullptr;
	pozitie_actuala->i = A.first;
	pozitie_actuala->j = A.second;
	pozitie_actuala->cost = 0;
	pozitie_actuala->distanta_manhattan = distanta_manhattan(pozitie_actuala->i, pozitie_actuala->j, B);
	neduplicare[A] = pozitie_actuala;
	coada_de_prioritate_noduri.push(pozitie_actuala);
	while (coada_de_prioritate_noduri.size() != 0)
	{
		pozitie_actuala = coada_de_prioritate_noduri.top();
		coada_de_prioritate_noduri.pop();

		if (pozitie_actuala->j + 1 < harta[0].size() && abs(harta[pozitie_actuala->i][pozitie_actuala->j] - harta[pozitie_actuala->i][pozitie_actuala->j + 1]) <= 2 && harta[pozitie_actuala->i][pozitie_actuala->j + 1] >= -2)
		{
			it = neduplicare.find(std::make_pair(pozitie_actuala->i, pozitie_actuala->j + 1));
			if (it == neduplicare.end())
			{
				Node* copil_nou = creare_nod_nou(pozitie_actuala, pozitie_actuala->i, pozitie_actuala->j + 1, harta, B);
				neduplicare[std::make_pair(pozitie_actuala->i, pozitie_actuala->j + 1)]= copil_nou;
				coada_de_prioritate_noduri.push(copil_nou);
				if (coada_de_prioritate_noduri.top()->i == B.first && coada_de_prioritate_noduri.top()->j == B.second)
				{
					break;
				}
			}
		}

		if (pozitie_actuala->j - 1 >= 0 && abs(harta[pozitie_actuala->i][pozitie_actuala->j] - harta[pozitie_actuala->i][pozitie_actuala->j - 1]) <= 2 && harta[pozitie_actuala->i][pozitie_actuala->j - 1] >= -2)
		{
			it = neduplicare.find(std::make_pair(pozitie_actuala->i, pozitie_actuala->j - 1));
			if (it == neduplicare.end())
			{
				Node* copil_nou = creare_nod_nou(pozitie_actuala, pozitie_actuala->i, pozitie_actuala->j-1, harta, B);
				neduplicare[std::make_pair(pozitie_actuala->i, pozitie_actuala->j - 1)]=copil_nou;
				coada_de_prioritate_noduri.push(copil_nou);
				if (coada_de_prioritate_noduri.top()->i == B.first && coada_de_prioritate_noduri.top()->j == B.second)
				{
					break;
				}
			}
		}

		if (pozitie_actuala->i - 1 >= 0 && abs(harta[pozitie_actuala->i][pozitie_actuala->j] - harta[pozitie_actuala->i - 1][pozitie_actuala->j]) <= 2 && harta[pozitie_actuala->i - 1][pozitie_actuala->j] >= -2)
		{
			it = neduplicare.find(std::make_pair(pozitie_actuala->i - 1, pozitie_actuala->j));
			if (it == neduplicare.end())
			{
				Node* copil_nou = creare_nod_nou(pozitie_actuala, pozitie_actuala->i-1, pozitie_actuala->j, harta, B);
				neduplicare[std::make_pair(pozitie_actuala->i-1, pozitie_actuala->j)] = copil_nou;
				coada_de_prioritate_noduri.push(copil_nou);
				if (coada_de_prioritate_noduri.top()->i == B.first && coada_de_prioritate_noduri.top()->j == B.second)
				{
					break;
				}
			}
		}

		if (pozitie_actuala->i + 1 < harta.size() && abs(harta[pozitie_actuala->i][pozitie_actuala->j] - harta[pozitie_actuala->i + 1][pozitie_actuala->j]) <= 2 && harta[pozitie_actuala->i + 1][pozitie_actuala->j] >= -2)
		{
			it = neduplicare.find(std::make_pair(pozitie_actuala->i + 1, pozitie_actuala->j));
			if (it == neduplicare.end())
			{
				Node* copil_nou = creare_nod_nou(pozitie_actuala, pozitie_actuala->i+1, pozitie_actuala->j, harta, B);
				neduplicare[std::make_pair(pozitie_actuala->i+1, pozitie_actuala->j)] = copil_nou;
				coada_de_prioritate_noduri.push(copil_nou);
				if (coada_de_prioritate_noduri.top()->i == B.first && coada_de_prioritate_noduri.top()->j == B.second)
				{
					break;
				}
			}
		}
	}

	std::map<std::pair<int, int>, Node*> drumul_cel_mai_optim;
	if (coada_de_prioritate_noduri.size() != 0)
	{
		Node* parinte = coada_de_prioritate_noduri.top();
		while (parinte != nullptr)
		{
			drumul_cel_mai_optim[std::make_pair(parinte->i, parinte->j)]= parinte;
			parinte = parinte->parent;
		}
	}
	else
	{
		std::cout << "Elementul nu a fost gasit";
		exit(1);
	}

	return drumul_cel_mai_optim;
}

void afisare_drumul_cel_mai_optim(std::map<std::pair<int, int>, Node*> drumul_cel_mai_optim, std::vector<std::vector<int>>harta)
{
	std::map<std::pair<int, int>, Node*>::iterator it;
	for (int i = 0; i < harta.size(); i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < harta[i].size(); j++)
		{
			it=drumul_cel_mai_optim.find(std::make_pair(i, j));
			if (it != drumul_cel_mai_optim.end())
			{
				std::cout << "  " << "X";
			}
			else
			{
				if (harta[i][j] >= 0)
				{
					std::cout << "  "<<harta[i][j];
				}
				else
				{
					std::cout << " " << harta[i][j];
				}
			}
		}
	}
}

int main()
{
	std::vector<std::vector<int>>harta;
	std::map<std::pair<int, int>, Node*>drumul_cel_mai_optim;
	citeste_harta(harta);
	std::pair<int, int>A;
	std::pair<int, int>B;
	citire_locatie_de_pornire_si_destinatie(A, B, harta);
	drumul_cel_mai_optim=determinare_cel_mai_optim_drum_pana_la_destinatie(A, B, harta);
	afisare_drumul_cel_mai_optim(drumul_cel_mai_optim, harta);

	return 0;
}
