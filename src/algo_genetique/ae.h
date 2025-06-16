#ifndef _AE_H
#define _AE_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "population.h"
#include "chromosome.h"

using namespace std;
// La classe Ae d�finie les param�tres d'une ex�cution d'un algorithme
//    �volutionniste ainsi que la proc�dure principale de recherche

enum CroisementType { CROISEMENT_1X, CROISEMENT_2X, CROISEMENT_LOX };
class Ae{
public:
    // ATTRIBUTS
	int nbgenerations;       // nombre de g�n�rations apr�s quoi la recherche est arr�t�e
	int taille_pop;          // nombre d'individus dans la population
	double taux_croisement;  // taux de croisement : valeur entre 0 et 1
	double taux_mutation;    // taux de mutation : valeur entre 0 et 1
	int taille_chromosome;   // nombre de g�nes dans le chromosome
	population *pop;         // liste des individus de la population
    int **les_distances;     // matrice des distances entre les villes
	CroisementType croisement_type;
    bool utiliser_2opt;

	// CONSTRUCTEURS
	Ae(int nbg, int tp, double tcroisement, double tmutation, int tc, char* nom_fichier,
       CroisementType croisement_type = CROISEMENT_2X, bool utiliser_2opt = true);
                             // constructeur de l'objet Algaorithme evolutioniste
	~Ae();                   // destructeur de l'objet Ae

	// METHODES
	void constuction_distance(char* nom_fichier);
                             // constuie la matrice des distances
	chromosome* optimiser(); // fonction principale qui d�cit le d�roulement de l'algorithme �volusionniste

	// OPERATEURS DE CROISEMENT
	// op�rateur de croisement 1X de deux chromosomes
    void croisement1X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant1, chromosome* enfant2);
	// op�rateur de croisement 1X de deux chromosomes
    void croisement2X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant1, chromosome* enfant2);
	// op�rateur de croisement Linear Order LOX de deux chromosomes
    void croisementLOX(chromosome* parent1, chromosome* parent2,
                        chromosome* enfant_s1, chromosome* enfant_s2);
};

# endif
