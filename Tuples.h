#ifndef LAW_TUPLES
#define LAW_TUPLES

#include <stdlib.h>
#include <stdio.h>

//define structs for each relation

//Course, SID, Grade
struct CSG {
    char *course;
    int SID;
    char *grade;
    CSG *next;
};
//SID, Name, Address, Phone
struct SNAP {
    int SID;
    char *name;
    char *address;
    int phone;
    SNAP *next;
};
//Course, Prerequisite
struct CP {
    char *course;
    char *prereq;
    CP *next;
};
//Course, Day, Hour
struct CDH {
    char *course;
    char *day;
    char *hour;
    CDH *next;
};
//Course, Room
struct CR {
    char *course;
    char *room;
    CR *next;
};

//function prototypes
CSG *createCSG(char *, int, char *);
SNAP *createSNAP(int, char *, char *, int);
CP *createCP(char *, char *);
CDH *createCDH(char *, char *, char *);
CR *createCR(char *, char *);

#endif
