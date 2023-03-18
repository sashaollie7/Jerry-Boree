//
// Created by sasha on 11/28/22.
//
#include "Jerry.h"
#include "Defs.h"

status deletePlanet(Planet *pln){
    if (!pln) //null check
        return nullFail;
    free(pln->name); //free name b4 object
    pln->name=NULL;
    free(pln);
    return success;
}

void deleteOrigin(Origin *org){
    if (!org) //null check
        return;
    free(org->dimension); //free name b4 pointer
    org->dimension=NULL;
    free(org); // delete planet in other function
}

void deletePhysicalCharacteristics(PhysicalCharacteristics *phChar){
    if (!phChar) //null check
        return;
    free(phChar->name); // free name b4 object
    phChar->name=NULL;
    free(phChar);
}

status deleteJerry(Jerry* jer){
    if(!jer) //null check
        return nullFail;
    deleteOrigin(jer->org); //delete first all pointers
    for (int i=0; i<jer->num_of_char; i++)
        deletePhysicalCharacteristics(jer->chars[i]);//delete each Physical character
    if (jer->chars){
        free(jer->chars);
        jer->chars=NULL;
    }
    free(jer->ID);
    jer->ID=NULL;
    free(jer);
    return success;
}

bool jerryHasChar(Jerry *jer, char* name){
    if (!jer||!name) //null check
        return false;

    for (int i=0; i<jer->num_of_char; i++) //check each character
    {
        if (strcmp((jer->chars[i])->name,name)==0)
            return true;
    }
    return false;
}
status addChar(Jerry *jer,PhysicalCharacteristics *phC){
    if (!jer||!phC) //null check
        return nullFail;
    if (jer->num_of_char==0){ // if Jerry doesn't have any characters
        jer->chars= (PhysicalCharacteristics**) malloc(sizeof(PhysicalCharacteristics*));
        if (!jer->chars) // memory allocation check
            return memoryFail;
        jer->chars[0]=phC;
        jer->num_of_char=jer->num_of_char+1;
        return success;
    }
    bool check = jerryHasChar(jer,phC->name); // check if Jerry already have the character
    if (check==1)
        return wrongParameter;
    else {
        PhysicalCharacteristics **check_alloc;
        check_alloc =(PhysicalCharacteristics **) realloc(jer->chars,(jer->num_of_char+1)*sizeof(PhysicalCharacteristics *));
        if (check_alloc==NULL) //memory reallocation check
            return memoryFail;
        jer->chars=check_alloc; // if succeed add the character
        jer->chars[jer->num_of_char] = phC;
        jer->num_of_char = jer->num_of_char + 1;
    }
    return success;
}
status jerryDeleteChar(Jerry *jer,char *name){
    if (!name) //null check
        return nullFail;
    if (jerryHasChar(jer,name)==0) //check if Jerry has the character
        return wrongParameter;
    else{ // if he has it enter here
        for (int i=0; i<jer->num_of_char; i++)
        {
            if (strcmp((jer->chars[i])->name,name)==0){
                jer->num_of_char=jer->num_of_char-1;
                deletePhysicalCharacteristics(jer->chars[i]);//when the character found delete it
                jer->chars[i]=NULL;
                for(int j=i;j<jer->num_of_char;j++)
                    jer->chars[j]=jer->chars[j+1];//from the character to the last reorganizes the array
                if (jer->num_of_char==0){
                    free(jer->chars);
                    jer->chars=NULL;
                    return success;
                }
                PhysicalCharacteristics **check; //reallocate the array to not waste memory
                check=(PhysicalCharacteristics **) realloc(jer->chars,(jer->num_of_char)*sizeof(PhysicalCharacteristics *));
                if (check==NULL) //reallocation check
                    return memoryFail;
                jer->chars=check;
                return success;
            }
        }
    }
}

Planet* initPlanet(char *name,double x,double y,double z){
    if (!name) //null check
        return NULL;
    Planet *pln;
    pln=(Planet*)malloc(sizeof(Planet)); // create memory space for planet
    if (!pln) // allocation check
        return NULL;
    pln->name= (char*) malloc((strlen(name)+1)*sizeof(char)); //create memory space for planet name
    if (!(pln->name)){ // allocation check
        deletePlanet(pln);
        return NULL;
    }
    strcpy(pln->name, name); //if here all the allocations succeeded
    pln->X = x;
    pln->Y = y;
    pln->Z = z;
    return pln;
}

