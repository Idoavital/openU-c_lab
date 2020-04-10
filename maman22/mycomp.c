#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <ctype.h> /* isspace */
#include <stdlib.h> /* strtof */


#include "complex.h"

/*****************************************************/
/*****************************************************/

#define MAX_LEN 256
#define	true	1
#define false	0
#define __IN		/* varaible get in to function */
#define	__OUT		/* varaible returend from the function */

/*****************************************************/
/*****************************************************/

enum ERROR {
	ERR_UNDEF_COMP,
	ERR_UNDEF_CMD,
	ERR_INVALID_PARAM,
	ERR_MIS_PARAM,
	ERR_EXTRA_TXT,
	ERR_MULTI_COMMA,
	ERR_MIS_COMMA,
	ERR_ILLEGAL_COMMA,
	ERR_UNDEF_ERROR

};

enum CMD {
	READ_COMP,
	PRINT_COMP,
	ADD_COMP,
	SUB_COMP,
	MULT_COMP_REAL,
	MULT_COMP_IMG,
	MULT_COMP_COMP,
	ABS_COMP,
	STOP,
	NOT_CMD
};


/*****************************************************/
/*****************************************************/
/*forward declaration*/

static void printErr(int err);

/**
 * @The function check if the string start with a operand 'A' -'G',
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the operand.
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsOp(char * str , char ** pEnd);

/**
 * @The function check if the string start with a number,
 * 	and convert the string number to float.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the number.
 * @param fNum  return float pointer that containing the converted number
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsNum(__IN char * strIn , __OUT char ** pEnd ,__OUT float *fNum);


/**
 * @The function check if the string start with a comma char(',').
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the comma char.
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsComma(__IN char * strIn , __OUT char ** pEnd );


/**
 * @The function check if the start of a string is legal command.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the command.
 * @return 		the int number of the command value
*/
static int checkCmd(__IN char * strIn , __OUT char ** pEnd);

/**
 * @The function check if a string is end of command.
 * 	
 * @param strIn pointer to the start of string.
 */
static int checkIsEndOfString(__IN char *strIn);


static void twoOpOneNum(char *str)
{
	float f;
	int ret =0;
	ret += checkIsOp(str,&str );
	ret += checkIsComma(str,&str);
	ret += checkIsNum(str,&str,&f);
	ret += checkIsComma(str,&str);
	ret += checkIsNum(str,&str,&f);
	ret += checkIsEndOfString(str);

}
/*****************************************************/
/*****************************************************/

int main(int argc, char const *argv[])
{
	char cmd[MAX_LEN] ;
/*	char Token_1[MAX_LEN];
	char Token_2[MAX_LEN];
	char Token_3[MAX_LEN];
*/
	char *pend;
	float f;
	complex a;

	errno = 0;

	f = strtod(cmd,&pend);

	fgets(cmd,MAX_LEN,stdin);
	checkCmd(cmd , &pend);
	twoOpOneNum(pend);

	while(1)
	{	
		scanf("%s",cmd);
		if( checkIsNum(cmd, &pend, &f) == false)
			printErr(ERR_INVALID_PARAM);
	}
	
	
	readCmd();

	a.img = 34.2333;
	a.real = 22.22222;

	print_comp(a);

	printErr(ERR_UNDEF_COMP);
	return 0;
}


/*****************************************************/
/*****************************************************/

static void printErr(int err)
{
	printf(RED);

	switch (err)
	{
	case ERR_UNDEF_COMP:
		printf("Undefined complex variable\n");
		break;

	case ERR_UNDEF_CMD:
		printf("Undefined command name\n");
		break;
	
	case ERR_INVALID_PARAM:
		printf("Invalid paramter - not a number\n");
		break;

	case ERR_MIS_PARAM:
		printf("Missing paramter\n");
		break;

	case ERR_EXTRA_TXT:
		printf("Extraneous text after end of command\n");
		break;

	case ERR_MULTI_COMMA:
		printf("Multiple consecutive commas\n");
		break;

	case ERR_MIS_COMMA:
		printf("Missing comma\n");
		break;

	case ERR_ILLEGAL_COMMA:
		printf("Missing comma\n");
		break;


	default:
		printf("Error - Unknow erorr\n");
		break;
	}
	
	printf(RESET);
}


/**
 * @The function check if the string start with a operand 'A' -'G',
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the operand.
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsOp(char * str , char ** pEnd)
{
	/* remove white space */
	while (*str =='\t' || *str ==' ')
		str++;
	
	if (*str <= 'G' && *str>='A')
		{
			 /* if(str[1] == ' ' || str[1] == '\t' || str[1] == ',') */
			{
				*pEnd = (str+1);
				return true;
			}
				
		}

	return false;
}

/**
 * @The function check if the string start with a number,
 * 	and convert the string number to float.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the number.
 * @param fNum  return float pointer that containing the converted number
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsNum(__IN char * strIn , __OUT char ** pEnd ,__OUT float *fNum)
{
	

	/* convert string to float */
	*fNum = strtod(strIn,pEnd);

	/*check failure strtod*/
	if(*fNum == 0 && **pEnd == *strIn)
		return false;

	return true;
}

/**
 * @The function check if the string start with a comma char(',').
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the comma char.
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsComma(__IN char * strIn , __OUT char ** pEnd )
{
	*pEnd =  strIn;
	for (;**pEnd != ','; (*pEnd)++)
	{
		if(**pEnd != ' ' && **pEnd != '\t')
			return false;
	
	}

	(*pEnd)++;
	return true;	
}


/**
 * @The function check if the start of a string is legal command.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the command.
 * @return 		the int number of the command value
*/
static int checkCmd(__IN char * strIn , __OUT char ** pEnd)
{

	char* str = NULL;

	str = strtok(strIn," \t");

	if (str == NULL)
		return NOT_CMD;

	/*Set pEnd to start of the next token*/
	*pEnd = str;
	for (; **pEnd ; (*pEnd)++);

	(*pEnd)++; /*start of the next token after '\0'*/

	if(0 == strcmp("read_comp",str))
		return READ_COMP;

	else if (0 == strcmp("print_comp",str))
		return PRINT_COMP;

	else if (0 == strcmp("add_comp",str))
		return ADD_COMP;

	else if (0 == strcmp("sub_comp",str))
		return SUB_COMP;

	else if (0 == strcmp("mult_comp_real",str))
		return MULT_COMP_REAL;

	else if (0 == strcmp("mult_comp_img",str))
		return MULT_COMP_IMG;

	else if (0 == strcmp("mult_comp_comp",str))
		return MULT_COMP_COMP;

	else if (0 == strcmp("abs_comp",str))
		return ABS_COMP;

	else if (0 == strcmp("stop",str))
		return STOP;

	return NOT_CMD;

}

/**
 * @The function check if a string is end of command.
 * 	
 * @param strIn pointer to the start of string.
 */
static int checkIsEndOfString(__IN char *strIn)
{
	char * pEnd =  strIn;
	while (*pEnd++ != '\n')
	{
		if(!isspace(pEnd))
			return false;
	}

	return true;	
}
