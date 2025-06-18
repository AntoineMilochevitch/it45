/**
 * Traveling Salesman Problem (TSP)
 *
 * Solution optimale : 1-3-6-2-5-4-1 (dist=20)
 *
 * Author : Olivier Grunder
 * 
 */
/* parameters */
param n ;

set I := (1..n) ;

param d{i in I, j in I};

/* Decision variables */
var x {i in I, j in I} binary ;

/* Subtour Elimination Constraint MTZ : ui = order of visit of town i */
var u{i in I }, integer, >=1, <=n ;

/* Objective function */
minimize z: sum{i in I, j in I : i!=j} d[i,j]*x[i,j] ;

/* Constraints */
s.t. totown{j in I}   : sum{i in I : i!=j} x[i,j] = 1 ;
s.t. fromtown{i in I} : sum{j in I : i!=j} x[i,j] = 1 ;

/* Subtour Elimination Constraint MTZ */
s.t. tour1 : u[1] = 1 ;
s.t. tour{i in I, j in I : i>=1 and j>=2 and i!=j} : u[i]-u[j]+n*x[i,j] <= n-1 ; 

solve ; 

display u ;

printf {i in I, j in I : x[i,j]>0} "de ville a ville : x[%d ,%d]=%d\n", i, j, x[i,j] ;

printf "-----\n" ;
printf "Tour= " ;
for {visitOrder in 1..n }
{
	printf {i in I : u[i]==visitOrder} "%d ", i ;
}
printf "\n" ;
printf "dist.tot=%f\n",z;
printf "-----\n" ;

/** Data section */
data;

param n := 6 ;

param d : 1 2 3 4 5 6 :=
	1 0 1 7 3 14 2 
	2 3 0 6 9 1 24 
	3 6 4 0 3 7 3  
	4 2 3 5 0 9 11 
	5 15 7 11 2 0 4 
	6 20 5 13 4 18 0 ;


end ;

