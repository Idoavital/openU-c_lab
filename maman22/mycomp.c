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
	CMD_ERR
}CMD;

typedef enum _PARSE {
	PARSE_1COMP,
	PARSE_2COMP,
	PARSE_1COMP1NUM,
	PARSE_1COMP2NUM,
	PARSE_STOP,
	PARSE_SIZE
}PARSE;


typedef enum _STATE{
	STATE_READ_CMD,
	STATE_PARSE_CMD,
	STATE_DO_CMD,
	STATE_STOP,
	STATE_SIZE
}STATE;

typedef struct _InputParam{
	int		chFirst;       /*the first operand char (comp var)*/
	int 	chSecond;
	float	fNumFirst;		/*the firat float number*/
	float	fNumSecond;
}stInputParam;

typedef void (*comp_func)(void *); /*type of wrapper functions pointer (to wrapp the command functions)*/
typedef int (*parse_func)(__IN char *str ,__OUT stInputParam* );

/*sturct for the data between functions*/
typedef struct _stDataP
{
	comp_func*		fp_comp_func;
	parse_func*		fp_parse_func;
	complex** 		p_comp_global;
	stInputParam*	p_input_param;
	char*			str_cmd;
	int				cmd_fun_num;
	int				read_fun_num;

}stDataP;


 typedef void (*state_func)(stDataP* pData);
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
 * @param read_fun_num return the number of the read string function (the way that need to parse the string)
 * @return 		the int number of the command value
*/
static int checkCmd(__IN char * strIn , __OUT char ** pEnd, __OUT int* read_fun_num);

/**
 * @The function check if a string is end of command.
 * 	
 * @param strIn pointer to the start of string.
 */
static int checkIsEndOfString(__IN char *strIn);


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
static int readOneCompTwoNum(__IN char *str ,__OUT stInputParam* inParam) 
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
static int readTWoComp(__IN char *str ,__OUT stInputParam* inParam)
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
static int readOneComp(__IN char *str ,__OUT stInputParam* inParam) 
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
static int readOneCompOneNum(__IN char *str ,__OUT stInputParam* inParam) 
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
static int readStop(__IN char *str ,__OUT stInputParam* inParam) 
{

	if (checkIsEndOfString(str)  == FAIL)
		return false;
	
	return true;
}

/*****************************************************/
				/* state functoin */
/*****************************************************/
static void stateReadCmd(stDataP* pData)
{
	char *pend = NULL; /*pointer to next stage in the input cmd string */
	char *cmd  = pData->str_cmd;
	int  *read_func_num = &pData->read_fun_num;

	/*ask from the user to input command*/
	printf("\nPlease enter command: ");

	fgets(pData->str_cmd,MAX_LEN,stdin);

	/*decide what is the next step after read the command*/
	if ((pData->cmd_fun_num = checkCmd(cmd , &pend,read_func_num)) == CMD_ERR) 
		state = STATE_READ_CMD;
	else
	{
		pData->str_cmd = pend;
		state = STATE_PARSE_CMD;
	}
		
}

static void stateParseCmd(stDataP* pData)
{

	char* str      = pData->str_cmd;
	parse_func *pf = pData->fp_parse_func;

	stInputParam* in_params = pData->p_input_param;

	if (pf[pData->read_fun_num](str,in_params))
	{
		if (pData->cmd_fun_num == CMD_STOP)
			state = STATE_STOP;
		else
			state = STATE_DO_CMD;
	}
	else
		state = STATE_READ_CMD;
	
}

static void stateDoCmd(stDataP* pData)
{
	stInputParam* inParam  = pData->p_input_param;
	complex** pGlobal_Comp = pData->p_comp_global;
	comp_func* cf		   = pData->fp_comp_func;
	complex* arrComp[2];
	int funcNum			   = pData->cmd_fun_num;
	
	switch (pData->read_fun_num)
	{
	case PARSE_1COMP:
		arrComp[0] = pGlobal_Comp[inParam->chFirst];
		break;
	case PARSE_2COMP:
		arrComp[0] = pGlobal_Comp[inParam->chFirst];
		arrComp[1] = pGlobal_Comp[inParam->chSecond];
		break;

	case PARSE_1COMP1NUM:
	case PARSE_1COMP2NUM:
		arrComp[0] = pGlobal_Comp[inParam->chFirst];
		arrComp[1] = (complex*)&inParam->fNumFirst;
		break;
	
	default:
		break;
	}

	cf[funcNum](arrComp);

	state = STATE_READ_CMD;

}


