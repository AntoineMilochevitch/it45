
#ifndef _RT_H
#define _RT_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include "solution.h"

// cette classe définie les paramètres d'une exécution
// de la recherche tabou ainsi que la procédure principale de recherche
class rechercheTabou{
    public:
        // ATTRIBUTS
        int iter_courante;
        int nbiterations;              // nombre d'itérations après quoi la recherche est arrétée
        int duree_tabou;               // durée tabou en nombre d'itérations
        int taille_solution;           // nombre de villes dans le solution
        solution *courant;             // solution courante gérée par la RechercheTabou
        int **list_tabou2;             // liste des solutions tabou
        int **list_tabou;              // liste des durées tabou associées à chaque couple de villes
        int **les_distances;           // matrice des distances entre les villes	                                                 

        // CONSTRUCTEURS
        rechercheTabou(int nbiter, int dt, int nv, char* nom_fichier);  // construction de la rechercheTabou
        ~rechercheTabou();

        // METHODES
        bool nonTabou(int i,int j);                      // le couple (ville i, ville j) est tabou : oui ou non
        bool nonTabou2(solution* sol);                   // la solution 'sol' est tabou : oui ou non
        void voisinage_swap(int& best_i, int &best_j);   // donne le meilleur voisin non tabou
        void constuction_distance(int nv, char* nom_fichier);                
        void mise_a_jour_liste_tabou_2(solution* sol, int& position); // ajout de la solution 'sol' à la position 'position' de la liste tabou
        solution* optimiser();                           // lancement de la rechercheTabou
};

# endif
