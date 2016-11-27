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
int hashNotSID(char *);
void saveDBToFile(Database, char *);

//inserting
void insertToDBFromCLI(Database);   //write later~
void insertToDBFromFile(Database, char *);
void insertCSG(Database, CSG);
void insertSNAP(Database, SNAP);
void insertCP(Database, CP);
void insertCDH(Database, CDH);
void insertCR(Database, CR);

char ***createSpec(char *, char *, char *, char *); //creates a specification from the strings given

//lookup
void lookup(Database, char ***, char *);
CSG *lookupCSG(Database, char ***);
SNAP *lookupSNAP(Database, char ***);
CP *lookupCP(Database, char ***);
CDH *lookupCDH(Database, char ***);
CR *lookupCR(Database, char ***);

//delete
void delete(Database, char ***, char *);
void deleteCSG(Database, char ***);
void deleteSNAP(Database, char ***);
void deleteCP(Database, char ***);
void deleteCDH(Database, char ***);
void deleteCR(Database, char ***);

//more advanced queries
char *findGrade(Database, char *, char *);    //answers the question: "What grade did 'studentName' get in 'courseName'? "
char *whereStudent(Database, char *, char *, char *);   //answers the question: "Where is 'studentName' at 'time' on 'day'?"

//set operations
void unionDB(Database, Database, char *);  //will union the relation 'rel' on two databases.
void insersectDB(Database, Database, char *);  //will intersect "
void differenceDB(Database, Database, char *); //will find the difference "

//relational algebra operations
//selection is already implemented in the 'lookup' functions
void projectCSG(CSG *, char *);
void projectSNAP(SNAP *, char *);
void projectCP(CP *, char *);
void projectCDH(CDH *, char *);
void projectCR(CR *, char *);
void joinDB(Database, char *, char *);

#endif
