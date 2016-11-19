#include "Database.h"

int hashSID(int SID) {
    return SID % 61;
}

int hashNotSID(char *course) {
    int i = 0;
    while (*course!='\0') {
        i+=*course-'0';
        course++;
    }
    return i % 61;
}

Database *createDatabase() {
    Database *PDatabase = malloc(sizeof(Database));
    PDatabase->CSGTable = malloc(sizeof(CSG *)*61);
    PDatabase->SNAPTable = malloc(sizeof(SNAP *)*61);
    PDatabase->CPTable = malloc(sizeof(CP *)*61);
    PDatabase->CDHTable = malloc(sizeof(CDH *)*61);
    PDatabase->CRTable =  malloc(sizeof(CR *)*61);

    return PDatabase;
}
