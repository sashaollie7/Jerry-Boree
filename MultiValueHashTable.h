//
// Created by sasha on 12/24/22.
//

#ifndef C_ADT_MULTIVALUEHASHTABLE_H
#define C_ADT_MULTIVALUEHASHTABLE_H
#include "HashTable.h"
#include "LinkedList.h"

typedef struct multiValueHashTable_s *mvHash;

// a function which creates a multiValueHashTable object ,it also needs 2 equal functions ,the function returns NULL if there was a problem creating it
mvHash createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber, int hashNumber);
// a functions which destroys and frees all the memory it was using
status destroyMultiValueHashTable(mvHash mvTable);
// adds an element by a key to the multivalue hash table.
status addToMultiValueHashTable(mvHash mvTable,Element key,Element value);
// a function which receives a multiValueHashTable and a key and returns the linked list of items with this key
GenList lookupInMultiValueHashTable(mvHash mvTable,Element key);
// a function which receives a multiValueHashTable,key and a value and removes this value from the table
status removeFromMultiValueHashTable(mvHash mvTable,Element key,Element value);
// a function which receives a multiValueHashTable and a key and prints all the data with this key.
void displayMultiValueHashTableElementsByKey(mvHash mvTable,Element key);

#endif //C_ADT_MULTIVALUEHASHTABLE_H
