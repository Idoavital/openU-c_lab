#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <ctype.h> /* isspace */
#include <stdlib.h> /* strtod */


#include "complex.h"

/*****************************************************/
					/* define */
/*****************************************************/

#define MAX_LEN 256
#define	true	1
#define false	0
#define FOREVER for(;;)
#define __IN		/* varaible get in to function */
#define	__OUT		/* varaible returend from the function */
#define COMP_VAR_NUM  6
#define COMP_FUNC_NUM 9

/*****************************************************/
				/*typedef definition*/
/*****************************************************/

typedef enum _OK {
	OK,
	FAIL,
	IS_COMMA,
	IS_NEWLINE
}_OK;

typedef enum _ERROR {
	ERR_UNDEF_COMP,
	ERR_UNDEF_CMD,
	ERR_INVALID_PARAM,
	ERR_MIS_PARAM,
	ERR_EXTRA_TXT,
	ERR_MULTI_COMMA,
	ERR_MIS_COMMA,
	ERR_ILLEGAL_COMMA,
	ERR_UNDEF_ERROR

}ERROR;

typedef enum _CMD {
	CMD_READ_COMP,
	CMD_PRINT_COMP,
	CMD_ADD_COMP,
	CMD_SUB_COMP,
	CMD_MULT_COMP_REAL,
	CMD_MULT_COMP_IMG,
	CMD_MULT_COMP_COMP,
	CMD_ABS_COMP,
	CMD_STOP,
	CMD_ERR,
}CMD;

typedef enum _STATE{
	STATE_READ_CMD,
	STATE_PARSE_CMD,
	STATE_DO_CMD,
	STATE_STOP
}STATE;

typedef struct _InputParam{
	int		chFirst;       /*the first operand char*/
	int 	chSecond;
	float	fNumFirst;		/*the firat float number*/
	float	fNumSecond;
}stInputParam;

typedef struct _stDataP
{
	comp_func*		P_func_table
	complex** 		p_comp_global;
	complex** 		p_comp_temp;
	stInputParam**	p_input_param;
	char*			str_cmd;
	int				function_num;

}stDataP;

typedef void (*comp_func)(void *); /*type of wrapper functions pointer (to wrapp the command functions)*/

/*****************************************************/
				/*global variable*/
/*****************************************************/
complex A,B,C,D,E,F;
STATE state;


/*****************************************************/
				/*wrapper fonctions*/
/*****************************************************/
/*All the wrapper functoin get array of complex numbers*/

void wrapper_read_comp (void * args)
{
	complex **pComp = (complex**) args;

	read_comp(pComp[0],pComp[1]->real, pComp[1]->img);
}

void wrapper_print_comp (void * args)
{
	complex **pComp = (complex**) args;

	print_comp(*pComp[0]);
}

void wrapper_add_comp (void * args)
{
	complex **pComp = (complex**) args;

	add_comp(*pComp[0],*pComp[1]);
}

void wrapper_sub_comp (void * args)
{
	complex **pComp = (complex**) args;

	sub_comp(*pComp[0],*pComp[1]);
}

void wrapper_mult_comp_real (void * args)
{
	complex **pComp = (complex**) args;

	mult_comp_real(*pComp[0],pComp[1]->real);
}

void wrapper_mult_comp_img (void * args)
{
	complex **pComp = (complex**) args;

	/*the real value is actually refer to the first input param (in this case is img)*/
	mult_comp_real(*pComp[0],pComp[1]->real); 
}

void wrapper_mult_comp_comp (void * args)
{
	complex **pComp = (complex**) args;

	mult_comp_comp(*pComp[0],*pComp[1]);
}

void wrapper_abs_comp (void * args)
{
	complex **pComp = (complex**) args;

	abs_comp(*pComp[0]);
}

void f_stop (void * args)
{
	/*TODO: EXIT CLEAN*/
}


/*****************************************************/
				/* forward declaration */
				/* static functions    */
/*****************************************************/

/**
 * @the function print error to stdout
 * @param err the error number
 */
static void printErr(int err);

/**
 * @The function check if the string start with a operand 'A' -'F',
 * @param strIn Double pointer to the start of string.
 * @param first true if the oprand should be the first oprand in the command.
 * @return 		OK if the string is find operand 'A' -'F',
*/
static int checkIsOp(char **strIn, int first );

/**
 * @The function check if the string start with a number,
 * 	and convert the string number to float.
 * @param strIn Double pointer to the start of string.
 * @param fNum  return float pointer that containing the converted number
 * @return 		OK if the function succeeded to converet the string to float, else return the char error, 
*/
static int checkIsNum(__IN char **strIn ,__OUT float *fNum);


/**
 * @The function check if the string start with a comma char(',').
 * @param strIn Double pointer to the start of string.
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
static int checkIsComma(__IN char **strIn );


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

/**
 *@The function set a functions table.
 *
 *@param cf pointer to array of functoins to be set.
 */
