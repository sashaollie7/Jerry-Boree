//
// Created by sasha on 12/17/22.
//

#ifndef C_ADT_DEFS_H
#define C_ADT_DEFS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum e_bool { false, true } bool;
typedef enum e_status { success, nullFail, wrongParameter, memoryFail} status;

typedef void * Element;

typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif //C_ADT_DEFS_H
