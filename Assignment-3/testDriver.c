#include "mymalloc.h"

#include <stdio.h>

int main()
{
    printf("\nALLOCATIG MEMORY");
	void *a=my_buddy_malloc(23);
	void *b=my_buddy_malloc(7901);
	void *c=my_buddy_malloc(904);
	void *d=my_buddy_malloc(42);
    void *e=my_buddy_malloc(327);
    void *f=my_buddy_malloc(182);
    void *g=my_buddy_malloc(71);
    void *h=my_buddy_malloc(4);

	my_free(a);	
	my_free(b);
	my_free(c);
	my_free(d);
    my_free(e);
    my_free(f);
    my_free(g);
    my_free(h);
    printf("\nFreeing memory!");
}
