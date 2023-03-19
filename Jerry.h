#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

#ifndef HW2_JERRY_H
#define HW2_JERRY_H
typedef struct Planet_t
{
    char *name;
    double X,Y,Z;
}Planet;

typedef struct Origin_t
{
    char *dimension;
    Planet *planet;
}Origin;

typedef struct PhysicalCharacteristics_t
{
    char *name;
    double number;
}PhysicalCharacteristics;

typedef struct Jerry_t
{
    char *ID;
    int happiness;
    Origin *org;
    PhysicalCharacteristics **chars;
    int num_of_char;
}Jerry;

/// leftovers from the last C homework

/// 'create' functions to support Jerry object
Jerry* initJerry(char *,int, char *, Planet *);
Planet* initPlanet(char*, double, double, double);
PhysicalCharacteristics* initPCharacter(char *,double);
Origin* initOrigin(char *,Planet *);
/// 'print' functions
status printJerry(Jerry*);
status printPlanet(Planet*);
/// add and search character in Jerry
bool jerryHasChar(Jerry*,char*);
status addChar(Jerry*,PhysicalCharacteristics*);
/// 'destroy' functions
void deletePhysicalCharacteristics(PhysicalCharacteristics *);
status jerryDeleteChar(Jerry*,char*);
status deleteJerry(Jerry* jer);
status deletePlanet(Planet *pln);

/// all the functions which help support the new data structures
status shallowDeleteJerry(Jerry* jer); //shallow delete jerry (does nothing actually)
Planet* copyPlanet(Planet *pln);// shallow copy the planet (returns the same pointer)
bool cmpJerry(Jerry* jer,Jerry* other); // compare 2 Jerries by their ID's
Jerry* shallowCopyJerry(Jerry* jer); // copy Jerry (returns the same pointer)
bool planetEqualKey(Planet*,char*);// receive a Planet and a name and checks if the name is the same as the planets

#endif //HW2_JERRY_H
