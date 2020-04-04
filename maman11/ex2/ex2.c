
/**********************************************************************************
* match - program to find the index with the best match of the pattern in the text	*		*
*                                                     								*
* Author:  Ido Abitbul			                      								*
*                                                     								*
* Usage :											  								*
*      Runs the program and follow by the order that apper on   					*
*      the screen.																	*
	   use the makefile to compile.	and after .					         		    		*
*																					*
****************************************************************/


#include <stdio.h>  
#include <string.h>

#include "match.h"  /* f_match */

#define YELLO "\033[0;33m"		/*Yello color*/
#define RESET "\033[0;0m"		/*Reset to defualt color*/


/*For fgets - delete the last char if equall to '\n'*/
static void f_deleteNewLine(char *str)
{
	while(*str){
		if (*str == '\n')
			*str = '\0';
		str++;
	}

}

int main(int argc, char const *argv[])
{
	char strText[MAX_LEN];
	char strPattern[MAX_LEN];

	int resMatch	= 0;

	printf("This program check where is the position of the best match between a text to a pattern\n\
			\rYou will e ask to enter the text at the first, and after the pattern string\n\n");

	while (	1 )
	{
		printf("Enter a text please\n");
		if(fgets(strText,MAX_LEN,stdin)==NULL)
			return 0;

		printf("Enter a pattern string to find in the text\n");
		if(fgets(strPattern,MAX_LEN,stdin) == NULL)
			return 0; 

		/*becuase the function fgets read allso the char 'new line' ('\n') so we delete it */
		f_deleteNewLine(strText); 	
		f_deleteNewLine(strPattern);

		resMatch = f_match(strText , strPattern);

		printf("You Enter the text:    %s\n",strText);
		printf("You Enter the pattern: %s\n",strPattern);

		printf(YELLO"The best macth of the pattern in text, found in index [%d]\n\n"RESET,resMatch);

	}
	

	printf("Goodbye !\n");


	return 0;
}