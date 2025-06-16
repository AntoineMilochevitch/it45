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

	// Ouvrir le fichier pour écrire les résultats
    std::ofstream outputFile("results/algo_génétique.txt", std::ios::app);
    if (!outputFile.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier algo_génétique.txt" << endl;
        return 1;
    }

	// valeurs par defaut
	int nb_generation     = 600;
	int taille_population = 100;
	float taux_croisement = 0.8;
	float taux_mutation   = 0.5;
	int taille_chromosome = 0;
	char fileDistances[100];
	std::vector<std::string> instances = {
		"data/a280.tsp",
		"data/berlin52.tsp",
		"data/eil76.tsp",
		"data/kroA100.tsp"
	};

	int choix_croisement = 1;
    bool utiliser_2opt = true;
    cout << "Quel croisement utiliser ? (1: 1X, 2: 2X, 3: LOX) [defaut: 1] : ";
    cin >> choix_croisement;
    cout << "Utiliser 2-opt ? (1: oui, 0: non) [defaut: 1] : ";
    cin >> utiliser_2opt;

    CroisementType croisement_type = CROISEMENT_2X;
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
		Ae algo(nb_generation, taille_population, taux_croisement, taux_mutation, taille_chromosome, const_cast<char*>(instance.c_str()), croisement_type, utiliser_2opt);
	
		auto start = chrono::high_resolution_clock::now();
		chromosome *best = algo.optimiser();
		auto end = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;

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

		// Écriture dans le fichier uniquement à la fin de chaque instance
        outputFile << "Instance : " << instance << endl;
        outputFile << "Nombre de générations : " << nb_generation << endl;
        outputFile << "Taille de la population : " << taille_population << endl;
        outputFile << "Taux de croisement : " << taux_croisement << endl;
        outputFile << "Taux de mutation : " << taux_mutation << endl;
        outputFile << "La meilleure solution trouvee est : ";
        best->afficher(outputFile); // Il faut surcharger afficher(std::ostream&) si ce n'est pas déjà fait
        outputFile << "Temps d'execution : " << elapsed.count() << " secondes" << endl;
        outputFile << "Gap : " << gap * 100 << "%" << endl;
        outputFile << "----------------------------------------" << endl;

		// Écriture CSV pour Python

		std::string croisement_str = (croisement_type == CROISEMENT_1X) ? "1x" :
                                     (croisement_type == CROISEMENT_2X) ? "2x" : "lox";
        std::string opt_str = utiliser_2opt ? "2opt" : "no2opt";

        std::ofstream csvFile("results/algo_genetique_results.csv", std::ios::app);
        if (csvFile.tellp() == 0) { // Si le fichier est vide, écrire l'en-tête
   			csvFile << "instance,nb_generation,taille_population,taux_croisement,taux_mutation,fitness,solution,temps,gap,croisement,2opt\n";
		}
        csvFile << instance << ","
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
                << croisement_str << "," << opt_str << "\n";
        csvFile.close();
	}
	return 0;
}
