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
                insertCSG(db, *(createCSG(strtok(NULL, "\t"), atoi(strtok(NULL, "\t")), strtok(NULL, "\t"))));
            } else if (strcmp(token, "SNAP") == 0) {
                //insert to SNAP
                insertSNAP(db, *(createSNAP(atoi(strtok(NULL, "\t")), strtok(NULL, "\t"), strtok(NULL, "\t"), atoi(strtok(NULL, "\t")))));
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

    if (*(data.CSGTable+hashval)) {
        //something is here! insert!
        CSG *temp = *(data.CSGTable+hashval);
        *(data.CSGTable+hashval) = createCSG(csg.course, csg.SID, csg.grade);
        (*(data.CSGTable+hashval))->next = temp;
    } else {
        //empty space!
        *(data.CSGTable+hashval) = createCSG(csg.course, csg.SID, csg.grade);
    }
}

void insertSNAP(Database data, SNAP snap) {
    int hashval=hashNotSID(snap.name);

    if (*(data.SNAPTable+hashval)) {
        //something is here! insert!
        SNAP *temp = *(data.SNAPTable+hashval);
        snap.next = temp;
        *(data.SNAPTable+hashval) = createSNAP(snap.SID, snap.name, snap.address, snap.phone);
    } else {
        //empty space!
        *(data.SNAPTable+hashval) = createSNAP(snap.SID, snap.name, snap.address, snap.phone);
    }
}

void insertCP(Database data, CP cp) {
    int hashval=hashNotSID(cp.course);

    if (*(data.CPTable+hashval)) {
        //something is here! insert!
        CP *temp = *(data.CPTable+hashval);
        cp.next = temp;
        *(data.CPTable+hashval) = createCP(cp.course, cp.prereq);
    } else {
        //empty space!
        *(data.CPTable+hashval) = createCP(cp.course, cp.prereq);
    }
}

void insertCDH(Database data, CDH cdh) {
    int hashval=hashNotSID(cdh.course);

    if (*(data.CDHTable+hashval)) {
        //something is here! insert!
        CDH *temp = *(data.CDHTable+hashval);
        cdh.next = temp;
        *(data.CDHTable+hashval) = createCDH(cdh.course, cdh.day, cdh.hour);
    } else {
        //empty space!
        *(data.CDHTable+hashval) = createCDH(cdh.course, cdh.day, cdh.hour);
    }
}

void insertCR(Database data, CR cr) {
    int hashval=hashNotSID(cr.course);

    if (*(data.CRTable+hashval)) {
        //something is here! insert!
        CR *temp = *(data.CRTable+hashval);
        cr.next = temp;
        *(data.CRTable+hashval) = createCR(cr.course, cr.room);
    } else {
        //empty space!
        *(data.CRTable+hashval) = createCR(cr.course, cr.room);
    }
}

