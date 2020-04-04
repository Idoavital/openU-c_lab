


#include <stdio.h>
#include "sequence.h"

#define LOG_S "The stirng \"%s\"\tis - " 

enum SEQUENCE
{
	UP, DOWN, REAL_UP, REAL_DOWN, NOT_CONST
};

/**The function get a string and check the sequence of the string
 * and print the result on the screen 		*/
void f_sequence(char str[])
{

	int  fOrder	= 0;   		/* a flag to save the order of the string */
	int	 i 		= 0;  		/* for loop index */

	/* If input is null or only one char */
	if (str[0] == '\0' || str[1] == '\0')
	{ 
		printf("Not long string\n");
		return;
	}

	/* For the first char - Determine the sequence order */
	if (str[1] >= str[0])
		fOrder = REAL_UP;

	else if (str[1] < str[0])
		fOrder = REAL_DOWN;

	/*For loop run over the string.
	  we check every char relative to is char before him,
	  and update the fOrder flag according to the new state */
	for  ( i = 2 ; str[i] != '\0' && str[i] != '\n' ; i++)
	{
		if (str[i] > str[i-1])
		{
			if (fOrder != REAL_UP && fOrder != UP)
			{
				fOrder = NOT_CONST;
				break;
			}

		}

		else if (str[i] == str[i-1])
		{
			if (fOrder == REAL_UP || fOrder == UP)
				fOrder = UP;
			else
				fOrder = DOWN;
		}

		else if (str[i] < str[i-1])
		{
			if (fOrder != REAL_DOWN && fOrder != DOWN)
			{
				fOrder = NOT_CONST;
				break;
			}
		}

	}

/* Print sequnece order answer */

	switch (fOrder)
	{
	case UP:
		printf(LOG_S"UP OREDER\n",str);
		break;
	case REAL_UP:
		printf(LOG_S"REAL UP OREDER\n",str);
		break;
	case DOWN:
		printf(LOG_S"DOWN OREDER\n",str);
		break;
	case REAL_DOWN:
		printf(LOG_S"REAL DOWN OREDER\n",str);
		break;
	default:
		printf(LOG_S"NOT CONST OREDER\n",str);
		break;
	}
}


