#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <map>
#include <vector>
#include "ae.h"
#include "chromosome.h"
#include "random.h"
#include "utils.h"

using namespace std;

double calculer_gap(int z, int z_optimal) {
    return (double)(z - z_optimal) / z_optimal;
}

std::map<std::string, int> solutionsOptimales = {
	{"data/a280.tsp", 2579},
	{"data/berlin52.tsp", 7542},
	{"data/eil76.tsp", 538},
	{"data/kroA100.tsp", 21282}
};

int main(int argc, char **argv)
// argc : nombre de parametres
// argv : tableau contenant les parametres
// Soit l'executable 'algo_evo' ne prend pas d'arguments soit il prend 6 arguments :
//    1. nombre de g�n�ration (entier > 0)
//    2. taille de la population (entier > 0)
//    3. taux de croisement (0 <= reel <= 1)
//    4. taux de mutation (0 <= reel <= 1)
//    5. nombre de villes (=taille d'un chromosome)
//    6. nom du fichier indiquant les distances entre villes
{
	//initialise le g�n�rateur de nombre al�atoire
	Random::randomize();

	// valeurs par defaut
	int nb_generation     = 100;
	int taille_population = 100;
	float taux_croisement = 0.8;
	float taux_mutation   = 0.3;
	int taille_chromosome = 10;
	char fileDistances[100];
	std::vector<std::string> instances = {
		"data/a280.tsp",
		"data/berlin52.tsp",
		"data/eil76.tsp",
		"data/kroA100.tsp"
	};
	

	for (const auto& instance : instances) {
		cout << "Instance : " << instance << endl;
	
		// Passer le fichier des distances ou des coordonnées à l'algorithme génétique
		Ae algo(nb_generation, taille_population, taux_croisement, taux_mutation, taille_chromosome, const_cast<char*>(instance.c_str()));
	
		auto start = chrono::high_resolution_clock::now();
		chromosome *best = algo.optimiser();
		auto end = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
	
		cout << "La meilleure solution trouvee est : ";
		best->afficher();
		cout << "Temps d'execution : " << elapsed.count() << " secondes" << endl;
	
		delete best;
		cout << "----------------------------------------" << endl;
	}
	return 0;
}
