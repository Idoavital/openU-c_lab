#if !defined(H_ADJACENCY)
#define H_ADJACENCY


/*****************************************************/
/**************** DEFINES ****************************/

#define N 		4 		/* The tree matrix size N*N */
#define TRUE 	1
#define FALSE 	0

typedef int adjmat[N][N];

/****************************************************/
/***************** FUNCTION  ************************/

/**
 * the function check if exists a path from cross section U to cross section V 
 * in the adjmat tree.
 * @param pAdjamt is a pointer to adjmat tree 
 * @param u 	  is the start place of the path in the tree
 * @param v		  is the end place of the path in the tree
 * @return 		  int variable - true (1) if exists path from U to V, and false (0) if  there isn't path
*/
int path(adjmat pAdjmat,int u, int v);


#endif /* H_ADJACENCY */
