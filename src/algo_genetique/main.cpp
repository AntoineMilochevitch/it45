#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <map>
#include <vector>
#include <fstream>
#include "ae.h"
#include "chromosome.h"
#include "random.h"
#include "utils.h"

using namespace std;

double calculer_gap(int z, int z_optimal) {
    return (double)(z - z_optimal) / z_optimal;
}

const char* mutationTypeToString(MutationType type) {
    switch(type) {
        case MUTATION_SWAP: return "swap";
        case MUTATION_OROPT: return "or-opt";
        case MUTATION_TABOU: return "tabou";
        default: return "inconnu";
    }
}

std::map<std::string, int> solutionsOptimales = {
	{"data/a280.tsp", 2579},
	{"data/berlin52.tsp", 7542},
	{"data/eil76.tsp", 538},
	{"data/kroA100.tsp", 21282}
};

int main(int argc, char **argv)
{
	//initialise le g�n�rateur de nombre al�atoire
	Random::randomize();

	// valeurs par defaut
	int nb_generation     = 0;
	int taille_population = 100;
	float taux_croisement = 0.8;
	float taux_mutation   = 0.2;
	int taille_chromosome = 0;
	char fileDistances[100];
	std::vector<std::string> instances = {
		"data/a280.tsp",
		"data/berlin52.tsp",
		"data/eil76.tsp",
		"data/kroA100.tsp"
	};

	int mode_arret = 1; // 1: nb générations, 2: temps
    int duree_seconde = 60;
    cout << "Mode d'arret ? (1: nb generations, 2: temps) [defaut: 1] : ";
    cin >> mode_arret;
	if (mode_arret == 1) {
		cout << "Nombre de générations ? [defaut: 1000] : ";
		cin >> nb_generation;
	} else if (mode_arret == 2) {
		cout << "Durée maximale (en secondes) ? [defaut: 60] : ";
		cin >> duree_seconde;
	}


	int choix_croisement = 1;
    bool utiliser_2opt = true;
	int choix_mutation = 1;

    cout << "Quel croisement utiliser ? (1: 1X, 2: 2X, 3: LOX) [defaut: 1] : ";
    cin >> choix_croisement;
	cout << "Quel opérateur de mutation ? (1: swap consécutif, 2: or-opt, 3: tabou) [defaut: 1] : ";
	cin >> choix_mutation;
	MutationType mutation_type = MUTATION_SWAP;
	if (choix_mutation == 2) mutation_type = MUTATION_OROPT;
	else if (choix_mutation == 3) mutation_type = MUTATION_TABOU;
    cout << "Utiliser 2-opt ? (1: oui, 0: non) [defaut: 1] : ";
    cin >> utiliser_2opt;

    CroisementType croisement_type = CROISEMENT_1X;
    if (choix_croisement == 2) croisement_type = CROISEMENT_2X;
    else if (choix_croisement == 3) croisement_type = CROISEMENT_LOX;
	

	for (const auto& instance : instances) {
		cout << "Instance : " << instance << endl;
		int taille_chromosome = 0;
		if (instance == "data/a280.tsp") taille_chromosome = 280;
		else if (instance == "data/berlin52.tsp") taille_chromosome = 52;
		else if (instance == "data/eil76.tsp") taille_chromosome = 76;
		else if (instance == "data/kroA100.tsp") taille_chromosome = 100;
	
		// Passer le fichier des distances ou des coordonnées à l'algorithme génétique
		Ae algo(nb_generation, taille_population, taux_croisement, taux_mutation, taille_chromosome, 
			const_cast<char*>(instance.c_str()), croisement_type, utiliser_2opt, mode_arret, duree_seconde);
	
		int nb_generations_effectuees = 0;
		auto start = chrono::high_resolution_clock::now();
		chromosome *best = algo.optimiser(nb_generations_effectuees);
		auto end = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		
		cout << "Nombre de g�n�rations effectuées : " << nb_generations_effectuees << endl;
		cout << "params : " << endl;
		cout << "Nombre de g�n�rations : " << nb_generation << endl;
		cout << "Taille de la population : " << taille_population << endl;
		cout << "Taux de croisement : " << taux_croisement << endl;
		cout << "Taux de mutation : " << taux_mutation << endl;
		
		cout << "croisement 2x" << endl;
		cout << "mutation : 2 gènes consectuif" << endl;
		cout << "La meilleure solution trouvee est : ";
		best->afficher(std::cout);
		cout << "Temps d'execution : " << elapsed.count() << " secondes" << endl;

		float gap = calculer_gap(best->fitness, solutionsOptimales[instance]);
		cout << "Gap : " << gap * 100 << "%" << endl;

	
		cout << "----------------------------------------" << endl;

		// Écriture CSV pour Python
		std::string croisement_str = (croisement_type == CROISEMENT_1X) ? "1x" :
                                     (croisement_type == CROISEMENT_2X) ? "2x" : "lox";
        std::string opt_str = utiliser_2opt ? "2opt" : "no2opt";
		std::string mutation_str = mutationTypeToString(mutation_type);

        std::ofstream csvFile("results/algo_genetique_results.csv", std::ios::app);
        if (csvFile.tellp() == 0) {
    		csvFile << "instance,mode_arret,nb_generation,taille_population,taux_croisement,taux_mutation,fitness,solution,temps,gap,croisement,2opt,mutation,duree_seconde,nb_generations_effectuees\n";
		}
        csvFile << instance << ","
			<< (mode_arret == 1 ? "generation" : "temps") << ","
			<< nb_generation << ","
			<< taille_population << ","
			<< taux_croisement << ","
			<< taux_mutation << ","
			<< best->fitness << ",\"";

		for (int i = 0; i < best->taille; ++i) {
			csvFile << best->genes[i];
			if (i < best->taille - 1) csvFile << "-";
		}
		
		csvFile << "\"," << elapsed.count() << "," << gap * 100 << ","
			<< croisement_str << "," << opt_str << ","
			<< mutation_str << ","
			<< duree_seconde << "," << nb_generations_effectuees << "\n";
        csvFile.close();
	}
	return 0;
}
