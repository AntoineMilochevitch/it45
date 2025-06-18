#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

struct Ville {
    int id;
    double x, y;
};

std::vector<Ville> lireCoordonnees(const std::string& fichier);
std::vector<std::vector<double>> genererMatriceDistances(const std::vector<Ville>& villes);

#endif