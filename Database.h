#ifndef LAW_DATABASE
#define LAW_DATABASE

#include <stdlib.h>
#include <stdio.h>
#include "Tuples.h"

typedef struct {
    CSG CSGTable[61];
    SNAP SNAPTable[61];
    CP CPTable[61];
    CDH CDHTable[61];
    CR CRTable[61];
} Database;

//function prototypes
int hashSID(int);
int hashString(char *);
//insert

#endif
