malloctest: mymalloc.o mallocdrv.o
	gcc -w -o malloctest mymalloc.o mallocdrv.o
	
mymalloc.o: mymalloc.c mymalloc.h
	gcc -w -c mymalloc.c
	
mallocdrv.o: mallocdrv.o mymalloc.h
	gcc -w -c mallocdrv.c
	
clean:
	rm -f *.o malloctest