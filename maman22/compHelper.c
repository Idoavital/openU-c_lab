#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compHelper.h"

 /*****************************************************/
				/*global variable*/
/*****************************************************/
extern STATE state;

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
	printf("You choose to finish the app\n");
	printf(GREEN"GoodBye !!!\n"RESET);
	exit(0);
}


/*****************************************************/
			/* functions definitions*/
/*****************************************************/

 void printErr(int err)
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
 int checkIsOp(char **strIn, int first )
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
 int checkIsNum(__IN char **strIn ,__OUT float *fNum)
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
 int checkIsComma(__IN char **strIn )
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
 int checkCmd(__IN char *strIn , __OUT char ** pEnd, __OUT int* read_fun_num)
{

	char* str = NULL;

	str = strtok(strIn," \r\t\n");

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
 int checkIsEndOfString(__IN char *strIn)
{
	/* remove white space */
	while (*strIn =='\t' || *strIn ==' ')
		strIn++;

	if (*strIn == '\n' || *strIn == '\0' || *strIn == '\r')
		return OK;	

	printErr(ERR_EXTRA_TXT);

	return FAIL;
}


/*****************************************************/
				/* state functoin */
/*****************************************************/
void stateReadCmd(stData* pData)
{
	char *pend = NULL; /*pointer to next stage in the input cmd string */
	char *cmd  = pData->str_cmd;
	int  *read_func_num = &pData->read_fun_num;

	/*ask from the user to input command*/
	fflush(stdin);
	printf("\nPlease enter command: \n");
	if(fgets(pData->str_cmd,MAX_LEN,stdin)==NULL)
	{
		printf(RED"[ERROR] END OF FILE !!!\nPlease exit with stop command\n"RESET);
		exit(1);
	}
	printf(YELLOW"You Enter: ");
	fputs(pData->str_cmd,stdout);
	printf(RESET);  				/*Reset color*/
	fflush(stdout);

	pData->cmd_fun_num = checkCmd(cmd , &pend,read_func_num);

	/*decide what is the next step after read the command*/
	if (pData->cmd_fun_num == CMD_ERR) 
		state = STATE_READ_CMD;
	else if (pData->cmd_fun_num == CMD_STOP)
	{
		state = STATE_DO_CMD;
	}
	else	
	{
		pData->str_cmd = pend;  /*the input string after the cmd*/
		state = STATE_PARSE_CMD;
	}
		
}
/*****************************************************/
/*****************************************************/
void stateParseCmd(stData* pData)
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

 void stateDoCmd(stData* pData)
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

	cf[funcNum](arrComp); /*exeute cmd*/

	state = STATE_READ_CMD;

}


/**
 *The function set a functions table.
 *
 *@param cf pointer to array of complex functions to be set.
 *@param pf pointer to array of parse read  functoins to be set.
 *@param sf pointer to array of parse state functions to be set.
 */
 void setFuncTable(comp_func * cf , parse_func *pf , state_func *sf)
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
