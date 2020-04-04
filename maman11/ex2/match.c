

#include <stdio.h>

/* The function return the number of char in the string
 * The end of the string is 'new line char' or 'null'
 */
static int strLen(char* str)
{
	int i = 0;
	for(i =0 ; str[i] != '\n' && str[i] != '\0'; i++) ;

	return i;
}

int f_match(char *text, char *pattern)
{
	int nTextLen 	 = 0;
	int nPatternLen  = 0;
	int nTextLoopEnd = 0;  /* number of times to run on the text loop */

	int nMatchIndex  = 0;
	int numOfCurrentDisMatches 	= 0;
	int numOfOldDisMatches 		= 0;

	int i = 0, j = 0;     /*loops indexes*/
	
	nPatternLen = strLen(pattern);
	nTextLen	= strLen(text);

	numOfOldDisMatches = nPatternLen ;  /* set to big number, we assume in the first time there is no match*/

	nTextLoopEnd = nTextLen - nPatternLen + 1;  /* number of times to run on the text loop */


	for(i =0 ; i < nTextLoopEnd ; i++)
	{
		numOfCurrentDisMatches = 0;  /* set dis match before loop */

		for(j =0 ; j < nPatternLen ; j++ )
		{
			if (pattern[j] != text[i+j])
			{
				numOfCurrentDisMatches++;
			}
		}

		/* set the match index if needed */
		if ( numOfOldDisMatches > numOfCurrentDisMatches )
		{
			numOfOldDisMatches = numOfCurrentDisMatches;
			nMatchIndex = i;
		}
	}
	
	return nMatchIndex;
}