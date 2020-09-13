#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "mylib.h"

/* Grabs a string from a filestream, removes non alphanumeric characters,  
 * and returns the resulting word
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    /*skip to start of word*/
    while (!isalnum(c = getc(stream)) && EOF != c) 
        ;
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

/* Allocates memory (n bits) to a void pointer, and returns the result.
 * prints an error if this fails. 
 * @param n the amount of memory we need to malloc
 */
void* emalloc(size_t n) {
    void* result = malloc(n);
    if (result==NULL) {
        fprintf(stderr, "malloc failed.\n");
    }
    return result;
}
