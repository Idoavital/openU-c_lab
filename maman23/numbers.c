
/********************************************************************************************************	
* numbers : Program to 								*	
*                                                     													*	
* Author:  Ido Abitbul			                      													*
*                                                     													*
* Usage :
*/

#include <stdio.h>

/**************************************************************/

/**************************************************************/
#define MAX_ARGC 3
#define TEN_DIGIT(a) ((a)%10)


enum eError {OK , ERROR};
/**************************************************************/

/**************************************************************/

int err_max_args(void );  		/* Print error when too many args */
int err_no_file(char* name);	/* Print error when no file with this name or can't open */
void convert_num_to_words(FILE *file_in ,FILE *file_out);

/**************************************************************/

/**************************************************************/
int main(int argc , char * argv[])
{
	FILE *file_in, *file_out;

	/*check if too many args*/
	if(argc > MAX_ARGC)
		return err_max_args();

	/* open a file to read from else read from stdin */
	file_in = argc > 1 ? fopen(argv[1],"r") : stdin ;
	if (file_in == NULL)
		return err_no_file(argv[1]);

	/* open a file to write from else write to stdout */
	file_in = argc > 1 ? fopen(argv[2],"w") : stdout ;
	if (file_in == NULL)
		return err_no_file(argv[2]);
	
}

/**************************************************************/

/**************************************************************/
int err_max_args(void )
{
	fprintf(stderr,"[ERROR] too many args \n");
	return ERROR;
}
/**************************************************************/

/**************************************************************/
int err_no_file(char* name)
{
	fprintf(stderr,"[ERROR] can't open the file: %s or the file is exists \n",name);
	return ERROR;
}
/**************************************************************/

/**************************************************************/
void convert_num_to_words(FILE *fd_in ,FILE *fd_out)
{

    char *single_digits[] = { "zero", "one", "two","three", "four" ,"five", "six","seven", "eight", "nine"}; 

    char *two_digits[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"}; 
  
    /* The first two string are not used, they are to make array indexing simple*/
    char *tens_multiple[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"}; 

	int Num;
	int len;

	while(fscanf(fd_in,"%d",&Num))
	{

		if(Num  < 10)
		{
			fprintf(fd_out,"%s \n",single_digits[Num]);
		}
		else if (Num >= 10 && Num < 20)
		{
			fprintf(fd_out,"%s \n",two_digits[Num%10]);
		}
		else if (Num >= 20)
		{
			fprintf(fd_out,"%s \n",tens_multiple[Num/10]);
			fprintf(fd_out,"%s \n",single_digits[Num%10]);
		}
		
	}
}