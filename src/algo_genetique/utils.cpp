#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include <cmath>
#include <vector>

struct Ville {
    int id;
    double x, y;
};

std::vector<Ville> lireCoordonnees(const std::string& fichier) {
    std::ifstream file(fichier);
    std::vector<Ville> villes;
    std::string ligne;

    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << fichier << std::endl;
        return villes;
    }

    bool sectionCoord = false;
    while (std::getline(file, ligne)) {
        if (ligne == "NODE_COORD_SECTION") {
            sectionCoord = true;
            continue;
        }
        if (ligne == "EOF") break;

        if (sectionCoord) {
            std::istringstream iss(ligne);
            Ville ville;
            iss >> ville.id >> ville.x >> ville.y;
            villes.push_back(ville);
        }
    }

    file.close();
    return villes;
}

std::vector<std::vector<double>> genererMatriceDistances(const std::vector<Ville>& villes) {
    int n = villes.size();
    std::vector<std::vector<double>> distances(n, std::vector<double>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                double dx = villes[i].x - villes[j].x;
                double dy = villes[i].y - villes[j].y;
                distances[i][j] = std::sqrt(dx * dx + dy * dy);
            } else {
                distances[i][j] = -1; // Distance à soi-même
            }
        }
    }

    return distances;
}