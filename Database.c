#include "Database.h"

int hashSID(int SID) {
    return SID % 61;
}

int hashNotSID(char *course) {
    int i = 0;
    while (*course!='\0') {
        i+=*course-'0';
    }
    return i % 61;
}


int hashString(char *string) {
    int i = 0;
    int sum = 0;
    while (*(string+i) != '\0') {
        char c = *(string+i);

        sum += (int)c;
        i++;
    }
    return sum % 61;
}

CSG *createCSG(char *course, int SID, char *grade) {
    CSG *CSGP = malloc(sizeof(CSG));
    CSGP->course = course;
    CSGP->SID = SID;
    CSGP->grade = grade;

    return CSGP;
}

SNAP *createSNAP(int SID, char *name, char *address, int phone) {
    SNAP *SNAPP = malloc(sizeof(SNAP));
    SNAPP->SID = SID;
    SNAPP->name = name;
    SNAPP->address = address;
    SNAPP->phone = phone;

    return SNAPP;
}

CP *createCP(char *C, char *P) {
    CP *CPP = malloc(sizeof(CP));
    CPP->course = C;
    CPP->prereq = P;

    return CPP;
}

CDH *createCDH(char *C, char *D, char *H) {
    CDH *CDHP = malloc(sizeof(CDH));
    CDHP->course = C;
    CDHP->day=D;
    CDHP->hour=H;

    return CDHP;
}

CR *createCR(char *C, char *R) {
    CR *CRP = malloc(sizeof(CR));
    CRP->course=C;
    CRP->room=R;

    return CRP;
}
