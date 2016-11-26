#include "Tuples.h"


CSG *createCSG(char *course, int SID, char *grade) {
    CSG *CSGP = malloc(sizeof(CSG));
    CSGP->course = course;
    CSGP->SID = SID;
    CSGP->grade = grade;
    CSGP->next = NULL;

    return CSGP;
}

SNAP *createSNAP(int SID, char *name, char *address, int phone) {
    SNAP *SNAPP = malloc(sizeof(SNAP));
    SNAPP->SID = SID;
    SNAPP->name = name;
    SNAPP->address = address;
    SNAPP->phone = phone;
    SNAPP->next = NULL;

    return SNAPP;
}

CP *createCP(char *C, char *P) {
    CP *CPP = malloc(sizeof(CP));
    CPP->course = C;
    CPP->prereq = P;
    CPP->next = NULL;

    return CPP;
}


CDH *createCDH(char *C, char *D, char *H) {
    CDH *CDHP = malloc(sizeof(CDH));
    CDHP->course = C;
    CDHP->day=D;
    CDHP->hour=H;
    CDHP->next = NULL;

    return CDHP;
}

CR *createCR(char *C, char *R) {
    CR *CRP = malloc(sizeof(CR));
    CRP->course=C;
    CRP->room=R;
    CRP->next = NULL;

    return CRP;
}
