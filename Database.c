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

void saveDBToFile(Database db, char *filename) {
    FILE *fp;
    fp = fopen(filename, "w+"); //clears the file and readys for writing (creates file if it doesn't exist)
    if (fp == NULL) {
        perror("Can't open file");
    } else {
        int i;
        //save CSGTable
        for (i=0; i<61; i++) {
            if (*(db.CSGTable+i) != NULL) {
                printf("%s\n", "Ding");
                fprintf(fp, "CSG\t%s\t%d\t%s\n", (*(db.CSGTable+i))->course, (*(db.CSGTable+i))->SID, (*(db.CSGTable+i))->grade);
            }
        }
        //save SNAPTable
        for (i=0; i<61; i++) {
            if (*(db.SNAPTable+i) != NULL) {
                fprintf(fp, "SNAP\t%d\t%s\t%s\t%d\n", (*(db.SNAPTable+i))->SID, (*(db.SNAPTable+i))->name, (*(db.SNAPTable+i))->address, (*(db.SNAPTable+i))->phone);
            }
        }
        //save CPtable
        for (i=0; i<61; i++) {
            if (*(db.CPTable+i) != NULL) {
                fprintf(fp, "CP\t%s\t%s\n", (*(db.CPTable+i))->course, (*(db.CPTable+i))->prereq);
            }
        }
        //save CDHTable
        for (i=0; i<61; i++) {
            if (*(db.CDHTable+i) != NULL) {
                fprintf(fp, "CDH\t%s\t%s\t%s\n", (*(db.CDHTable+i))->course, (*(db.CDHTable+i))->day, (*(db.CDHTable+i))->hour);
            }
        }
        //save CRTable
        for (i=0; i<61; i++) {
            if (*(db.CRTable+i) != NULL) {
                fprintf(fp, "CR\t%s\t%s\n", (*(db.CRTable+i))->course, (*(db.CRTable+i))->room);
            }
        }
        fclose(fp);
    }
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
                char *course = strtok(NULL, "\t");
                int SID = atoi(strtok(NULL, "\t"));
                char *grade = strtok(NULL, "\t");
                insertCSG(db, *(createCSG(course, SID, grade)));
            } else if (strcmp(token, "SNAP") == 0) {
                //insert to SNAP
                int SID = atoi(strtok(NULL, "\t"));
                char *name = strtok(NULL, "\t");
                char *address = strtok(NULL, "\t");
                int phone = atoi(strtok(NULL, "\t"));
                insertSNAP(db, *(createSNAP(SID, name, address, phone)));
            } else if (strcmp(token, "CP") == 0) {
                //insert to CP
                char *course = strtok(NULL, "\t");
                char *prereq = strtok(NULL, "\t");
                insertCP(db, *(createCP(course, prereq)));
            } else if (strcmp(token, "CDH") == 0) {
                //insert to CDH
                char *course = strtok(NULL, "\t");
                char *day = strtok(NULL, "\t");
                char *hour = strtok(NULL, "\t");
                insertCDH(db, *(createCDH(course, day, hour)));
            } else if (strcmp(token, "CR") == 0) {
                //insert to CR
                char *course = strtok(NULL, "\t");
                char *room = strtok(NULL, "\t");
                insertCR(db, *(createCR(course, room)));
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

void lookup(Database data, char ***spec, char *rel) {
    if (strcmp("CSG", rel) == 0) {
        CSG *results = lookupCSG(data, spec);

        printf("%s", "Results:");
        if (!results) printf("%s", "Nothing Found!");
        printf("\n");
        while(results) {
            printf("Course: %s\n", results->course);
            printf("SID: %d\n", results->SID);
            printf("Grade: %s\n", results->grade);
            results = results->next;
        }
    } else if (strcmp("SNAP", rel) == 0) {
        SNAP *results = lookupSNAP(data, spec);

        printf("%s", "Results:");
        if (!results) printf("%s", "Nothing Found!");
        printf("\n");
        while(results) {
            printf("SID: %d\n", results->SID);
            printf("Name: %s\n", results->name);
            printf("Address: %s\n", results->address);
            printf("Phone: %d\n", results->phone);
            results = results->next;
        }
    } else if (strcmp("CP", rel) == 0) {
        CP *results = lookupCP(data, spec);

        printf("%s", "Results:");
        if (!results) printf("%s", "Nothing Found!");
        printf("\n");
        while(results) {
            printf("Course: %s\n", results->course);
            printf("Prerequisite: %s\n", results->prereq);
            results = results->next;
        }
    } else if (strcmp("CDH", rel) == 0) {
        CDH *results = lookupCDH(data, spec);

        printf("%s", "Results:");
        if (!results) printf("%s", "Nothing Found!");
        printf("\n");
        while(results) {
            printf("Course: %s\n", results->course);
            printf("SID: %s\n", results->day);
            printf("Grade: %s\n", results->hour);
            results = results->next;
        }
    } else if (strcmp("CR", rel) == 0) {
        CR *results = lookupCR(data, spec);

        printf("%s", "Results:");
        if (!results) printf("%s", "Nothing Found!");
        printf("\n");
        while(results) {
            printf("Course: %s\n", results->course);
            printf("Grade: %s\n", results->room);
            results = results->next;
        }
    } else {
        printf("Unknown Relation: %s\n", rel);
    }
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

CP *lookupCP(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+0))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+0));

        if (*(data.CPTable+hashval)) {
            //something is here! build list of matches!
            CP *current = *(data.CPTable+hashval);
            CP *retval = NULL;

            //check that the all information actually matches
            while(current) {
                if (cmpCP(*current, spec) == 0) {
                    //info match! insert to retval!
                    CP *new = createCP(current->course, current->prereq);
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
        CP *retval = NULL;
        int i;
        for (i = 0; i < 61; i++) {
            CP *current = *(data.CPTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCP(*current, spec) == 0) {
                    //info match! insert to retval!
                    CP *new = createCP(current->course, current->prereq);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }
        }
        return retval;
    }
}

CDH *lookupCDH(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+0))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+0));

        if (*(data.CDHTable+hashval)) {
            //something is here! build list of matches!
            CDH *current = *(data.CDHTable+hashval);
            CDH *retval = NULL;

            //check that the all information actually matches
            while(current) {
                if (cmpCDH(*current, spec) == 0) {
                    //info match! insert to retval!
                    CDH *new = createCDH(current->course, current->day, current->hour);
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
        CDH *retval = NULL;
        int i;
        for (i = 0; i < 61; i++) {
            CDH *current = *(data.CDHTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCDH(*current, spec) == 0) {
                    //info match! insert to retval!
                    CDH *new = createCDH(current->course, current->day, current->hour);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }
        }
        return retval;
    }
}

CR *lookupCR(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+0))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+0));

        if (*(data.CRTable+hashval)) {
            //something is here! build list of matches!
            CR *current = *(data.CRTable+hashval);
            CR *retval = NULL;

            //check that the all information actually matches
            while(current) {
                if (cmpCR(*current, spec) == 0) {
                    //info match! insert to retval!
                    CR *new = createCR(current->course, current->room);
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
        CR *retval = NULL;
        int i;
        for (i = 0; i < 61; i++) {
            CR *current = *(data.CRTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCR(*current, spec) == 0) {
                    //info match! insert to retval!
                    CR *new = createCR(current->course, current->room);
                    new->next = retval;
                    retval = new;
                }
                current = current->next;
            }
        }
        return retval;
    }
}
