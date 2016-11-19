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

    unsigned int i;
    PDatabase->CSGTable = malloc(sizeof(CSG *)*61);
    for (i = 0; i < 61; i++) {
        *(PDatabase->CSGTable+i) = NULL;
    }

    PDatabase->SNAPTable = malloc(sizeof(SNAP *)*61);
    for (i = 0; i < 61; i++) {
        *(PDatabase->SNAPTable+i) = NULL;
    }

    PDatabase->CPTable = malloc(sizeof(CP *)*61);
    for (i = 0; i < 61; i++) {
        *(PDatabase->CPTable+i) = NULL;
    }

    PDatabase->CDHTable = malloc(sizeof(CDH *)*61);
    for (i = 0; i < 61; i++) {
        *(PDatabase->CDHTable+i) = NULL;
    }

    PDatabase->CRTable =  malloc(sizeof(CR *)*61);
    for (i = 0; i < 61; i++) {
        *(PDatabase->CRTable+i) = NULL;
    }

    return PDatabase;
}

void insertToDBFromFile(Database db, char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Can't open file");
    } else {
        char buf[100];
        int len;
        //read lines from the file untill EOF is reached
        while (fgets(buf, 100, fp)) {
            //remove the \n (also ensure the last character is \0)
            int len = strlen(buf);
            char short_buf[len];
            strcpy(short_buf, buf);
            if (short_buf[len-1] !='\0') short_buf[len-1] = '\0';

            //do the things
            char *token = strtok(short_buf, "\t");

            if (strcmp(token, "CSG") == 0) {
                //insert to CSG
                insertCSG(db, *(createCSG(strtok(NULL, "\t"), strtok(NULL, "\t"), strtok(NULL, "\t"))));
            } else if (strcmp(token, "SNAP") == 0) {
                //insert to SNAP
                insertSNAP(db, *(createSNAP(strtok(NULL, "\t"), strtok(NULL, "\t"), strtok(NULL, "\t"), strtok(NULL, "\t"))));
            } else if (strcmp(token, "CP") == 0) {
                //insert to CP
                insertCP(db, *(createCP(strtok(NULL, "\t"), strtok(NULL, "\t"))));
            } else if (strcmp(token, "CDH") == 0) {
                //insert to CDH
                insertCDH(db, *(createCDH(strtok(NULL, "\t"), strtok(NULL, "\t"), strtok(NULL, "\t"))));
            } else if (strcmp(token, "CR") == 0) {
                //insert to CR
                insertCR(db, *(createCR(strtok(NULL, "\t"), strtok(NULL, "\t"))));
            } else {
                perror("Uknown Relation");
            }
        }
        fclose(fp);
    }
}

void insertCSG(Database data, CSG csg) {
    int hashval=hashSID(csg.SID);
    int temp = hashval;
    while ((*(data.CSGTable+hashval))) {
        hashval++;
        hashval=hashval%61;
        if (hashval==temp) {
            perror("hashtable full");
            return;
        }
    }
    if (*(data.CSGTable+hashval)) {
        *(data.CSGTable+hashval)= &csg;
    }
}

void insertSNAP(Database data, SNAP snap) {
    int hashval=hashNotSID(snap.name);
    int temp = hashval;
    while ((*(data.SNAPTable+hashval))) {
        hashval++;
        hashval=hashval%61;
        if (hashval==temp) {
            perror("hashtable full");
            return;
        }
    }
    if (*(data.SNAPTable+hashval)) {
        *(data.SNAPTable+hashval)= &snap;
    }
}
void insertCP(Database data, CP cp) {
    int hashval=hashNotSID(cp.course);
    int temp = hashval;
    while ((*(data.CPTable+hashval))) {
        hashval++;
        hashval=hashval%61;
        if (hashval==temp) {
            perror("hashtable full");
            return;
        }
    }
    if (*(data.CPTable+hashval)) {
        *(data.CPTable+hashval)= &cp;
    }
}
void insertCDH(Database data, CDH cdh) {
    int hashval=hashNotSID(cdh.course);
    int temp = hashval;
    while ((*(data.CDHTable+hashval))) {
        hashval++;
        hashval=hashval%61;
        if (hashval==temp) {
            perror("hashtable full");
            return;
        }
    }
    if (*(data.CDHTable+hashval)) {
        *(data.CDHTable+hashval)= &cdh;
    }
}
void insertCR(Database data, CR cr) {
    int hashval=hashNotSID(cr.course);
    int temp = hashval;
    while ((*(data.CRTable+hashval))) {
        hashval++;
        hashval=hashval%61;
        if (hashval==temp) {
            perror("hashtable full");
            return;
        }
    }
    if (*(data.CRTable+hashval)) {
        *(data.CRTable+hashval)= &cr;
    }
}
