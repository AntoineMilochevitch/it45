 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>
 #include <time.h>
 #include <limits.h>
#include "utils.h"
#include "instances.h"

#define NBR_TOWNS 280
#define MAX_VILLES 280
#define min(a,b) ((a) < (b) ? (a) : (b))


int nbr_town;
double (*dist)[NBR_TOWNS];
next_town
int *best_solution;          // Meilleure solution trouvée
double best_eval = -1.0 ;
int first_it = 1;

double coord[MAX_VILLES][2];
Ville *villes;             // Tableau des villes


double calculer_gap(int z, int z_optimal) {
    return (double)(z - z_optimal) / z_optimal;
}





 /**
  * print a matrix
  */
 void print_matrix(double d[NBR_TOWNS][NBR_TOWNS])
 {
     int i, j ;
     for (i=0; i<nbr_town; i++)
     {
         printf ("%d\t", i+1) ;
         for (j=0; j<nbr_town; j++)
         {
             printf ("%f ", d[i][j]) ;
         }
         printf ("\n") ;
     }
 }



 /**
  * compute distance matrix
  */
 void compute_matrix(double d[NBR_TOWNS][NBR_TOWNS])
 {
     for (int i = 0; i < nbr_town; i++) {
         for (int j = 0; j < nbr_town; j++) {
             if (i == j) {
                 d[i][j] = -1;
             } else {
                 double dx = villes[j].x - villes[i].x;
                double dy = villes[j].y - villes[i].y;
                d[i][j] = sqrt(dx * dx + dy * dy);
             }
         }
     }
 }



 /**
  * print a solution
  */
 void print_solution(int* sol, double eval)
 {
     int i ;
     printf ("(%f): ", eval) ;
     for (i=0; i<nbr_town; i++)
         printf ("%d ",sol[i]+1);
     printf("\n") ;
 }




 /**
  * evaluation of a solution
  */
 double evaluation_solution(int* sol)
 {
     double eval=0 ;
     int i ;
     int start, end ;
     for (i=0; i<nbr_town; i++)
     {
         start = sol[i] ;
         end = sol[(i+1)%nbr_town];
         eval += dist[start][end] ;
     }

     return eval ;
 }




 /**
  * initial solution
  */
double initial_solution()
{
    /* solution of the nearest neighbour */
    int i, sol[NBR_TOWNS] ;

    /* evaluation of the solution */
    double eval = 0 ;

    /* Heuristic solution : the nearest neighbour */
	int visited[NBR_TOWNS] = {0};
    int current = 0;
	sol[0] = current;
	visited[current] = 1;

	for (i = 1; i < nbr_town; i++) {
		double min_dist = -1;
		int next = -1;
		for (int j = 0; j < nbr_town; j++) {
    		if (!visited[j] && dist[current][j] != -1) {
        		if (min_dist == -1 || dist[current][j] < min_dist) {
            		min_dist = dist[current][j];
            		next = j;
        		}
     		}
 		}
 		if (next == -1) break;
 		sol[i] = next;
 		visited[next] = 1;
 		current = next;
	}

     eval = evaluation_solution(sol) ;
     printf("Initial solution ") ;
     print_solution (sol, eval) ;

     /* initialize best solution with initial solution */
     for (i=0;i<nbr_town;i++) best_solution[i] = sol[i] ;
     best_eval  = eval ;

     return eval ;
 }



 /**
  *  Build final solution
  */
 void build_solution()
 {
    int i, solution[NBR_TOWNS];
    memset(solution, 0, sizeof(solution));


    int indiceCour = 0;
    int villeCour = 0;
	int visited[NBR_TOWNS] = {0};

	int compteur = 0;
    int max_iterations = nbr_town * 2;



     while (indiceCour < nbr_town && compteur < max_iterations)
    {
        // Vérification des indices
        if (villeCour < 0 || villeCour >= nbr_town)
        {
            return;
        }

        solution[indiceCour] = villeCour;
        visited[villeCour] = 1;

        /* Test si le cycle est hamiltonien */
        for (i = 0; i < indiceCour; i++)
        {
            if (solution[i] == villeCour)
            {
                return;
            }
        }

        /* Recherche de la ville suivante */
        int prochaine_ville = next_town[villeCour];

        // Vérification de la validité de la prochaine ville
        if (prochaine_ville < 0 || prochaine_ville >= nbr_town)
        {
            return;
        }

        villeCour = prochaine_ville;
        indiceCour++;
        compteur++;  // Protection contre les boucles infinies
    }


     if (indiceCour == nbr_town)
    {
        double eval = evaluation_solution(solution);

        if (best_eval < 0 || eval < best_eval)
        {
            best_eval = eval;
            for (i = 0; i < nbr_town; i++)
                best_solution[i] = solution[i];
            printf("New best solution: ");
            print_solution(solution, best_eval);
        }
    }

 }


 double reduire_lignes(double mat[NBR_TOWNS][NBR_TOWNS]) {
     double LB = 0;
     for (int i = 0; i < nbr_town; i++) {
         double min = INT_MAX;
         for (int j = 0; j < nbr_town; j++) {
             if (mat[i][j] >= 0 && mat[i][j] < min)
                 min = mat[i][j];
         }
         if (min != INT_MAX && min > 0) {
             for (int j = 0; j < nbr_town; j++) {
                 if (mat[i][j] >= 0)
                     mat[i][j] -= min;
             }
             LB += min;
         }
     }
     return LB;
 }

 double reduire_colonnes(double mat[NBR_TOWNS][NBR_TOWNS]) {
     double LB = 0;
     for (int j = 0; j < nbr_town; j++) {
         double min = INT_MAX;
         for (int i = 0; i < nbr_town; i++) {
             if (mat[i][j] >= 0 && mat[i][j] < min)
                 min = mat[i][j];
         }
         if (min != INT_MAX && min > 0) {
             for (int i = 0; i < nbr_town; i++) {
                 if (mat[i][j] >= 0)
                     mat[i][j] -= min;
             }
             LB += min;
         }
     }
     return LB;
 }

 /**
  *  Little Algorithm
  */