Origin* initOrigin(char *dmn, Planet *pln ){
    if (!pln||!dmn) // null check
        return NULL;
    Origin *org;
    org=(Origin*)malloc(sizeof(Origin)); // allocate space for origin
    if (!org) // allocation check
        return NULL;
    org->dimension= (char*) malloc((strlen(dmn)+1)*sizeof(char)); // allocate space for the name
    if (!(org->dimension)) // allocation check
    {
        deleteOrigin(org);
        return NULL;
    }
    strcpy(org->dimension, dmn); // if here all the allocations succeeded
    org->planet=pln;
    return org;
}

PhysicalCharacteristics* initPCharacter(char *name,double num){
    if (!name) // null check
        return NULL;
    PhysicalCharacteristics *phC;
    phC=(PhysicalCharacteristics*) malloc(sizeof(PhysicalCharacteristics)); // allocate space for the character
    if (!phC) // allocation check
        return NULL;
    phC->name=(char*) malloc((strlen(name)+1)*sizeof(char)); // allocate space for the name
    if (!(phC->name))// allocation check
    {
        deletePhysicalCharacteristics(phC);
        return NULL;
    }
    phC->number=num; // if here all the allocations succeeded
    strcpy(phC->name,name);
    return phC;
}

bool planetEqualKey(Planet* pln1,char* name){
    if(!pln1||!name)
        return false;
    if(strcmp(pln1->name,name)==0) // compare the name of the planet and the given name
        return true;
    return false;
}

Jerry* initJerry(char * id,int hap, char * dmn, Planet *pln){
    if (!dmn||!pln) // null check
        return NULL;
    Origin *org;
    org = initOrigin(dmn,pln); // create origin
    if (!org) // check if succeeded create a planet
        return NULL;
    Jerry *jer;
    jer=(Jerry*) malloc(sizeof(Jerry)); // allocate space for Jerry
    if (!jer){ // allocation check
        deleteOrigin(org);
        return NULL;
    }
    jer->ID=(char*) malloc((strlen(id)+1)*sizeof(char)); // allocate space for the id
    if (!(jer->ID)){ // allocation check
        deleteJerry(jer);
        deleteOrigin(org);
        return NULL;
    }
    strcpy(jer->ID,id); // if here all the allocation succeeded
    jer->num_of_char=0;
    jer->happiness=hap;
    jer->chars=NULL;
    jer->org=org;
    return jer;
}


status printPlanet(Planet *pln){
    if (pln){ //null check
        printf("Planet : %s (%0.2f,%0.2f,%0.2f) \n",pln->name,pln->X,pln->Y,pln->Z);
        return success;
    }
    return nullFail;
}

status shallowDeleteJerry(Jerry* elem){ // really doesn't do anything
    return success;
}

status printJerry(Jerry* jer){
    if (jer){//null check
        printf("Jerry , ID - %s : \n",jer->ID);
        printf("Happiness level : %d \n",jer->happiness);
        printf("Origin : %s \n",jer->org->dimension);
        printPlanet(jer->org->planet);
        if (jer->num_of_char>0){ //print all the characters
            printf("Jerry's physical Characteristics available : \n");
            printf("\t%s : %0.2f ",jer->chars[0]->name,(double)jer->chars[0]->number);
            for (int i=1;i<jer->num_of_char;i++)
                printf(", %s : %0.2f ",jer->chars[i]->name,(double)jer->chars[i]->number);
            printf("\n");
        }
    }
    return success;
}

Jerry* shallowCopyJerry(Jerry* jer){ // also doesn't do anything
    return jer;
}

bool cmpJerry(Jerry* this,Jerry* other){
    if(!this||!other)
        return false;
    if(strcmp(this->ID,other->ID)==0) // compare the 2 ID's
        return true;
    return false;
}

Planet* copyPlanet(Planet *pln){ // also doesn't do anything
    return pln;
}