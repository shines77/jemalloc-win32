//
// jemalloc_example.cpp
//

#include "jemalloc_example.h"
#include "jemalloc/jemalloc.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
    void *p;
    printf("Hello world!\n\n");

    // If you use jemalloc through the static lib,
    // must be manual initialize jemalloc first.
    je_init();

    p = (void *)je_malloc(128);
    if (p) {
        printf("malloc(%u) result ptr = 0x%016"PRIXPTR"\n\n", 128, p);
        je_free(p);
    }

    p = (void *)je_malloc(256);
    if (p) {
        printf("malloc(%u) result ptr = 0x%016"PRIXPTR"\n\n", 256, p);
        je_free(p);
    }

    // Unload the jemalloc
    je_uninit();

    system("pause");
	return 0;
}
