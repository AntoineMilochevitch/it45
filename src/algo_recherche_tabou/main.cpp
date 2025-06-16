#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <chrono>
#include <map>
#include <vector>
#include "random.h"
#include "rechercheTabou.h"
#include "solution.h"

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
    Random::randomize();

    std::vector<std::string> instances = {
        "data/a280.tsp",
        "data/berlin52.tsp",
        "data/eil76.tsp",
        "data/kroA100.tsp"
    };

    // Paramètres par défaut
    int nb_iteration = 10;
    int duree_tabou  = 10;
    int nb_villes    = 0;

    int mode_arret = 1; // 1: nb itérations, 2: temps
    int duree_seconde = 60;
    cout << "Mode d'arret ? (1: nb iterations, 2: temps) [defaut: 1] : ";
    cin >> mode_arret;
    if (mode_arret == 2) {
        cout << "Durée maximale (en secondes) ? [defaut: 60] : ";
        cin >> duree_seconde;
    }
    cout << "Nombre d'iterations ? [defaut: 1000] : ";
    cin >> nb_iteration;
    cout << "Durée de la liste tabou ? [defaut: 10] : ";
    cin >> duree_tabou;
    int type_voisinage = 1; // 1: swap, 2: 2-opt
    cout << "Type de voisinage ? (1: swap, 2: 2-opt) [defaut: 1] : ";
    cin >> type_voisinage;

    // Ouvre le fichier CSV pour écrire les résultats
    std::ofstream csvFile("results/recherche_tabou_results.csv", std::ios::app);
    if (csvFile.tellp() == 0) {
        csvFile << "instance,mode_arret,nb_iteration,duree_tabou,fitness,solution,temps,gap,nb_villes,duree_seconde\n";
    }

    for (const auto& instance : instances) {
        if (instance == "data/a280.tsp") nb_villes = 280;
        else if (instance == "data/berlin52.tsp") nb_villes = 52;
        else if (instance == "data/eil76.tsp") nb_villes = 76;
        else if (instance == "data/kroA100.tsp") nb_villes = 100;

        cout << "Instance : " << instance << endl;

        solution* best = nullptr;
        int iterations_effectuees = 0;
        auto start = chrono::high_resolution_clock::now();

        if (mode_arret == 1) {
            rechercheTabou algo(nb_iteration, duree_tabou, nb_villes, const_cast<char*>(instance.c_str()), type_voisinage);
            best = algo.optimiser();
            iterations_effectuees = nb_iteration;
        } else {
            int iter = 0;
            auto t0 = chrono::high_resolution_clock::now();
            rechercheTabou algo(nb_iteration, duree_tabou, nb_villes, const_cast<char*>(instance.c_str()), type_voisinage);
            solution* current_best = nullptr;
            do {
                solution* candidate = algo.optimiser();
                if (!current_best || candidate->fitness < current_best->fitness) {
                    if (current_best) delete current_best;
                    current_best = new solution(*candidate);
                }
                delete candidate;
                iter++;
                auto t1 = chrono::high_resolution_clock::now();
                chrono::duration<double> elapsed = t1 - t0;
                if (elapsed.count() >= duree_seconde) break;
            } while (true);
            best = current_best;
            iterations_effectuees = iter;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "La meilleure solution trouvee est : ";
        best->afficher();
        float gap = calculer_gap(best->fitness, solutionsOptimales[instance]);
        cout << "Gap : " << gap * 100 << "%" << endl;
        cout << "Temps d'execution : " << elapsed.count() << " secondes" << endl;
        cout << "----------------------------------------" << endl;

        // Ecriture CSV
        csvFile << instance << ","
                << (mode_arret == 1 ? "iteration" : "temps") << ","
                << iterations_effectuees << ","
                << duree_tabou << ","
                << best->fitness << ",\"";
        for (int i = 0; i < best->taille; ++i) {
            csvFile << best->ville[i];
            if (i < best->taille - 1) csvFile << "-";
        }
        csvFile << "\"," << elapsed.count() << "," << gap * 100 << ","
                << nb_villes << "," << duree_seconde << "\n";

        delete best;
    }
    csvFile.close();
    return 0;
}