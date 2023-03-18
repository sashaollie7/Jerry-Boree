//
// Created by sasha on 12/18/22.
//

#ifndef C_ADT_KEYVALUEPAIR_H
#define C_ADT_KEYVALUEPAIR_H
#include "Defs.h"

typedef struct KeyValuePair *Pair;

// create the KeyValuePair an object like a tuple, need to send it the listed functions. the function returns NULL if there was a problem creating it
Pair createKeyValuePair(CopyFunction copyKey,FreeFunction freeKey,PrintFunction printKey,EqualFunction equalKey,CopyFunction copyValue,FreeFunction freeValue,PrintFunction printValue,Element key,Element value);
// delete the object and the elements it had ,also frees all the memory it was using
status destroyKeyValuePair(Pair pr);
// print the value of the pair
status displayValue(Pair pr);
// prints the key of the pair
void displayKey(Pair pr);
Element getValue(Pair pr);
Element getKey(Pair pr);
// checks if the key of the pair and the key it received are the same
bool isEqualKey(Pair pair,Element elem);
// shallow copy the pair (returns the same pointer)
Pair copyKeyValuePair(Pair pair);

#endif //C_ADT_KEYVALUEPAIR_H