/**
 *The function set a functions table.
 *
 *@param cf pointer to array of complex functions to be set.
 *@param pf pointer to array of parse read  functoins to be set.
 *@param sf pointer to array of parse state functions to be set.
 */
static void setFuncTable(comp_func * cf , parse_func *pf , state_func *sf)
{
	cf[CMD_READ_COMP] 		= wrapper_read_comp;
	cf[CMD_PRINT_COMP] 		= wrapper_print_comp;
	cf[CMD_ADD_COMP] 		= wrapper_add_comp;
	cf[CMD_SUB_COMP] 		= wrapper_sub_comp;
	cf[CMD_MULT_COMP_REAL] 	= wrapper_mult_comp_real;
	cf[CMD_MULT_COMP_IMG] 	= wrapper_mult_comp_img;
	cf[CMD_MULT_COMP_COMP] 	= wrapper_mult_comp_comp;
	cf[CMD_ABS_COMP] 		= wrapper_abs_comp;
	cf[CMD_STOP] 			= f_stop;

	pf[PARSE_1COMP]		= readOneComp;
	pf[PARSE_2COMP]		= readTWoComp;
	pf[PARSE_1COMP1NUM]	= readOneCompOneNum;
	pf[PARSE_1COMP2NUM]	= readOneCompTwoNum;
	pf[PARSE_STOP]		= readStop;

	sf[STATE_READ_CMD]	= stateReadCmd;
	sf[STATE_PARSE_CMD]	= stateParseCmd;
	sf[STATE_DO_CMD]	= stateDoCmd;

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
	stDataP		  stData;
	char 		  cmd[MAX_LEN];

	/*TODO: set stData*/
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

	else if (**strIn == '\n' || **strIn == '\0'|| **strIn == EOF)
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

	if (**strIn =='\n' || **strIn == EOF || **strIn == '\0')
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
 * The function check if the start of a string is legal command.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the command.
 * @param read_fun_num return the number of the read string function (the way that need to parse the string)
 * @return 		the int number of the command value
*/
static int checkCmd(__IN char *strIn , __OUT char ** pEnd, __OUT int* read_fun_num)
{

	char* str = NULL;

	str = strtok(strIn," \t\n");

	if (str == NULL)
		return CMD_ERR;

	/*Set pEnd to start of the next token*/
	*pEnd = str;
	for (; **pEnd ; (*pEnd)++);

	(*pEnd)++; /*start of the next token after '\0'*/

	if(0 == strcmp("read_comp",str))
	{
		*read_fun_num = PARSE_1COMP2NUM;
		return CMD_READ_COMP;
	}

	else if (0 == strcmp("print_comp",str))
	{
		*read_fun_num = PARSE_1COMP;
		return CMD_PRINT_COMP;
	}

	else if (0 == strcmp("add_comp",str))
	{
		*read_fun_num = PARSE_2COMP;
		return CMD_ADD_COMP;
	}
		

	else if (0 == strcmp("sub_comp",str))
	{
		*read_fun_num = PARSE_2COMP;
		return CMD_SUB_COMP;
	}
		

	else if (0 == strcmp("mult_comp_real",str))
	{
		*read_fun_num = PARSE_1COMP1NUM;
		return CMD_MULT_COMP_REAL;
	}
		

	else if (0 == strcmp("mult_comp_img",str))
	{
		*read_fun_num = PARSE_1COMP1NUM;
		return CMD_MULT_COMP_IMG;
	}
		

	else if (0 == strcmp("mult_comp_comp",str))
	{
		*read_fun_num = PARSE_2COMP;
		return CMD_MULT_COMP_COMP;
	}
		

	else if (0 == strcmp("abs_comp",str))
	{
		*read_fun_num = PARSE_1COMP;
		return CMD_ABS_COMP;
	}
		

	else if (0 == strcmp("stop",str))
	{
		*read_fun_num = PARSE_STOP;
		return CMD_STOP;
	}
		
	printErr(ERR_UNDEF_CMD);
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

	if (*strIn == '\n' || *strIn == '\0')
		return OK;	

	printErr(ERR_EXTRA_TXT);

	return FAIL;
}
