#ifndef LAW_DATABASE
#define LAW_DATABASE

#include <stdlib.h>
#include <stdio.h>
#include "Tuples.h"

typedef struct {
    CSG **CSGTable;
    SNAP **SNAPTable;
    CP **CPTable;
    CDH **CDHTable;
    CR **CRTable;
} Database;

//function prototypes
Database *createDatabase();
int hashSID(int);
int hashString(char *);

//inserting
//~master insert works somehow
void insertCSG(Database, CSG);
void insertSNAP(Database, SNAP);
void insertCP(Database, CP);
void insertCDH(Database, CDH);
void insertCR(Database, CR);

#endif
