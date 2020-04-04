

#include "adjacency.h"

/*******************************************************/

/*******************************************************/
static int RecPath(adjmat pAdjmat,int u, int v ,int start)
{
	int index = 0;
	/* If cycle path*/
	if (u == start)
		return FALSE;

	for(index = 0 ; index < N ; index++)
	{
		/* if we reach to V point in the path*/
		if (u == v)
			return TRUE;

		/*if there is a continue to the path */
		else if (pAdjmat[u][index]== TRUE)
		{
			if (RecPath(pAdjmat, index, v ,start) == TRUE)
				return TRUE;
		}
	}

	/*if reach to end of the path*/
	return FALSE;

}
/*******************************************************/

/*******************************************************/
int path(adjmat pAdjmat,int u, int v)
{
	if (u == v)
		return TRUE;

	if (u >= N || v >= N)
		return FALSE;

	return RecPath( pAdjmat, u,  v ,u);

}