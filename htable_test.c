#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"

int main(int argc, char **argv) {
    
    int capacity = atoi(argv[1]);

    fprintf(stderr, "%d\n", capacity);

    htable h = htable_new(capacity);
    char word[256];

    while (getword(word, 256, stdin) != EOF) {
        htable_insert(h, word);
    }

    htable_print(h, stdout);
    htable_free(h);

    return EXIT_SUCCESS;
}
