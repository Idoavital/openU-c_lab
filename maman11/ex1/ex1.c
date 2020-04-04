
/****************************************************************
* main -- program to find the sequence of a string		     	*
*                                                     			*
* Author:  Ido Abitbul.                                         *
*                                                               *
* Purpose: to test f_sequence function                          *
* Usage :											  			*
*      Runs the program and follow by the order that apper on   *
*   	the screen							         			*
****************************************************************/

#include <stdio.h>      /* scanf       */
#include "sequence.h"   /* f_sequenece */



int main(int argc, char const *argv[])
{

    char str[MAX_INPUT];    /**/
    
    printf("Enetr a string to find is sequence, \n\
            \rPress enter key after evrey string, \n\
            \rto end EOF (ctrl + d)\n\n");

    /* While scanf don't return with EOF */
    /* If you want to scan allso white space, use fgets insted of scanf */
    while( scanf("%s",str ) != EOF)
    {
        f_sequence(str);
    }

    return 0;
}
