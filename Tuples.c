#include "Tuples.h"
#include <string.h>

CSG *createCSG(char *course, int SID, char *grade) {
    CSG *CSGP = malloc(sizeof(CSG));
    CSGP->course = malloc(sizeof(char)*100);
    strncpy(CSGP->course, course, 100);
    CSGP->SID = SID;
    CSGP->grade = malloc(sizeof(char)*100);
    strncpy(CSGP->grade, grade, 100);
    CSGP->next = NULL;

    return CSGP;
}

SNAP *createSNAP(int SID, char *name, char *address, int phone) {
    SNAP *SNAPP = malloc(sizeof(SNAP));
    SNAPP->SID = SID;
    SNAPP->name = malloc(sizeof(char)*100);
    strncpy(SNAPP->name, name, 100);
    SNAPP->address = malloc(sizeof(char)*100);
    strncpy(SNAPP->address, address, 100);
    SNAPP->phone = phone;
    SNAPP->next = NULL;

    return SNAPP;
}

CP *createCP(char *C, char *P) {
    CP *CPP = malloc(sizeof(CP));
    CPP->course = malloc(sizeof(char)*100);
    strncpy(CPP->course, C, 100);
    CPP->prereq = malloc(sizeof(char)*100);
    strncpy(CPP->prereq, P, 100);
    CPP->next = NULL;

    return CPP;
}


CDH *createCDH(char *C, char *D, char *H) {
    CDH *CDHP = malloc(sizeof(CDH));
    CDHP->course = malloc(sizeof(char)*100);
    strncpy(CDHP->course, C, 100);
    CDHP->day = malloc(sizeof(char)*100);
    strncpy(CDHP->day, D, 100);
    CDHP->hour=malloc(sizeof(char)*100);
    strncpy(CDHP->hour, H, 100);
    CDHP->next = NULL;

    return CDHP;
}

CR *createCR(char *C, char *R) {
    CR *CRP = malloc(sizeof(CR));
    CRP->course=malloc(sizeof(char)*100);
    strncpy(CRP->course, C, 100);
    CRP->room=malloc(sizeof(char)*100);
    strncpy(CRP->room, R, 100);
    CRP->next = NULL;

    return CRP;
}

int cmpCSG(CSG csg, char ***spec) {
    if ((strcmp("*", *(*(spec+0))) == 0 || strcmp(csg.course, *(*(spec+0))) == 0) &&
    (strcmp("*", *(*(spec+1))) == 0 || csg.SID == atoi(*(*(spec+1)))) &&
    (strcmp("*", *(*(spec+2))) == 0 || strcmp(csg.grade, *(*(spec+2))) == 0)) {
        return 0;
    } else {
        return 1;
    }
}

int cmpSNAP(SNAP snap, char ***spec) {
    if ((strcmp("*", *(*(spec+0))) == 0 || snap.SID == atoi(*(*(spec+0)))) &&
    (strcmp("*", *(*(spec+1))) == 0 || strcmp(snap.name, *(*(spec+1))) == 0) &&
    (strcmp("*", *(*(spec+2))) == 0 || strcmp(snap.address, *(*(spec+2))) == 0) &&
    (strcmp("*", *(*(spec+3))) == 0 || snap.phone == atoi(*(*(spec+3))))) {
        return 0;
    } else {
        return 1;
    }
}

int cmpCP(CP cp, char ***spec) {
    if ((strcmp("*", *(*(spec+0))) == 0 || strcmp(cp.course, *(*(spec+0))) == 0) &&
    (strcmp("*", *(*(spec+1))) == 0 || strcmp(cp.prereq, *(*(spec+1))) == 0)) {
        return 0;
    } else {
        return 1;
    }
}

int cmpCDH(CDH cdh, char ***spec) {
    if ((strcmp("*", *(*(spec+0))) == 0 || strcmp(cdh.course, *(*(spec+0))) == 0) &&
    (strcmp("*", *(*(spec+1))) == 0 || strcmp(cdh.day, *(*(spec+1))) == 0) &&
    (strcmp("*", *(*(spec+2))) == 0 || strcmp(cdh.hour, *(*(spec+2))) == 0)) {
        return 0;
    } else {
        return 1;
    }
}

int cmpCR(CR cr, char ***spec) {
    if ((strcmp("*", *(*(spec+0))) == 0 || strcmp(cr.course, *(*(spec+0))) == 0) &&
    (strcmp("*", *(*(spec+1))) == 0 || strcmp(cr.room, *(*(spec+1))) == 0)) {
        return 0;
    } else {
        return 1;
    }
}
