#ifndef MATCH_H
#define MATCH_H

#define MAX_LEN 256		/*Max stirng chars*/

/** The fuction find the index with the best match of the pattern in the text
 * @param text the text string 
 * @param pattern the pattern string
 * @return the index in the text string, of the best match of the pattern in the text
 * 
*/
int f_match(char *text, char *pattern);


#endif