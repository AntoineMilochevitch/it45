/**
 * Projec : gtsp (voyageur de commerce)
 *
 * Date   : 07/04/2014
 * Author : Olivier Grunder
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>
 #include <time.h>
 #include <limits.h>

 #define NBR_TOWNS 6
 int nbr_town = 6;


 /* Distance matrix */
 double dist[NBR_TOWNS][NBR_TOWNS] ;

 /* next_town[i] = next town after town i */
 int next_town[NBR_TOWNS] ;

 /* no comment */
 int best_solution[NBR_TOWNS] ;
 double best_eval=-1.0 ;

 int first_it = 1;


 /**
  * Berlin52 :
  *  6 towns : Best solution (2315.15): 0 1 2 3 5 4
  * 10 towns : Best solution (2826.50): 0 1 6 2 7 8 9 3 5 4
  */
 double coord[10][2]=
 {
     {565.0,  575.0},
     { 25.0,  185.0},
     {345.0,  750.0},
     {945.0,  685.0},
     {845.0,  655.0},
     {880.0,  660.0},
     {25.0,  230.0},
     {525.0,  1000.0},
     {580.0,  1175.0},
     {650.0,  1130.0}
 };

 #define MAX_VILLES 52

 int lireCoordonnees(const char *nomFichier, double coord[MAX_VILLES][2]) {
     FILE *fichier = fopen(nomFichier, "r");
     if (!fichier) {
         perror("Erreur lors de l'ouverture du fichier");
         return 0;
     }

     char ligne[256];
     while (fgets(ligne, sizeof(ligne), fichier)) {
         if (strncmp(ligne, "NODE_COORD_SECTION", 18) == 0) {
             break;
         }
     }

     int index = 0;
     int id;
     double x, y;
     while (fgets(ligne, sizeof(ligne), fichier)) {
         if (strncmp(ligne, "EOF", 3) == 0) break;

         if (sscanf(ligne, "%d %lf %lf", &id, &x, &y) == 3) {
             if (index >= NBR_TOWNS) {
                 fprintf(stderr, "Trop de villes dans le fichier\n");
                 break;
             }
             coord[index][0] = x;
             coord[index][1] = y;
             index++;
         }
     }

     fclose(fichier);
     return 1;
 }

 /**
  * print a matrix
  */
 void print_matrix(double d[nbr_town][nbr_town])
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
 void compute_matrix(double d[nbr_town][nbr_town])
 {
     for (int i = 0; i < nbr_town; i++) {
         for (int j = 0; j < nbr_town; j++) {
             if (i == j) {
                 d[i][j] = -1;
             } else {
                 double x = coord[j][0] - coord[i][0];
                 double y = coord[j][1] - coord[i][1];
                 d[i][j] = (sqrt(x * x + y * y));
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
     int i, sol[nbr_town] ;

     /* evaluation of the solution */
     double eval = 0 ;

     /* default solution : 1-2-3-...-n */
     /*for (i=0;i<NBR_TOWNS;i++)
         sol[i] = i ;
     */

     /* Heuristic solution : the nearest neighbour */
     int visited[nbr_town] = {};
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
     int i, solution[nbr_town] ;

     int indiceCour = 0;
     int villeCour = 0;

     while (indiceCour < nbr_town)
     {

         solution[indiceCour] = villeCour ;

         /* Test si le cycle est hamiltonien */
         for (i = 0; i < indiceCour; i++)
         {
             if (solution[i] == villeCour)
             {
                 /* printf ("cycle non hamiltonien\n") ; */
                 return;
             }
         }
         /* Recherche de la ville suivante */
         villeCour = next_town[villeCour] ;

         indiceCour++;
     }

     double eval = evaluation_solution(solution) ;

     if (best_eval<0 || eval < best_eval)
     {
         best_eval = eval ;
         for (i=0; i<nbr_town; i++)
             best_solution[i] = solution[i] ;
         printf ("New best solution: ") ;
         print_solution (solution, best_eval) ;
     }
     return;
 }


 double reduire_lignes(double mat[nbr_town][nbr_town]) {
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

 double reduire_colonnes(double mat[nbr_town][nbr_town]) {
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
 void little_algorithm(double d0[nbr_town][nbr_town], int iteration, double eval_node_parent)
 {
     if ((NBR_TOWNS == 6 && (first_it == 1 && iteration == 1)) || (NBR_TOWNS == 10 && (iteration == 0))) {
         printf("Borne minimum a la 1ere iteration : %f\n", eval_node_parent);
     }
     if (iteration == nbr_town)
     {
         build_solution();
         return;
     }

     /* Do the modification on a copy of the distance matrix */
     double d[nbr_town][nbr_town] ;
     memcpy (d, d0, nbr_town*nbr_town*sizeof(double)) ;

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

     if (izero == -1 || jzero == -1) {
         return; //solution infeasible
     }

     /* Store the row and column of the zero with max penalty in
      *  starting_town and ending_town */

     int starting_town = izero;
     int ending_town = jzero;

     /* Do the modification on a copy of the distance matrix */
     double d2[nbr_town][nbr_town] ;
     memcpy (d2, d, nbr_town*nbr_town*sizeof(double)) ;

     /* Modify the matrix d2 according to the choice of the zero with the max penalty */
     // Choix constructif, on inclut le chemin starting_town -> ending_town
     for (i = 0; i < nbr_town; i++)
         for (j = 0; j < nbr_town; j++)
             d2[i][j] = d[i][j];
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
     memcpy (d2, d, nbr_town*nbr_town*sizeof(double)) ;

     /* Modify the dist matrix to explore the other possibility : the non-choice
      *  of the zero with the max penalty */
     // Autre choix, on exclut le chemin izero -> jzero
     for (int i = 0; i < nbr_town; i++)
         for (int j = 0; j < nbr_town; j++)
             d2[i][j] = d[i][j];
     d2[starting_town][ending_town] = -1;

     /* Explore right child node according to non-choice */
     little_algorithm(d2, iteration, eval_node_child);

 }




 /**
  *
  */
 int main (int argc, char* argv[])
 {
     clock_t start, end;
     double cpu_time_used;

     compute_matrix(dist);
     best_eval = -1 ;

     /* Print problem information */
     printf ("Points coordinates:\n");
     int i ;
     for (i=0; i<nbr_town; i++)
     {
         printf ("Node %d: x=%f, y=%f\n", i, coord[i][0], coord[i][1]);
     }
     printf ("\n");

     printf ("\n");
     printf ("Distance Matrix:\n");
     print_matrix(dist);
     printf ("\n");


     /* initial solution */
     double initial_value =initial_solution() ;

     // Little
     int iteration = 0 ;
     double lowerbound = 0.0 ;

     start = clock();
     little_algorithm(dist, iteration, lowerbound) ;
     end = clock();
     printf("Best solution:") ;
     print_solution (best_solution, best_eval) ;

     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("Temps de calcul : %f secondes\n", cpu_time_used);

     printf ("Hit RETURN!\n") ;
     getchar() ;

     return 0 ;
 }