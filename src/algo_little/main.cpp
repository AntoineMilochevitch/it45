#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <map>
#include "little.h"


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

int main()
{
    std::ofstream outputFile("results/little.txt", std::ios::app);
    if (!outputFile.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier little.txt" << endl;
        return 1;
    }

    std::vector<std::string> instances = {
		"data/a280.tsp",
		"data/berlin52.tsp",
		"data/eil76.tsp",
		"data/kroA100.tsp"
	};

    for (const auto& instance : instances) {
        cout << "Instance : " << instance << endl;

        // Instanciation de l'algorithme de Little
        TSP algo;

        auto start = chrono::high_resolution_clock::now();
        auto resultat = algo.solve(instance);
        auto end = chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        int cout_solution = resultat.first;
        std::vector<int> chemin = resultat.second;

        cout << "La meilleure solution trouvee est : ";
        for (int ville : chemin) cout << ville << " ";
        cout << endl;

        cout << "Coût : " << cout_solution << endl;
        cout << "Temps d'execution : " << elapsed.count() << " secondes" << endl;

        float gap = calculer_gap(cout_solution, solutionsOptimales[instance]);
        cout << "Gap : " << gap * 100 << "%" << endl;
        cout << "----------------------------------------" << endl;

        outputFile << "Instance : " << instance << endl;
        outputFile << "Coût : " << cout_solution << endl;
        outputFile << "Chemin : ";
        for (int ville : chemin) outputFile << ville << " ";
        outputFile << endl;
        outputFile << "Temps d'execution : " << elapsed.count() << " secondes" << endl;
        outputFile << "Gap : " << gap * 100 << "%" << endl;
        outputFile << "----------------------------------------" << endl;

        std::ofstream csvFile("results/little_results.csv", std::ios::app);
        if (csvFile.tellp() == 0) {
            csvFile << "instance,cout,chemin,temps,gap\n";
        }

        csvFile << instance << "," << cout_solution << ",\"";
        for (size_t i = 0; i < chemin.size(); ++i) {
            csvFile << chemin[i];
            if (i < chemin.size() - 1) csvFile << "-";
        }
        csvFile << "\"," << elapsed.count() << "," << gap * 100 << "\n";
        csvFile.close();
    }

    return 0;
}
