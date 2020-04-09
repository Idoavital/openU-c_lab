
#include "adjacency.h"


/*******************************************************/
int path(adjmat pAdjmat,int u, int v)
{
	int index = 0;

	if (u >= N || v >= N)
		return FALSE;

	for(index = 0 ; index < N ; index++)
	{
		/* if we reach to V point in the path*/
		if (u == v)
			return TRUE;

		/*if there is a continue to the path */
		else if (pAdjmat[u][index]== TRUE)
		{
			if (path(pAdjmat, index, v) == TRUE)
				return TRUE;
		}
	}

	/*if reach to end of the path*/
	return FALSE;
}