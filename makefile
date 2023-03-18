JerryBoree: JerryBoreeMain.o Jerry.o HashTable.o LinkedList.o KeyValuePair.o MultiValueHashTable.o
	gcc JerryBoreeMain.o Jerry.o HashTable.o LinkedList.o KeyValuePair.o MultiValueHashTable.o -o JerryBoree

JerryBoreeMain.o: JerryBoreeMain.c Jerry.h Defs.h LinkedList.h HashTable.h MultiValueHashTable.h KeyValuePair.h Defs.h
	gcc -c JerryBoreeMain.c

Jerry.o:Jerry.h Defs.h
	gcc -c Jerry.c
	
KeyValuePair.o:KeyValuePair.h
	gcc -c KeyValuePair.c

LinkedList.o:LinkedList.h
	gcc -c LinkedList.c
	
HashTable.o:HashTable.h LinkedList.h KeyValuePair.h
	gcc -c HashTable.c

MultiValueHashTable.o:HashTable.h HashTable.h LinkedList.h KeyValuePair.h
	gcc -c MultiValueHashTable.c

clean :
	rm -f *.o
