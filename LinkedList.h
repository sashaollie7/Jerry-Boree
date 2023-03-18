//
// Created by sasha on 12/17/22.
//

#ifndef C_ADT_LINKEDLIST_H
#define C_ADT_LINKEDLIST_H
#include "Defs.h"

typedef struct t_LinkedList *GenList;


GenList createLinkedList(CopyFunction copyF,FreeFunction freeF,PrintFunction printF,EqualFunction equalF); // create the list return NULL if something failed
status destroyList(GenList ls); //destroy the list returns the status of the operation
status appendNode(GenList ls,Element elem); // add a node to the tail of the list
status deleteNode(GenList ls,Element elem); // delete a node by value
status displayList(GenList ls); // print all the elements of the list
Element getDataByIndex(GenList ls,int idx); // return the value of the node with the given index
int getLengthList(GenList ls);
Element searchByKeyInList(GenList ls, Element elem); // search an element with the given key
GenList copyList(GenList ls); // does a shallow copy of the list

#endif //C_ADT_LINKEDLIST_H