/*
CSG *lookup(Database data, char ***information, int switchin) {
    switch(switchin) {  //0 is CSG, 1 is SNAP, 2 is CP, 3 is CDH, 4 is CR



        case 0 :
            int hashval=hashSID(csg.SID);
            int temp = hashval;

            while (*(data.CSGTable+hashval)) {
                if (
                (strcmp((data.CSGTable+hashval)->course, *(*(information)+0)==0)) &&
                (strcmp((itoa(data.CSGTable+hashval)->SID), *(*(information)+1)==0)) &&
                (strcmp((data.CSGTable+hashval)->grade, *(*(information)+2)==0))) {
                    return (data.CSGTable+hashval);
                }
                else if (
                ((strcmp((data.CSGTable+hashval)->course, *(*(information)+0))==0)||(strcmp((data.CSGTable+hashval)->course, "*")==0)) &&
                ((strcmp((itoa(data.CSGTable+hashval)->SID), *(*(information)+1))==0)||(strcmp(itoa((data.CSGTable+hashval)->SID), "*")==0)) &&
                ((strcmp((data.CSGTable+hashval)->grade, *(*(information)+2))==0)||(strcmp((data.CSGTable+hashval)->grade, "*")==0))) {
                    ADD (data.CSGTable+hashval) to XX;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                }
                hashval++;
                hashval=hashval%61;
                if (hashval==temp) {
                    if (*XX=='\0') {~~~~~~~~~
                        perror("not contained");
                        return;
                    }
                    return XX;~~~~~~~~~
                }
            }

        case 1 :
            int hashval=hashNotSID(snap.name);
            int temp = hashval;

            while (*(data.SNAPTable+hashval)) {
                if (
                (strcmp(itoa((data.SNAPTable+hashval)->SID), *(*(information)+0)==0)) &&
                (strcmp((data.SNAPTable+hashval)->name, *(*(information)+1)==0)) &&
                (strcmp((data.SNAPTable+hashval)->address, *(*(information)+2)==0))&&
                (strcmp((data.SNAPTable+hashval)->phone, *(*(information)+3)==0))) {
                    return (data.SNAPTable+hashval);
                }
                else if (
                ((strcmp(itoa((data.SNAPTable+hashval)->SID), *(*(information)+0))==0)||(strcmp(itoa((data.SNAPTable+hashval)->SID), "*")==0)) &&
                ((strcmp((data.SNAPTable+hashval)->name, *(*(information)+1))==0)||(strcmp((data.SNAPTable+hashval)->name, "*")==0)) &&
                ((strcmp((data.SNAPTable+hashval)->address, *(*(information)+2))==0)||(strcmp((data.SNAPTable+hashval)->address, "*")==0)) &&
                ((strcmp(itoa((data.SNAPTable+hashval)->phone), *(*(information)+2))==0)||(strcmp(itoa((data.SNAPTable+hashval)->phone), "*")==0))) {
                    ADD (data.SNAPTable+hashval) to XX;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                }
                hashval++;
                hashval=hashval%61;
                if (hashval==temp) {
                    if (*XX=='\0') {~~~~~~~~~
                        perror("not contained");
                        return;
                    }
                    return XX;~~~~~~~~~
                }
            }
        case 2 :
            int hashval=hashNotSID(cp.course);
            int temp = hashval;

            while (*(data.CPTable+hashval)) {
                if (
                (strcmp((data.CPTable+hashval)->course, *(*(information)+0)==0)) &&
                (strcmp((data.CPTable+hashval)->prereq, *(*(information)+1)==0))) {
                    return (data.CPTable+hashval);
                }
                else if (
                ((strcmp((data.CPTable+hashval)->course, *(*(information)+0))==0)||(strcmp((data.CPTable+hashval)->course, "*")==0)) &&
                ((strcmp((data.CPTable+hashval)->prereq, *(*(information)+1))==0)||(strcmp((data.CPTable+hashval)->prereq, "*")==0))) {
                    ADD (data.CPTable+hashval) to XX;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                }
                hashval++;
                hashval=hashval%61;
                if (hashval==temp) {
                    if (*XX=='\0') {~~~~~~~~~
                        perror("not contained");
                        return;
                    }
                    return XX;~~~~~~~~~
                }
            }

        case 3 :
            int hashval=hashNotSID(cdh.course);
            int temp = hashval;

            while (*(data.CDHTable+hashval)) {
                if (
                (strcmp((data.CDHTable+hashval)->course, *(*(information)+0)==0)) &&
                (strcmp((data.CDHTable+hashval)->day, *(*(information)+1)==0)) &&
                (strcmp((data.CDHTable+hashval)->hour, *(*(information)+2)==0))) {
                    return (data.CDHTable+hashval);
                }
                else if (
                ((strcmp((data.CDHTable+hashval)->course, *(*(information)+0))==0)||(strcmp((data.CDHTable+hashval)->course, "*")==0)) &&
                ((strcmp((data.CDHTable+hashval)->day, *(*(information)+1))==0)||(strcmp((data.CDHTable+hashval)->day, "*")==0)) &&
                ((strcmp((data.CDHTable+hashval)->hour, *(*(information)+2))==0)||(strcmp((data.CDHTable+hashval)->hour, "*")==0))) {
                    ADD (data.CDHTable+hashval) to XX;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                }
                hashval++;
                hashval=hashval%61;
                if (hashval==temp) {
                    if (*XX=='\0') {~~~~~~~~~
                        perror("not contained");
                        return;
                    }
                    return XX;~~~~~~~~~
                }
            }

        case 4 :
            int hashval=hashNotSID(cr.course);
            int temp = hashval;

            while (*(data.CRTable+hashval)) {
                if (
                (strcmp((data.CRTable+hashval)->course, *(*(information)+0)==0)) &&
                (strcmp((data.CRTable+hashval)->room, *(*(information)+1)==0))) {
                    return (data.CRTable+hashval);
                }
                else if (
                ((strcmp((data.CRTable+hashval)->course, *(*(information)+0))==0)||(strcmp((data.CRTable+hashval)->course, "*")==0)) &&
                ((strcmp((data.CRTable+hashval)->room, *(*(information)+1))==0)||(strcmp((data.CRTable+hashval)->room, "*")==0))) {
                    ADD (data.CRTable+hashval) to XX;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                }
                hashval++;
                hashval=hashval%61;
                if (hashval==temp) {
                    if (*XX=='\0') {~~~~~~~~~
                        perror("not contained");
                        return;
                    }
                    return XX;~~~~~~~~~
                }
            }
    }




}
*/

