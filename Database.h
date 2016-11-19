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
Database *createDatabase();
int hashSID(int);
int hashString(char *);

//inserting
//~master insert works somehow
void insertCSG(CSG);
void insertSNAP(SNAP);
void insertCP(CP);
void insertCDH(CDH);
void insertCR(CR);

#endif