static void setFuncTable(comp_func * cf)
{
	cf[READ_COMP] 		= wrapper_read_comp;
	cf[PRINT_COMP] 		= wrapper_print_comp;
	cf[ADD_COMP] 		= wrapper_abs_comp;
	cf[SUB_COMP] 		= wrapper_sub_comp;
	cf[MULT_COMP_REAL] 	= wrapper_mult_comp_real;
	cf[MULT_COMP_IMG] 	= wrapper_mult_comp_img;
	cf[MULT_COMP_COMP] 	= wrapper_mult_comp_comp;
	cf[ABS_COMP] 		= wrapper_abs_comp;
	cf[STOP] 			= f_stop;

}

static void setArrComplex(complex* pComp[COMP_VAR_NUM])
{
	pComp[0] = &A;
	pComp[1] = &B;
	pComp[2] = &C;
	pComp[3] = &D;
	pComp[4] = &E;
	pComp[5] = &F;
}
/******************************************************************/
				/* read param functions */
/******************************************************************/
/**
 * @the function check if the string is from the format "cmd operand , number , number "
*/
static void readOneOpTwoNum(__IN char *str ,__OUT stInputParam* inParam) 
{

	/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chFirst = *str-'A';
		str++;
	}
	else 
		return;
	
	/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return;

	/* check and read Is float number else print error */
	if (checkIsNum(&str,&inParam->fNumFirst) == FAIL)
		return;
	
	/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return;


	/* check and read Is float number else print error */
	if (checkIsNum(&str,&inParam->fNumSecond) == FAIL)
		return;

	if (checkIsEndOfString(str)  == OK)
	{
		state = STATE_DO_CMD;
	}
	else 
		return;

}

/**
 * @the function check if the string is from the format "cmd operand , operand "
*/
static void readTWoOp(__IN char *str ,__OUT stInputParam* inParam)
{
	/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chFirst = *str-'A';
		str++;
	}
	else 
		return;
	
		/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return;

		/* check and read if is oprand else print error*/
	if (checkIsOp(&str,false ) == OK)
	{
		inParam->chSecond = *str-'A';
		str++;
	}
	else 
		return;

	if (checkIsEndOfString(str)  == OK)
	{
		state = STATE_DO_CMD;
	}
	else 
		return;

}

/**
 * @the function check if the string is from the format "cmd operand "
*/
static void readOneOp(__IN char *str ,__OUT stInputParam* inParam) 
{
	/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chSecond = *str-'A';
		str++;
	}
	else 
		return;

	if (checkIsEndOfString(str)  == OK)
	{
		state = STATE_DO_CMD;
	}
	else 
		return;
}

/**
 * @the function check if the string is from the format "cmd operand , number "
*/
static void readOneOpOneNum(__IN char *str ,__OUT stInputParam* inParam) 
{
		/* check and read if is oprand else print error*/
	if (checkIsOp(&str,true ) == OK)
	{
		inParam->chSecond = *str-'A';
		str++;
	}
	else 
		return;
	
	/* check  Is Comma else print error */
	if (checkIsComma(&str) == FAIL)
		return;

	/* check and read Is float number else print error */
	if (checkIsNum(&str,&inParam->fNumFirst) == FAIL)
		return;
	
	if (checkIsEndOfString(str)  == OK)
	{
		state = STATE_DO_CMD;
	}
	else 
		return;

}

static void stateReadCmd(stDataP* pData)
{
	/*ask from the user to input command*/
	printf("\nPlease enter command: ");

	fgets(pData->str_cmd,MAX_LEN,stdin);

	/*decide what is the next step after read the command*/
	if ((pData->function_num = checkCmd(cmd , &pend)) == CMD_ERR) 
		state = STATE_READ_CMD;
	else
		state = STATE_PARSE_CMD;

}

static void stateParseCmd(stDataP* pData)
{
	/*TODO: SET VARIABLES*/
	int funcNum    		= pData->function_num;
	comp_func func 		= pData->P_func_table[funcNum];
	stInputParam* params = pData->p_input_param;

	func(params);

}
/*****************************************************/
				/* MAIN */
/*****************************************************/

int main(int argc, char const *argv[])
{

	complex*	  arrCompVar[COMP_VAR_NUM]; 
	complex*      arrtempcomp[2];
	stInputParam  inParam;				/* the variable from stdin */
	comp_func 	  arrCompFunc[COMP_FUNC_NUM];
	char 		  cmd[MAX_LEN];
	char*		  pend;
	int 		  funcNum;

	/*TODO: set stData*/
	/*set the complex array to variable*/
	setArrComplex(arrCompVar);

	/*set the function table */
	setFuncTable(arrCompFunc);


	readOneOpTwoNum(pend, &inParam) ;

	/*set params*/
	arrtempcomp[0] = arrCompVar[inParam.chFirst];
	arrtempcomp[1] = (complex*)&inParam.fNumFirst;


	arrCompFunc[funcNum]((void*)arrtempcomp);
	


FOREVER
{
	fgets(cmd,MAX_LEN,stdin);
	checkCmd(cmd , &pend);
	readOneOpTwoNum(pend, &inParam) ;

	fgets(cmd,MAX_LEN,stdin);
	checkCmd(cmd , &pend);
	readTWoOp(pend ,&inParam) ;

	fgets(cmd,MAX_LEN,stdin);
	checkCmd(cmd , &pend);
	readOneOp(pend ,&inParam) ;

}


	return 0;
}


