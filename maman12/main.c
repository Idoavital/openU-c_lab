
#include <stdio.h>
#include <string.h>
#include "adjacency.h"

#define LEN_INPUT 20
#define YELLO "\033[0;33m"		/*Yello color*/
#define RESET "\033[0;0m"		/*Reset to defualt color*/
/*************************************************************/
/*************************************************************/

/* function to read the adjmat tree input */
static void SetAdjmatTree(adjmat pAdjMat)
{
	int indexRow = 0;
	int indexCol = 0;

	for (indexRow = 0 ; indexRow < N ; indexRow++)
		for (indexCol = 0 ; indexCol < N ; indexCol++)
			scanf("%d",&pAdjMat[indexRow][indexCol]);

	/* flush the input buffer */
	while(getchar()!='\n');
}
/*************************************************************/
/*************************************************************/
static void printMat(adjmat pAdjMat)
{
	int indexRow = 0;
	int indexCol = 0;

	printf("\nYour matrix:\n");

	for (indexRow = 0 ; indexRow < N ; indexRow++)
	{
		for (indexCol = 0 ; indexCol < N ; indexCol++)
			printf("%d  ",pAdjMat[indexRow][indexCol]);
		printf("\n");
	}
}
/*************************************************************/
/*************************************************************/

int main(int argc, char const *argv[])
{
	int U,V = 0;	/* the index of the junction in the tree */
	int res = 0;	/* the result of the path */
	adjmat AdjMat ; 

	char strUVinput[LEN_INPUT];		/*for read input u v path indexes*/

	/* Instructions for the user, to input a adjacency tree */
	printf("Please enter adjacency matrix tree !\n");
	printf("The matrix size is  %d by %d \n",N,N);
	printf("For every cell enter 0 or 1, after every input enter a space\n");
	printf("After every row press enter \n\n");

	/* Read the adjmat from the user */
	SetAdjmatTree(AdjMat);
	printMat(AdjMat);

	/*Instructions to input junctions*/
	printf("\n\nInput two indexes (with a space between) of the path you want to check in the tree,\n \
			\rFirst input the begining of the path, and the second index is will be the end of the path\n");

	while(fgets(strUVinput,LEN_INPUT,stdin))
	{

		sscanf(strUVinput ,"%d %d ",&U,&V);
		
		/*Get out from the loop*/
		if( (U == -1) && (V == -1)) 
			break;

		res = path (AdjMat ,U,V);

		/*Print the result to stdout*/
		if (res)
			printf(YELLO"\nFor the path from index [%d] to index [%d], the path IS EXISTS\n"RESET,U,V );
		else
			printf(YELLO"\nFor the path from index [%d] to index [%d], there NO EXISTS path\n"RESET,U,V );
		
		
		printf("Enter a new indexes or EOF to exit\n");
	}

	printf("Goodbye !!!\n");
	return 0;
}