void little_algorithm(double d0[NBR_TOWNS][NBR_TOWNS], int iteration, double eval_node_parent)
{
	if (iteration < 0 || iteration >= NBR_TOWNS) {
        printf("ERREUR : iteration invalide (%d)\n", iteration);
        return;
    }

    static int recursion_depth = 0;
    recursion_depth++;
    if (recursion_depth > NBR_TOWNS * 2) {
        printf("ERREUR : Profondeur de récursion excessive (%d)\n", recursion_depth);
        recursion_depth--;
        return;
    }

    if (iteration > nbr_town * 2) {
        recursion_depth--;
        return;
    }

    /* Do the modification on a copy of the distance matrix */
    double d[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d, d0, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    int i, j ;

    double eval_node_child = eval_node_parent;

    /* substract the min of the rows and the min of the columns
     * and update the evaluation of the current node */
    eval_node_child += reduire_lignes(d);
    eval_node_child += reduire_colonnes(d);


    /* Cut : stop the exploration of this node */
    if (best_eval>=0 && eval_node_child >= best_eval)
        return;


    /**
     *  Compute the penalities to identify the zero with max penalty
     *  If no zero in the matrix, then return, solution infeasible
     *  TO COMPLETE
     *  ...
     *  ...
    */
    /* row and column of the zero with the max penalty */
    int izero=-1, jzero=-1 ;
    double pmax = -1;           // max pénalité

    for (i = 0; i < nbr_town; i++) {
        for (j = 0; j < nbr_town; j++) {
            if (d[i][j] == 0) {
                double min_lig = INT_MAX, min_col = INT_MAX;
                for (int k = 0; k < nbr_town; k++) {
                    if (k != j && d[i][k] >= 0)
                        if (d[i][k] < min_lig) min_lig = d[i][k];
                    if (k != i && d[k][j] >= 0)
                        if (d[k][j] < min_col) min_col = d[k][j];
                }

                double pen;
                if (min_lig != INT_MAX && min_col != INT_MAX) {
                    pen = min_lig+min_col;
                }
                else {pen = 0;}

                if (pen > pmax) {
                    pmax = pen;
                    izero = i;
                    jzero = j;
                }
            }
        }
    }
    if (first_it == 1) {
        printf("1er zero ayant la penalite la plus forte : depart = %d  arrivee = %d\n", izero, jzero);
        first_it = 0;
    }

    if (izero == -1 || jzero == -1 || izero >= nbr_town || jzero >= nbr_town) {
        return;
    }


     /* Store the row and column of the zero with max penalty in
      *  starting_town and ending_town */

     int starting_town = izero;
     int ending_town = jzero;

     /* Do the modification on a copy of the distance matrix */
     double d2[NBR_TOWNS][NBR_TOWNS] ;
     memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

     // On suprrime la ligne imax et la colonne jmax
     for (int k = 0; k < nbr_town; k++) {
         d2[starting_town][k] = -1;
         d2[k][ending_town] = -1;
     }
     // Chemin inverse interdit
     d2[ending_town][starting_town] = -1;

     next_town[starting_town] = ending_town;

     /* Explore left child node according to given choice */
     little_algorithm(d2, iteration + 1, eval_node_child);

     /* Do the modification on a copy of the distance matrix */
     memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

     d2[starting_town][ending_town] = -1;

     /* Explore right child node according to non-choice */
     little_algorithm(d2, iteration, eval_node_child);

}


 /**
  *
  */
int main (void){
	dist = malloc(NBR_TOWNS * sizeof(*dist));
    next_town = malloc(NBR_TOWNS * sizeof(int));
    best_solution = malloc(NBR_TOWNS * sizeof(int));
    villes = malloc(NBR_TOWNS * sizeof(Ville));

    if (!dist || !next_town || !best_solution || !villes) {
        printf("Erreur d'allocation mémoire\n");
        // Libération de la mémoire allouée
        free(dist);
        free(next_town);
        free(best_solution);
        free(villes);
        return 1;
    }


    for (int i = 0; i < NB_INSTANCES; i++) {

		// Initialisation de la matrice de distances
        for (int i = 0; i < NBR_TOWNS; i++) {
            for (int j = 0; j < NBR_TOWNS; j++) {
                dist[i][j] = -1;
            }
        }

        printf("\nInstance : %s\n", instances[i]);
        printf("----------------------------------------\n");

        // Réinitialisation des variables globales
        best_eval = -1.0;
        first_it = 1;
        memset(next_town, 0, NBR_TOWNS * sizeof(int));
        memset(best_solution, 0, NBR_TOWNS * sizeof(int));

		int nb_villes;

        // Lecture des coordonnées
        if (!lireCoordonneesFichier(instances[i], villes, NBR_TOWNS, &nb_villes)) {
            printf("Erreur lors de la lecture du fichier %s\n", instances[i]);
            continue;
        }

        nbr_town = nb_villes;

		if (nbr_town > NBR_TOWNS) {
            printf("Erreur : nombre de villes trop grand (%d > %d)\n", nbr_town, MAX_TOWNS);
            continue;
        }


        // Calcul de la matrice de distances
		genererMatriceDistances(villes, nbr_town, dist);

        // Solution initiale
        initial_solution();

        // Exécution de l'algorithme de Little
        int iteration = 0;
        double lowerbound = 0.0;

        clock_t start = clock();
printf("\nVérifications avant little_algorithm :\n");
printf("nbr_town = %d\n", nbr_town);
printf("iteration = %d\n", iteration);
printf("lowerbound = %f\n", lowerbound);
printf("best_eval = %f\n", best_eval);

// Vérification de la matrice de distances
int has_invalid_distance = 0;
int row_with_error = -1;
int col_with_error = -1;
for (int i = 0; i < nbr_town && !has_invalid_distance; i++) {
    for (int j = 0; j < nbr_town; j++) {
        if (i != j && dist[i][j] < 0) {
            has_invalid_distance = 1;
            row_with_error = i;
            col_with_error = j;
            break;
        }
    }
}

if (has_invalid_distance) {
    printf("ERREUR : Distance invalide trouvée à [%d][%d] = %f\n",
           row_with_error, col_with_error, dist[row_with_error][col_with_error]);
}

// Vérification des tableaux
printf("Taille de next_town : %lu\n", sizeof(next_town));
printf("Taille de best_solution : %lu\n", sizeof(best_solution));

// Vérification que nbr_town ne dépasse pas les limites
if (nbr_town <= 0 || nbr_town > NBR_TOWNS) {
    printf("ERREUR : nbr_town invalide (%d)\n", nbr_town);
    return 1;
}

// Vérification de la diagonale de la matrice
int diagonal_error = 0;
for (int i = 0; i < nbr_town; i++) {
    if (dist[i][i] != -1) {
        printf("ERREUR : Diagonale non initialisée à -1 à la position %d\n", i);
        diagonal_error = 1;
    }
}

printf("État de first_it : %d\n", first_it);

// Affichage des premières valeurs de la matrice pour vérification
printf("Échantillon de la matrice de distances :\n");
for (int i = 0; i < min(3, nbr_town); i++) {
    for (int j = 0; j < min(3, nbr_town); j++) {
        printf("%7.2f ", dist[i][j]);
    }
    printf("\n");
}

printf("\nDémarrage de little_algorithm...\n");

// Si tout est OK, on continue avec l'appel
if (!has_invalid_distance && !diagonal_error && nbr_town > 0 && nbr_town <= NBR_TOWNS) {
    little_algorithm(dist, iteration, lowerbound);
} else {
    printf("ERREUR : Conditions non remplies pour exécuter little_algorithm\n");
}


        little_algorithm(dist, iteration, lowerbound);
        clock_t end = clock();

        // Affichage des résultats
        printf("\nMeilleure solution trouvée :");
        print_solution(best_solution, best_eval);

        // Calcul et affichage du temps d'exécution
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %.2f secondes\n", cpu_time_used);

        // Calcul et affichage du GAP
        double gap = calculer_gap(best_eval, solutions_optimales[i]);
        printf("GAP : %.2f%%\n", gap * 100);

        printf("----------------------------------------\n");
    }

	free(dist);
    free(next_town);
    free(best_solution);
    free(villes);



	return 0 ;

}