/*****************************************************/
			/*static functions definitions*/
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
		printf("Illegal comma\n");
		break;


	default:
		printf("Error - Unknow erorr\n");
		break;
	}
	
	printf(RESET"\n");
}


/**
 * @The function check if the string start with a operand 'A' -'F',
 * @param strIn Double pointer to the start of string.
 * @param first true if the oprand should be the first oprand in the command.
 * @return 		OK if the string is find operand 'A' -'F',
*/
static int checkIsOp(char **strIn, int first )
{
	/* remove white space */
	while (**strIn =='\t' || **strIn ==' ')
		(*strIn)++;
	
	if (**strIn <= 'F' && **strIn>='A')
		return OK;

	else if (**strIn == ',' && first)
	{
		printErr(ERR_ILLEGAL_COMMA);
		return FAIL;
	}

	else if (**strIn == ',')
	{
		printErr(ERR_MULTI_COMMA);
		return FAIL;
	}

	else if (**strIn == '\n')
	{
		printErr(ERR_MIS_PARAM);
		return FAIL;
	}

	printErr(ERR_UNDEF_COMP);

	return FAIL;
}

/**
 * @The function check if the string start with a number,
 * 	and convert the string number to float.
 * @param strIn Double pointer to the start of string.
 * @param fNum  return float pointer that containing the converted number
 * @return 		OK if the function succeeded to converet the string to float, else return the char error, 
*/
static int checkIsNum(__IN char **strIn ,__OUT float *fNum)
{
	char * pEnd = NULL;
	
	/* remove white space */
	while (**strIn =='\t' || **strIn ==' ')
		(*strIn)++;
	
	if (**strIn ==',')
	{		
		printErr(ERR_MULTI_COMMA);
		return FAIL;
	}

	if (**strIn =='\n')
	{
		printErr(ERR_MIS_PARAM);
		return FAIL;
	}
	/* convert string to float */
	*fNum = strtod(*strIn ,&pEnd);

	/*check failure strtod*/
	if(*fNum == 0 && *pEnd == **strIn)
	{
		printErr(ERR_INVALID_PARAM);
		return FAIL;
	}

	*strIn = pEnd;

	return OK;
}

/**
 * @The function check if the string start with a comma char(',').
 * @param strIn Double pointer to the start of string.
 * @return 		OK if the char is a comma.
*/
static int checkIsComma(__IN char **strIn )
{

	/* remove white space */
	while (**strIn =='\t' || **strIn ==' ')
		(*strIn)++;


	if (**strIn == '\n')
	{
		printErr(ERR_MIS_PARAM);
		return FAIL;			
	}

	if (**strIn != ',')
	{
		printErr(ERR_MIS_COMMA);
		return FAIL;
	}

	(*strIn)++;

	return OK;	
}


/**
 * @The function check if the start of a string is legal command.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the command.
 * @return 		the int number of the command value
*/
static int checkCmd(__IN char *strIn , __OUT char ** pEnd)
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
		return CMD_READ_COMP;

	else if (0 == strcmp("print_comp",str))
		return CMD_PRINT_COMP;

	else if (0 == strcmp("add_comp",str))
		return CMD_ADD_COMP;

	else if (0 == strcmp("sub_comp",str))
		return CMD_SUB_COMP;

	else if (0 == strcmp("mult_comp_real",str))
		return CMD_MULT_COMP_REAL;

	else if (0 == strcmp("mult_comp_img",str))
		return CMD_MULT_COMP_IMG;

	else if (0 == strcmp("mult_comp_comp",str))
		return CMD_MULT_COMP_COMP;

	else if (0 == strcmp("abs_comp",str))
		return CMD_ABS_COMP;

	else if (0 == strcmp("stop",str))
		return CMD_STOP;

	return CMD_ERR;

}

/**
 * @The function check if a string is end of command.
 * 	
 * @param strIn pointer to the start of string.
 */
static int checkIsEndOfString(__IN char *strIn)
{
	/* remove white space */
	while (*strIn =='\t' || *strIn ==' ')
		strIn++;

	if (*strIn == '\n' || *strIn == '\n')
		return OK;	

	printErr(ERR_EXTRA_TXT);

	return FAIL;
}
