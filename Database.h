#ifndef LAW_DATABASE
#define LAW_DATABASE

#include <stdlib.h>
#include <stdio.h>
#include "Tuples.h"

typedef struct {
    CSG *CSGTable;
    SNAP *SNAPTable;
    CP *CPTable;
    CDH *CDHTable;
    CR *CRTable;
} Database;

//function prototypes
int hashSID(int);
int hashString(char *);
//insert

#endif
