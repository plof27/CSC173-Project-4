#ifndef LAW_DATABASE
#define LAW_DATABASE

#include <stdlib.h>
#include <stdio.h>

//define structs for each relation

//Course, SID, Grade
typedef struct {
    char *course;
    int SID;
    char *grade;
} CSG;
//SID, Name, Address, Phone
typedef struct {
    int SID;
    char *name8;
    char *address;
    int phone;
} SNAP;
//Course, Prerequisite
typedef struct {
    char *course;
    char *prereq;
} CP;
//Course, Day, Hour
typedef struct {
    char *course;
    char *day;
    char *hour;
} CDH;
//Course, Room
typedef struct {
    char *course;
    char *room;
} CR;

CSG CSGTable[61];
SNAP SNAPTable[61];
CP CPTable[61];
CDH CDHTable[61];
CR CRTable[61];

CSG *createCSG(char *, int, char *);
SNAP *createSNAP(int, char *, char *, int);
CP *createCP(char *, char *);
CDH *createCDH(char *, char *, char *);
CR *createCR(char *, char *);

int hashSID(int);
int hashString(char *);
//insert

#endif
