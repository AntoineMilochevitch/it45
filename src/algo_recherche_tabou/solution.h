
#ifndef SOLUTION_H
#define SOLUTION_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "random.h"

// La classe solution represente la structure
// d'une solution du probleme du voyageur de commerce
class solution{
    public:
        // ATTRIBUTS
        int *ville;                  // les variables de la solution
        int taille;                  // la taille du solution
        int fitness;                 // la valeur de fitness de la solution = longueur de la boucle

        // CONSTRUCTEURS
        solution(int nv);
        ~solution();
        solution& operator=(const solution& source);    // copie la solution 'source'
        bool operator==(const solution& chro);   // test si 2 solutions sont identiques

        // METHODES
        //   Elle doit etre lancée à la creation des solution et apres
        //   l'exécution des operateurs de mutation et de croisement
        void evaluer(int **distance);     // fonction d'évaluation de la solution (c-à-d calcul la fitness)
        void afficher();                  // fonction affichant la solution
        void ordonner();                  // ordonne le sens de la tournée si ville[1] > ville[taille-1]
        void swap(int ville1, int ville2);          // interchange 2 villes de la solution
};

# endif