char ***createSpec(char *s1, char *s2, char *s3, char *s4) {
    char ***spec = malloc(sizeof(char **)*4);
    *(spec+0) = malloc(sizeof(char *));
    *(spec+1) = malloc(sizeof(char *));
    *(spec+2) = malloc(sizeof(char *));
    *(spec+3) = malloc(sizeof(char *));

    **(spec+0) = malloc(sizeof(char)*100);
    **(spec+1) = malloc(sizeof(char)*100);
    **(spec+2) = malloc(sizeof(char)*100);
    **(spec+3) = malloc(sizeof(char)*100);

    strncpy(**(spec+0), s1, 100);
    strncpy(**(spec+1), s2, 100);
    strncpy(**(spec+2), s3, 100);
    strncpy(**(spec+3), s4, 100);

    return spec;
}

CSG *lookupCSG(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+1))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashSID(atoi(**(spec+1)));

        if (*(data.CSGTable+hashval)) {
            //something is here! build list of matches!
            CSG *current = *(data.CSGTable+hashval);
            CSG *retval = NULL;

            //check that the all information actually matches
            while(current) {
                if (cmpCSG(*current, spec) == 0) {
                    //info match! insert to retval!
                    CSG *new = createCSG(current->course, current->SID, current->grade);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }

            return retval;
        } else {
            //nothing here! return NULL!
            return NULL;
        }
    } else {
        //no SID given, iterate over table and find matches
        CSG *retval = NULL;
        int i;
        for (i = 0; i < 61; i++) {
            CSG *current = *(data.CSGTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCSG(*current, spec) == 0) {
                    //info match! insert to retval!
                    CSG *new = createCSG(current->course, current->SID, current->grade);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }
        }
        return retval;
    }
}

SNAP *lookupSNAP(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+1))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+1));

        if (*(data.SNAPTable+hashval)) {
            //something is here! build list of matches!
            SNAP *current = *(data.SNAPTable+hashval);
            SNAP *retval = NULL;

            //check that the all information actually matches
            while(current) {
                if (cmpSNAP(*current, spec) == 0) {
                    //info match! insert to retval!
                    SNAP *new = createSNAP(current->SID, current->name, current->address, current->phone);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }

            return retval;
        } else {
            //nothing here! return NULL!
            return NULL;
        }
    } else {
        //no SID given, iterate over table and find matches
        SNAP *retval = NULL;
        int i;
        for (i = 0; i < 61; i++) {
            SNAP *current = *(data.SNAPTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpSNAP(*current, spec) == 0) {
                    //info match! insert to retval!
                    SNAP *new = createSNAP(current->SID, current->name, current->address, current->phone);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }
        }
        return retval;
    }
}
