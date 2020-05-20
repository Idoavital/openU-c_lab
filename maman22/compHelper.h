#ifndef __COMPHELPER_H__
#define __COMPHELPER_H__

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
typedef struct _stData
{
	comp_func*		fp_comp_func;
	parse_func*		fp_parse_func;
	complex** 		p_comp_global;
	stInputParam*	p_input_param;
	char*			str_cmd;
	int				cmd_fun_num;
	int				read_fun_num;

}stData;


 typedef void (*state_func)(stData* pData);

 /*****************************************************/
				/*wrapper fonctions*/
/*****************************************************/
/*All the wrapper functoin get array of complex numbers*/

void wrapper_read_comp (void * args);

void wrapper_print_comp (void * args);

void wrapper_add_comp (void * args);

void wrapper_sub_comp (void * args);

void wrapper_mult_comp_real (void * args);

void wrapper_mult_comp_img (void * args);

void wrapper_mult_comp_comp (void * args);

void wrapper_abs_comp (void * args);

void f_stop (void * args);


/*****************************************************/
				/* helper functions    */
/*****************************************************/

/**
 * @the function print error to stdout
 * @param err the error number
 */
 void printErr(int err);

/**
 * @The function check if the string start with a operand 'A' -'F',
 * @param strIn Double pointer to the start of string.
 * @param first true if the oprand should be the first oprand in the command.
 * @return 		OK if the string is find operand 'A' -'F',
*/
 int checkIsOp(char **strIn, int first );

/**
 * @The function check if the string start with a number,
 * 	and convert the string number to float.
 * @param strIn Double pointer to the start of string.
 * @param fNum  return float pointer that containing the converted number
 * @return 		OK if the function succeeded to converet the string to float, else return the char error, 
*/
 int checkIsNum(__IN char **strIn ,__OUT float *fNum);


/**
 * @The function check if the string start with a comma char(',').
 * @param strIn Double pointer to the start of string.
 * @return 		true if the function succeeded to converet the string to float, else return false .
*/
 int checkIsComma(__IN char **strIn );


/**
 * @The function check if the start of a string is legal command.
 * @param strIn pointer to the start of string.
 * @param pEnd  return pointer to first char after the command.
 * @param read_fun_num return the number of the read string function (the way that need to parse the string)
 * @return 		the int number of the command value
*/
 int checkCmd(__IN char * strIn , __OUT char ** pEnd, __OUT int* read_fun_num);

/**
 * @The function check if a string is end of command.
 * 	
 * @param strIn pointer to the start of string.
 */
 int checkIsEndOfString(__IN char *strIn);


/******************************************************************/
				/* read param functions */
/******************************************************************/
/**
 * @the function check if the string is from the format "cmd operand , number , number "
*/
 int readOneCompTwoNum(__IN char *str ,__OUT stInputParam* inParam) ;

/**
 * @the function check if the string is from the format "cmd operand , operand "
*/
 int readTWoComp(__IN char *str ,__OUT stInputParam* inParam);

/**
 * @the function check if the string is from the format "cmd operand "
*/
 int readOneComp(__IN char *str ,__OUT stInputParam* inParam) ;

/**
 * @the function check if the string is from the format "cmd operand , number "
*/
 int readOneCompOneNum(__IN char *str ,__OUT stInputParam* inParam) ;

/**
 * @the function check if the string is from the format "stop "
*/
 int readStop(__IN char *str ,__OUT stInputParam* inParam) ;

/*****************************************************/
				/* state functoin */
/*****************************************************/

/**
 *the function run When the state of the program is STATE_READ_CMD we 
 *
 *@param pData pointer to data struct that hold tha main data
 */
 void stateReadCmd(stData* pData);

/**
 *the function run When the state of the program is STATE_PARSE_CMD we 
 *
 *@param pData pointer to data struct that hold tha main data
 */
 void stateParseCmd(stData* pData);

/**
 *the function run When the state of the program is STATE_DO_CMD we 
 *
 *@param pData pointer to data struct that hold tha main data
 */
 void stateDoCmd(stData* pData);


/**
 *The function set a functions table.
 *
 *@param cf pointer to array of complex functions to be set.
 *@param pf pointer to array of parse read  functoins to be set.
 *@param sf pointer to array of parse state functions to be set.
 */
 void setFuncTable(comp_func * cf , parse_func *pf , state_func *sf);


 #endif /*__COMPHELPER_H__*/