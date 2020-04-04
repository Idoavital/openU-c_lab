#include <stdio.h>
#include <string.h>

#include "complex.h"

#define MAX_LEN 256

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

/*****************************************************/
/*****************************************************/
/*forward declaration*/

static void printErr(int err);

/*****************************************************/
/*****************************************************/

int main(int argc, char const *argv[])
{
	char cmd[MAX_LEN];
	char Token_1[MAX_LEN];
	char Token_2[MAX_LEN];
	char Token_3[MAX_LEN];


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