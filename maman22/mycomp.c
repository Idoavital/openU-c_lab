/********************************************************************************************************	
* mycomp : Program to calculate arithmtic opartion on complex number												*	
*                                                     													*	
* Author:  Ido Abitbul			                      													*
*                                                     													*
* Usage :											  													*																					
*		there is 6 complex number named only A,B,C,D,E,F 												*														
*	the commands format are:																			*
*	read_comp COMPLEX VARIABLE NAME, NUMBER, NUMBER   		example: read_comp A, 6.55, -45.5			*
*	print_comp COMPLEX VARIABLE NAME						example: print_comp D						*
*	add_comp COMPLEX VARIABLE NAME, COMPLEX VARIABLE NAME	example: add_comp B,F						*
*	sub_comp COMPLEX VARIABLE NAME, COMPLEX VARIABLE NAME	example: sub_comp B,F						*
*	mult_comp_real VARIABLE NAME, NUMBER					example: mult_comp_real A, 6.55				*
*	mult_comp_img VARIABLE NAME, NUMBER						example: mult_comp_img A, 6.55				*
*	mult_comp_comp COMPLEX VARIABLE NAME, COMPLEX VARIABLE NAME example: mult_comp_comp A, B			*
*	abs_comp COMPLEX VARIABLE NAME							example: abs_comp D							*
*	stop																								*
*																										*
*	the command shuold be exactly in the concurrent format, any outher format will lead to mistake.		*
*																										*
* The algorithm:																						*
*		the program work in state machine in infinty loop												*
*		there is 4 state: 																				*
*			STATE_READ_CMD:  in this state we read the user command.									*
*			STATE_PARSE_CMD: in this state we analaze the user input string (command).					*
*			STATE_DO_CMD:	 in this state we execute the command .										*
*			STATE_STOP:		 in this state we stop the program.											*
*																										*
*		the base state is STATE_READ_CMD, if the state finsh o.k we pass to next state else				*
*		we return to STATE_READ_CMD.																	*
*																										*
*																										*
*********************************************************************************************************/																												


#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <ctype.h> /* isspace */
#include <stdlib.h> /* strtod */

#include "complex.h"
#include "compHelper.h"


/*****************************************************/
				/*global variable*/
/*****************************************************/
complex A,B,C,D,E,F;
STATE state;

static void setArrComplex(complex* pComp[COMP_VAR_NUM])
{
	pComp[0] = &A;
	pComp[1] = &B;
	pComp[2] = &C;
	pComp[3] = &D;
	pComp[4] = &E;
	pComp[5] = &F;
}
/*****************************************************/
				/* MAIN */
/*****************************************************/

int main(int argc, char const *argv[])
{

	complex*	  arrGlobal_Comp_Var[COMP_VAR_NUM]; 
	stInputParam  inParam;				/* the variable from stdin */
	comp_func 	  arrCompFunc[COMP_FUNC_NUM];
	parse_func	  arrParseFunc[PARSE_SIZE];
	state_func    arrStateFunc[STATE_SIZE];
	stData		  stData;
	char 		  cmd[MAX_LEN];

	
	/*set the complex array to variable*/
	setArrComplex(arrGlobal_Comp_Var);

	/*set the function table */
	setFuncTable(arrCompFunc, arrParseFunc, arrStateFunc);

	/*set params*/
	state = STATE_READ_CMD;

	stData.read_fun_num  = 0;
	stData.cmd_fun_num   = 0;
	stData.fp_comp_func  = arrCompFunc;
	stData.fp_parse_func = arrParseFunc;
	stData.p_comp_global = arrGlobal_Comp_Var;
	stData.p_input_param = &inParam;
	stData.str_cmd		 = cmd;


	FOREVER
	{
		arrStateFunc[state](&stData);
	}
	
	return 0;
}

/******************************************************************/
				/* read param functions */
/******************************************************************/
/**
 * @the function check if the string is from the format "complex variable , number , number "
*/
int readOneCompTwoNum(__IN char *str ,__OUT stInputParam* inParam) 
{

	/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chFirst = *str-'A';
		str++;
	}
	else 
		return false;
	
	/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return false;

	/* check and read Is float number else print error */
	if (checkIsNum(&str,&inParam->fNumFirst) == FAIL)
		return false;
	
	/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return false;


	/* check and read Is float number else print error */
	if (checkIsNum(&str,&inParam->fNumSecond) == FAIL)
		return false;

	if (checkIsEndOfString(str)  == FAIL)
		return false;
	
	return true;

}

/**
 * @the function check if the string is from the format "complex variable , operand "
*/
int readTWoComp(__IN char *str ,__OUT stInputParam* inParam)
{
	/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chFirst = *str-'A';
		str++;
	}
	else 
		return false;;
	
		/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return false;;

		/* check and read if is oprand else print error*/
	if (checkIsOp(&str,false ) == OK)
	{
		inParam->chSecond = *str-'A';
		str++;
	}
	else 
		return false;;

	if (checkIsEndOfString(str)  == FAIL)
		return false;
	
return true;

}

/**
 * @the function check if the string is from the format "complex variable "
*/
int readOneComp(__IN char *str ,__OUT stInputParam* inParam) 
{
	/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chSecond = *str-'A';
		str++;
	}
	else 
		return false;

	if (checkIsEndOfString(str)  == FAIL)
		return false;
	
	return true;
}

/**
 * @the function check if the string is from the format "complex variable , number "
*/
int readOneCompOneNum(__IN char *str ,__OUT stInputParam* inParam) 
{
		/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chSecond = *str-'A';
		str++;
	}
	else 
		return false;
	
	/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return false;

	/* check and read Is float number else print error */
	if (checkIsNum(&str,&inParam->fNumFirst) == FAIL)
		return false;
	
	if (checkIsEndOfString(str)  == FAIL)
		return false;
	
	return true;

}

/**
 * @the function check if the string is from the format "stop "
*/
int readStop(__IN char *str ,__OUT stInputParam* inParam) 
{

	if (checkIsEndOfString(str)  == FAIL)
		return false;
	
	return true;
}


