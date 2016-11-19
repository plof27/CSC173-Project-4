#ifndef LAW_DATABASE
#define LAW_DATABASE

#define MAX_NAME_LENGTH 10
#define MAX_ADDRESS_LENGTH 20
#define MAX_ROOM_LENGTH 20

#include <stdlib.h>

//define structs for each relation

//Course, SID, Grade
typedef struct {
    char course[6];
    int SID;
    char grade[3];
} CSG;
//SID, Name, Address, Phone
typedef struct {
    int SID;
    char name[MAX_NAME_LENGTH+1];
    char address[MAX_ADDRESS_LENGTH+1];
    int phone;
} SNAP;
//Course, Prerequisite
typedef struct {
    char course[6];
    char prereq[6];
} CP;
//Course, Day, Hour
typedef struct {
    char course[6];
    char day[3];
    char hour[5];
} CDH;
//Course, Room
typedef struct {
    char course[6];
    char room[MAX_ROOM_LENGTH+1];
} CR;

CSG CSGTable[61];
SNAP SNAPTable[61];
CP CPTable[61];
CDH CDHTable[61];
CR CRTable[61];



#endif
