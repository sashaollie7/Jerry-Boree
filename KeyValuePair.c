//
// Created by sasha on 12/18/22.
//
#include "KeyValuePair.h"

struct KeyValuePair{
    CopyFunction copyFuncKey;
    FreeFunction freeFuncKey;
    EqualFunction equalFuncKey;
    PrintFunction printFuncKey;
    CopyFunction copyFuncValue;
    FreeFunction freeFuncValue;
    PrintFunction printFuncValue;
    Element key;
    Element value;
};

Pair createKeyValuePair(CopyFunction copyKey,FreeFunction freeKey,PrintFunction printKey,EqualFunction equalKey,CopyFunction copyValue,FreeFunction freeValue,PrintFunction printValue,Element key,Element value){
    if(!copyKey||!freeKey||!printKey||!equalKey||!copyValue||!freeValue||!printValue||!key||!value) //null check
        return NULL;
    Pair pr=(Pair) malloc(sizeof(struct KeyValuePair)); // create the pair
    if (!pr)
        return NULL;
    Element Ekey,Evalue;
    Ekey = copyKey(key);
    if(!Ekey){ // copy the key
        destroyKeyValuePair(pr);
        return NULL;
    }
    Evalue = copyValue(value); // copy the value
    if(!Evalue){
        pr->freeFuncKey(Ekey);
        destroyKeyValuePair(pr);
        return NULL;
    }
    //initiate the pointers
    pr->freeFuncKey=freeKey;
    pr->copyFuncKey=copyKey;
    pr->equalFuncKey=equalKey;
    pr->printFuncKey=printKey;
    pr->freeFuncValue=freeValue;
    pr->copyFuncValue=copyValue;
    pr->printFuncValue=printValue;
    pr->value=Evalue;
    pr->key=Ekey;
    return pr;
}
status destroyKeyValuePair(Pair pr){
    if (!pr) // null check
        return nullFail;
    pr->freeFuncValue(pr->value); //free the key
    pr->freeFuncKey(pr->key); // free the value
    free(pr);
    return success;
}

status displayValue(Pair pr){
    if (!pr)
        return nullFail;
    pr->printFuncValue(pr->value); // print the value
    return success;
}

void displayKey(Pair pr){
    if (!pr)
        return;
    pr->printFuncKey(pr->key); // print the key
}

Element getValue(Pair pr){
    if (!pr) // null check
        return NULL;
    return pr->copyFuncValue(pr->value); // return a copy of the value
}

Element getKey(Pair pr){
    if (!pr) //  null check
        return NULL;
    return pr->copyFuncKey(pr->key); // return a copy of the key
}

bool isEqualKey(Pair pr,Element elem){
    if (!pr||!elem)
        return false;
    return pr->equalFuncKey(pr->key,elem); // compares the key of the pair and the key it received
}

Pair copyKeyValuePair(Pair pair){
    return pair;
}
