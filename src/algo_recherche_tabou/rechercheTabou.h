
#ifndef _RT_H
#define _RT_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include "solution.h"
#include "utils.h"

// cette classe d�finie les param�tres d'une ex�cution
// de la recherche tabou ainsi que la proc�dure principale de recherche
class rechercheTabou{
    public:
        // ATTRIBUTS
        int iter_courante;
        int nbiterations;              // nombre d'it�rations apr�s quoi la recherche est arr�t�e
        int duree_tabou;               // dur�e tabou en nombre d'it�rations
        int taille_solution;           // nombre de villes dans le solution
        solution *courant;             // solution courante g�r�e par la RechercheTabou
        int **list_tabou2;             // liste des solutions tabou
        int **list_tabou;              // liste des dur�es tabou associ�es � chaque couple de villes
        int **les_distances;           // matrice des distances entre les villes	   
        int type_voisinage; // 1: swap, 2: 2-opt                                              

        // CONSTRUCTEURS
        rechercheTabou(int nbiter, int dt, int nv, char* nom_fichier, int type_vois);  // construction de la rechercheTabou
        ~rechercheTabou();

        // METHODES
        bool nonTabou(int i,int j);                      // le couple (ville i, ville j) est tabou : oui ou non
        bool nonTabou2(solution* sol);                   // la solution 'sol' est tabou : oui ou non
        void voisinage_swap(int& best_i, int &best_j);   // donne le meilleur voisin non tabou
        void constuction_distance(int nv, char* nom_fichier);                
        void mise_a_jour_liste_tabou_2(solution* sol, int& position); // ajout de la solution 'sol' � la position 'position' de la liste tabou
        solution* optimiser();                           // lancement de la rechercheTabou
        void voisinage_2opt(int &best_i, int &best_j);
};

# endif
