//
// Created by sasha on 12/18/22.
//
#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s{
    CopyFunction copyFuncKey;
    FreeFunction freeFuncKey;
    EqualFunction equalFuncKey;
    PrintFunction printFuncKey;
    CopyFunction copyFuncValue;
    FreeFunction freeFuncValue;
    PrintFunction printFuncValue;
    TransformIntoNumberFunction transformIntoNumber;
    GenList *elements;  // an array of linked lists
    int hashNumber; // the size of the array
};

int hashIndex(hashTable hash,Element elem){
    /// a function which receives a key and transform him to an index of the linked list array
    if (!elem||!hash)
        return -1;
    int i = hash->transformIntoNumber(elem); // use the transform function from the user on the key
    return i%(hash->hashNumber); // module the size of the array
}

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber){
    if(!copyKey||!freeKey||!printKey||!equalKey||!copyValue||!freeValue||!printValue||!transformIntoNumber) //null check
        return NULL;
    hashTable hash;
    hash = (hashTable) malloc(sizeof(struct hashTable_s));
    if (!hash) //check the memory allocation
        return NULL;
    GenList *elems= malloc(hashNumber*sizeof(Element));
    if (!elems){ //check the memory allocation
        free(hash);
        return NULL;
    }
    for(int i=0;i<hashNumber;i++){
        GenList ls = createLinkedList((CopyFunction ) copyKeyValuePair,(FreeFunction ) destroyKeyValuePair, (PrintFunction )printValue,(EqualFunction) isEqualKey);
        if(!ls){ //check the memory allocation
            for(int j=0;j<i;j++) //if didn't succeed to create the list then delete all the lists created until now
                destroyList(ls);
            free(elems);
            free(hash);
            return NULL;
        }
        elems[i]=ls; // add the list to the array
    }
    // initiate all the functions to their pointers
    hash->equalFuncKey=equalKey;
    hash->freeFuncKey=freeKey;
    hash->copyFuncKey=copyKey;
    hash->printFuncKey=printKey;
    hash->transformIntoNumber=transformIntoNumber;
    hash->freeFuncValue=freeValue;
    hash->copyFuncValue=copyValue;
    hash->printFuncValue=printValue;
    hash->elements=elems;
    hash->hashNumber=hashNumber;
    return hash;
}

status destroyHashTable(hashTable hash){ // destroy the hash table and all the data structures it used
    if (!hash) //null check
        return nullFail;
    for(int i=0;i<hash->hashNumber;i++) // destroy all the lists in the array
        destroyList(hash->elements[i]);
    free(hash->elements);
    free(hash);
    return success;
}

status addToHashTable(hashTable hash, Element key,Element value){
    if (!hash||!key||!value) //null check
        return nullFail;
    int i = hashIndex(hash,key); // get the hash index
    if (searchByKeyInList(hash->elements[i],key)!=NULL) // check if the key was already created
        return wrongParameter;
    Pair pr = createKeyValuePair(hash->copyFuncKey,hash->freeFuncKey,hash->printFuncKey,hash->equalFuncKey,hash->copyFuncValue,hash->freeFuncValue,hash->printFuncValue,key,value);
    if (!pr) //check the memory allocation
        return memoryFail;
    status stat =appendNode(hash->elements[i],pr);
    if(stat!=success){ //check the memory allocation
        destroyKeyValuePair(pr);
        return memoryFail;
    }
    return success;
}

Element lookupInHashTable(hashTable hash, Element key){
    if (!hash||!key) // null check
        return NULL;
    int i = hashIndex(hash,key); // get the hash index
    Pair value = searchByKeyInList(hash->elements[i],key);
    if (!value) // if the value is NULL then the key doesn't exist in the table
        return NULL;
    return getValue(value); // return the value
}

status removeFromHashTable(hashTable hash, Element key){
    if (!hash||!key) // null check
        return nullFail;
    int i = hashIndex(hash,key); // get the hash index
    Pair pr = searchByKeyInList(hash->elements[i],key);
    if (!pr) // if pr id null then couldn't find the key in list, so it isn't in the hash table
        return wrongParameter;
    char * copykey = getKey(pr); // get the key
    deleteNode(hash->elements[i],copykey ); // delete the value
    hash->freeFuncKey(copykey); // delete the key
    return success;
}

status displayHashElements(hashTable hash){
    if (!hash) // null check
        return nullFail;
    Pair pr;
    for(int i=0;i<hash->hashNumber;i++)
    {
        int length = getLengthList(hash->elements[i]); // print all the lists in the array
        for(int j=0;j<length;j++){
            pr = getDataByIndex(hash->elements[i],j+1);
            displayKey(pr);
            displayValue(pr);
        }
    }
    return success;
}