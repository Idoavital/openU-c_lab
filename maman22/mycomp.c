#include <stdio.h>
#include <string.h>

#include "complex.h"

#define MAX_LEN 256
#define	true	1
#define false	0

enum ERROR {
	UNDEF_COMP,
	UNDEF_CMD,
	INVALID_PARAM,
	MIS_PARAM,
	EXTRA_TXT,
	MULTI_COMMA,
	MIS_COMMA,
	ILLEGAL_COMMA,
	UNDEF_ERROR

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

static int checkIsOp(char * str)
{
	if (*str <= 'G' && *str>='A')
		return true;

	return false;
}

static checkIsNum(char *)
{
	
}
static int findComma(char * str)
{
	while (*str != ' ' || *str != '\t')
	{
		str++;
	}
	if (*str == ',')
		return true;
	
	return false;
}

static int checkCmd(char *str)
{
	
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

static void readCmd()
{
	char *pTocken;
	char cmd[MAX_LEN];
	char Token_1[MAX_LEN];
	char Token_2[MAX_LEN];
	char Token_3[MAX_LEN];

	printf("Enter command\n");
	fgets(cmd,MAX_LEN,stdin);
	pTocken = strtok(cmd,", \t,");
	pTocken = strtok(NULL," \t,");
	pTocken = strtok(NULL,", \t,");

	
}
/*****************************************************/
/*****************************************************/

int main(int argc, char const *argv[])
{
	char cmd[MAX_LEN];
	char Token_1[MAX_LEN];
	char Token_2[MAX_LEN];
	char Token_3[MAX_LEN];

	readCmd();
	complex a;
	a.img = 34.2333;
	a.real = 22.22222;

	print_comp(a);

	printErr(UNDEF_COMP);
	return 0;
}


/*****************************************************/
/*****************************************************/

static void printErr(int err)
{
	printf(RED);

	switch (err)
	{
	case UNDEF_COMP:
		printf("Undefined complex variable\n");
		break;

	case UNDEF_CMD:
		printf("Undefined command name\n");
		break;
	
	case INVALID_PARAM:
		printf("Invalid paramter - not a number\n");
		break;

	case MIS_PARAM:
		printf("Missing paramter\n");
		break;

	case EXTRA_TXT:
		printf("Extraneous text after end of command\n");
		break;

	case MULTI_COMMA:
		printf("Multiple consecutive commas\n");
		break;

	case MIS_COMMA:
		printf("Missing comma\n");
		break;

	case ILLEGAL_COMMA:
		printf("Missing comma\n");
		break;


	default:
		printf("Error - Unknow erorr\n");
		break;
	}
	
	printf(RESET);
}