#ifndef LAW_TUPLES
#define LAW_TUPLES

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
    char *name;
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

//function prototypes
CSG *createCSG(char *, int, char *);
SNAP *createSNAP(int, char *, char *, int);
CP *createCP(char *, char *);
CDH *createCDH(char *, char *, char *);
CR *createCR(char *, char *);

#endif
