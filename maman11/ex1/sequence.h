
#ifndef SEQUENCE_H
#define SEQUENCE_H

/***************************************************************
* sequence -- program to find the sequence of a string			*
*                                                     			*
* Author:  Ido Abitbul			                      			*
*                                                     			*
* Usage :											  			*
*      Runs the program and follow by the order that apper on   *
*   	 the screen							         		    *
****************************************************************/

#define MAX_INPUT 256  /* max lentgh of input string */






/**The function get a string and check the sequence of the string
 * and print the result on the screen.
 *
 * @param str - the string to check.
 *
 * the function will print the result like that:
 *
 *- if the string sequence is Real Up 		   (for example - "demo")
 *- if the string sequence is Up 	 		   (for example - "beef")
 *- if the string sequence is Real Down 	   (for example - "tonic")
 *- if the string sequence is Down 			   (for example - "spoon")
 *- if the string sequence is with no order  (for example - "suddenly")
 *-  string with a const sequence (for example - "aaa") will print  Up
 */
void f_sequence(char str[]);


#endif