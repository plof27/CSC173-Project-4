#ifndef LAW_DATABASE
#define LAW_DATABASE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
void printDB(Database);     //~write later
void saveDBToFile(char *);  //~write later

//inserting
void insertToDBFromCLI(Database);   //write later~
void insertToDBFromFile(Database, char *);
void insertCSG(Database, CSG);
void insertSNAP(Database, SNAP);
void insertCP(Database, CP);
void insertCDH(Database, CDH);
void insertCR(Database, CR);
CSG *lookupCSG(Database, CSG);

#endif
