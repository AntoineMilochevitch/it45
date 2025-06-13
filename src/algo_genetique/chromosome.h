#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "random.h"

// La classe chromosome represente la structure d'une solution du problème
class chromosome{
public:
    // ATTRIBUTS
	int *genes;         // les gènes du chromosome/solution
	int taille;         // la taille du chromosome = nombre de gènes
	int fitness;        // la valeur de la fonction objectif (fitness) de la solution

	// CONSTRUCTEURS
	chromosome(int tc); // constructeur de l'objet aléatoirement
	~chromosome();      // destructeur de l'objet

	// METHODES
	void afficher();    // fonction d'affichage du chromosome (i.e. de la solution)
	void evaluer(int **distance);
	                    // fonction d'évaluation du chromosome (c-à-d calcul la fitness)
                        //   Elle doit etre lancée à la creation des solution et apres
					    //   l'exécution des operateurs de mutation et de croisement
    void ordonner();    // ordonne le sens de la tournée si gene[1] > gene[taille-1]
    void copier(chromosome* source);  // copie le chromosome 'source'
    bool identique(chromosome* chro); // test si 2 chromosome sont identique

    // OPERATEURS DE MUTATION
    void echange_2_genes(int gene1,     // interchange 2 gènes du chromosome
                         int gene2);
    void echange_2_genes_consecutifs(); // interchange 2 gènes consécutifs du chromosome
    void echange_2_genes_quelconques(); // interchange aléatoirement 2 gènes du chromosome
    void deplacement_1_gene();          // déplace un gène dans le chromosome
    void inversion_sequence_genes();    // inverse une séquence de gènes du chromosome
};


































# endif
