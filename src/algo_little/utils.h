#ifndef UTILS_H
#define UTILS_H

typedef struct {
    int id;
    double x;
    double y;
} Ville;

// Fonction pour lire les coordonnées d'un fichier TSP
int lireCoordonneesFichier(const char* fichier, Ville* villes, int max_villes, int* nb_villes);

// Fonction pour générer la matrice des distances
void genererMatriceDistances(const Ville* villes, int nb_villes, double distances[280][280]);

#endif
