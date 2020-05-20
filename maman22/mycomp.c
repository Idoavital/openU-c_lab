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


	while (state != STATE_STOP)
	{
		arrStateFunc[state](&stData);
	}
	
	return 0;
}

/******************************************************************/
				/* read param functions */
/******************************************************************/
/**
 * @the function check if the string is from the format "cmd operand , number , number "
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
 * @the function check if the string is from the format "cmd operand , operand "
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
 * @the function check if the string is from the format "cmd operand "
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
 * @the function check if the string is from the format "cmd operand , number "
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


