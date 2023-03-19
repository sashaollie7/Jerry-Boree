//
// Created by sasha on 12/24/22.
//
#include "MultiValueHashTable.h"
#include "KeyValuePair.h"

struct multiValueHashTable_s{
    hashTable hash; // using the hash table
    CopyFunction copyFuncKey;
    FreeFunction freeFuncKey;
    EqualFunction equalFuncKey;
    EqualFunction equalFuncValue;
    PrintFunction printFuncKey;
    CopyFunction copyFuncValue;
    FreeFunction freeFuncValue;
    PrintFunction printFuncValue;
    TransformIntoNumberFunction transformIntoNumber;
};


mvHash createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,EqualFunction equalValue,TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !equalKey || !copyValue || !freeValue || !printValue ||!transformIntoNumber||!equalValue)
        return NULL; // null check
    hashTable hash;
    mvHash mvTable = (mvHash)malloc(sizeof(struct multiValueHashTable_s));
    if(!mvTable) // check the memory allocation
        return NULL;
    hash = createHashTable(copyKey,freeKey,printKey,(CopyFunction )copyList,(FreeFunction) destroyList,(PrintFunction) displayList,equalKey,transformIntoNumber, hashNumber);
    if (!hash){ // check if successfully created the hash table
        free(mvTable);
        return NULL;
    }
    // initiate the function pointers
    mvTable->equalFuncKey=equalKey;
    mvTable->freeFuncKey=freeKey;
    mvTable->copyFuncKey=copyKey;
    mvTable->printFuncKey=printKey;
    mvTable->transformIntoNumber=transformIntoNumber;
    mvTable->freeFuncValue=freeValue;
    mvTable->copyFuncValue=copyValue;
    mvTable->equalFuncValue=equalValue;
    mvTable->printFuncValue=printValue;
    mvTable->hash=hash;
}

status destroyMultiValueHashTable(mvHash mvTable){
    if(!mvTable) // null check
        return nullFail;
    destroyHashTable(mvTable->hash);
    free(mvTable);
    return success;
}

status addToMultiValueHashTable(mvHash mvTable,Element key,Element value){
    if(!mvTable||!key||!value)
        return nullFail;
    GenList ls = lookupInHashTable(mvTable->hash,key); // search for the key in the table
    if (!ls){ // if is NULL then create a new list
        ls = createLinkedList(mvTable->copyFuncValue,mvTable->freeFuncValue,mvTable->printFuncValue,mvTable->equalFuncValue);
        if(!ls) // check if the list was created
            return memoryFail;
        status stat = appendNode(ls,value);
        if (stat!=success){ // check if successfully added the node
            destroyList(ls);
            return stat;
        }
        return addToHashTable(mvTable->hash,key,ls); // add the linked list with the key to the table and return the status
    }
    return appendNode(ls,value); // add the value to the list and return the status
}

GenList lookupInMultiValueHashTable(mvHash mvTable,Element key){
    if(!mvTable||!key)
        return NULL;
    return lookupInHashTable(mvTable->hash,key); // search if the ket exists it the hash table
}

status removeFromMultiValueHashTable(mvHash mvTable,Element key,Element value){
    if(!mvTable||!key||!value) // null check
        return nullFail;
    GenList ls = lookupInHashTable(mvTable->hash,key); // search for the key in the hash table
    if (!ls) // check if the list was created
        return wrongParameter;
    status stat = deleteNode(ls,value);
    if(stat!=success)
        return stat;
    if (getLengthList(ls)==0) // if it was the last node then delete all the list;
        removeFromHashTable(mvTable->hash,key);
    return success;
}

void displayMultiValueHashTableElementsByKey(mvHash mvTable,Element key){
    if(!mvTable||!key) // null check
        return;
    GenList ls = lookupInHashTable(mvTable->hash,key); // look for the list of values with the given key
    mvTable->printFuncKey(key); // print the key
    displayList(ls); // print all the elements in the list
}
