#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include "utils.h"

std::vector<Ville> lireCoordonnees(const std::string& fichier) {
    std::vector<Ville> villes;
    std::ifstream fin(fichier);
    if (!fin.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier : " << fichier << std::endl;
        return villes;
    }

    std::string line;
    // Chercher la section NODE_COORD_SECTION
    while (std::getline(fin, line)) {
        if (line.find("NODE_COORD_SECTION") != std::string::npos)
            break;
    }

    // Lire les coordonnées jusqu'à EOF ou une ligne contenant "EOF"
    while (std::getline(fin, line)) {
        if (line.find("EOF") != std::string::npos)
            break;
        std::istringstream iss(line);
        int id;
        double x, y;
        if (!(iss >> id >> x >> y))
            continue;
        villes.push_back({id, x, y});
    }
    return villes;
}

std::vector<std::vector<double>> genererMatriceDistances(const std::vector<Ville>& villes) {
    int n = static_cast<int>(villes.size());
    std::vector<std::vector<double>> distances(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                distances[i][j] = 0.0;
            } else {
                double dx = villes[i].x - villes[j].x;
                double dy = villes[i].y - villes[j].y;
                distances[i][j] = std::sqrt(dx * dx + dy * dy);
            }
        }
    }
    return distances;
}