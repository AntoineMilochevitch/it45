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

#define NBR_TOWNS 6

/* Distance matrix */
double dist[NBR_TOWNS][NBR_TOWNS] ;

/* next_town[i] = next town after town i */
int next_town[NBR_TOWNS] ;

/* no comment */
int best_solution[NBR_TOWNS] ;
double best_eval=-1.0 ;


/**
 * Berlin52 :
 *  6 towns : Best solution (2315.15): 0 1 2 3 5 4
 * 10 towns : Best solution (2826.50): 0 1 6 2 7 8 9 3 5 4
 */
double coord[NBR_TOWNS][2]=
{
    {565.0,  575.0},
    { 25.0,  185.0},
    {345.0,  750.0},
    {945.0,  685.0},
    {845.0,  655.0},
    {880.0,  660.0}
} ;



/**
 * print a matrix
 */
void print_matrix(double d[NBR_TOWNS][NBR_TOWNS])
{
    int i, j ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("%d\t", i+1) ;
        for (j=0; j<NBR_TOWNS; j++)
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
    /**
     *  TO COMPLETE
     *  ...
     *  ...
     */
}



/**
 * print a solution
 */
void print_solution(int* sol, double eval)
{
    int i ;
    printf ("(%f): ", eval) ;
    for (i=0; i<NBR_TOWNS; i++)
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
    for (i=0; i<NBR_TOWNS; i++)
    {
        start = sol[i] ;
        end = sol[(i+1)%NBR_TOWNS];
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

    /* default solution : 1-2-3-...-n */
    for (i=0;i<NBR_TOWNS;i++) 
        sol[i] = i ;

    /**
     *  Build an heuristic solution : the nearest neighbour
     *  TO COMPLETE
     *  ...
     *  ...
     */
    eval = evaluation_solution(sol) ;
    printf("Initial solution ") ;
    print_solution (sol, eval) ;

    /* initialize best solution with initial solution */
    for (i=0;i<NBR_TOWNS;i++) best_solution[i] = sol[i] ;
    best_eval  = eval ;

    return eval ;
}




/**
 *  Build final solution
 */
void build_solution()
{
    int i, solution[NBR_TOWNS] ;

    int indiceCour = 0;
    int villeCour = 0;

    while (indiceCour < NBR_TOWNS)
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
        for (i=0; i<NBR_TOWNS; i++)
            best_solution[i] = solution[i] ;
        printf ("New best solution: ") ;
        print_solution (solution, best_eval) ;
    }
    return;
}




/**
 *  Little Algorithm
 */
void little_algorithm(double d0[NBR_TOWNS][NBR_TOWNS], int iteration, double eval_node_parent)
{

    if (iteration == NBR_TOWNS)
    {
        build_solution ();
        return;
    }

    /* Do the modification on a copy of the distance matrix */
    double d[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d, d0, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    int i, j ;

    double eval_node_child = eval_node_parent;

    /**
     * substract the min of the rows and the min of the columns
     * and update the evaluation of the current node
     *  TO COMPLETE
     *  ...
     *  ...
     */


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

    /**
     *  Store the row and column of the zero with max penalty in
     *  starting_town and ending_town
     *  TO COMPLETE
     *  ...
     *  ...
     */

    /* Do the modification on a copy of the distance matrix */
    double d2[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    /**
     *  Modify the matrix d2 according to the choice of the zero with the max penalty
     *  TO COMPLETE
     *  ...
     *  ...
     */

    /* Explore left child node according to given choice */
    little_algorithm(d2, iteration + 1, eval_node_child);

    /* Do the modification on a copy of the distance matrix */
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    /**
     *  Modify the dist matrix to explore the other possibility : the non-choice
     *  of the zero with the max penalty
     *  TO COMPLETE
     *  ...
     *  ...
     */

    /* Explore right child node according to non-choice */
    little_algorithm(d2, iteration, eval_node_child);

}




/**
 *
 */
int main (int argc, char* argv[])
{

    best_eval = -1 ;

    /* Print problem information */
    printf ("Points coordinates:\n") ;
    int i ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("Node %d: x=%f, y=%f\n", i, coord[i][0], coord[i][1]) ;
    }
    printf ("\n") ;

    compute_matrix(dist) ;

    printf ("\n") ;
    printf ("Distance Matrix:\n") ;
    print_matrix (dist) ;
    printf ("\n") ;

   
    /* initial solution */
    double initial_value =initial_solution() ;

    /** Little : uncomment when needed
     * 
    int iteration = 0 ;
    double lowerbound = 0.0 ;
     
    little_algorithm(dist, iteration, lowerbound) ;
     
    printf("Best solution:") ;
    print_solution (best_solution, best_eval) ;
     */

    printf ("Hit RETURN!\n") ;
    getchar() ;

    return 0 ;
}
