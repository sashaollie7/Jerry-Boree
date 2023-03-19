//
// Created by sasha on 12/17/22.
//

#include "LinkedList.h"

typedef struct t_Node{ // struct of node, it is needed for the linked list
    Element data;
    struct Node_t *next;
}Node;

struct t_LinkedList{
    int length;
    Node *head;
    Node *tail;
    CopyFunction copyFunc;
    FreeFunction freeFunc;
    EqualFunction equalFunc;
    PrintFunction printFunc;
};

GenList createLinkedList(CopyFunction copyF,FreeFunction freeF,PrintFunction printF,EqualFunction equalF){ //initialize the list
    if(!copyF||!freeF||!printF||!equalF) // null check
        return NULL;
    GenList ls;
    ls=(GenList) malloc(sizeof(struct t_LinkedList));
    if (!ls)
        return NULL;
    // set all the functions to the list
    ls->freeFunc=freeF;
    ls->copyFunc=copyF;
    ls->equalFunc=equalF;
    ls->printFunc=printF;
    ls->head=NULL;
    ls->tail=NULL;
    ls->length=0;
    return ls;
}

status destroyList(GenList ls)
{
    if (!ls) // null check
        return nullFail;
    int idx=ls->length;
    Node *node,*temp;
    node=ls->head;
    for(int i=0;i<idx;i++){ //go thought all the nodes
        temp=(Node *)node->next; // a temp node to not lose the pointers
        ls->freeFunc(node->data);
        free(node);
        node=temp;
    }
    free(ls); // after destroying all the nodes free the list
    return success;
}

status appendNode(GenList ls,Element elem){
    if (!ls||!elem) // null check
        return nullFail;
    Element temp;
    temp=ls->copyFunc(elem); //create a copy of the data
    if(!temp)
        return memoryFail;
    Node *node;
    node = (Node*) malloc(sizeof(Node));
    if(!node){ // check if node created
        ls->freeFunc(temp);
        return memoryFail;
    }
    node->next=NULL;
    node->data=temp;
    if (ls->length==0){ // check if the list was empty
        ls->head=node;
        ls->tail=node;
    }
    else{ // add the node to the tail if it wasn't empty
        ls->tail->next= (struct Node_t *) node;
        ls->tail=node;
    }
    ls->length++;
    return success;
}
status deleteNode(GenList ls,Element elem){
    if (!ls||!elem) //null check
        return nullFail;
    int idx=ls->length;
    Node *node,*temp;
    if (ls->equalFunc(ls->head->data,elem)==1){ //if the node to delete is the head
        node=(Node*) ls->head->next;
        ls->freeFunc(ls->head->data);
        free(ls->head);
        ls->head=node;
        if(idx==1) // if it was the only node
            ls->tail=NULL;
        ls->length--;
        return success;
    }
    node=ls->head;
    temp = (Node*)node->next;
    for (int i =1;i<idx;i++){ // find the to delete
        if (ls->equalFunc(temp->data,elem)==1){ // enter here if found
            node->next=temp->next;
            ls->freeFunc(temp->data);
            free(temp);
            if(!node->next)
                ls->tail=node;
            ls->length--;
            return success;
        }
        node = (Node*)node->next;
        temp = (Node*)temp->next;
    }
    return wrongParameter; // if here then you want to delete a none existing node
}

status displayList(GenList ls){
    if(!ls) //null check
        return nullFail;
    Node *node;
    node=ls->head; // start with the head
    for(int i=0;i<ls->length;i++){ // print each value of each node
        ls->printFunc(node->data);
        node=(Node*)node->next;
    }
    return success;
}

Element getDataByIndex(GenList ls,int idx){
    if(!ls) //null check
        return NULL;
    if(idx<1||idx>ls->length) // check if got the right index for the search
        return NULL;
    Node *node;
    node = ls->head;
    for(int i=1;i<idx;i++) // advance to the given index
        node=(Node*)node->next;
    return ls->copyFunc(node->data); //sent a copy of thew value
}
int getLengthList(GenList ls){
    if(!ls) // null check
        return -1;
    return ls->length;
}

Element searchByKeyInList(GenList ls, Element elem){
    if(!ls||!elem) //null check
        return NULL;
    Node *node;
    node = ls->head;
    for(int i=0;i<ls->length;i++){ // search though all the list
        if (ls->equalFunc(node->data,elem)==true)
            return ls->copyFunc(node->data);
        node=(Node*)node->next;
    }
    return NULL;
}

GenList copyList(GenList ls){
    return ls; //return a shallow copy
}