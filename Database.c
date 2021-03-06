#include "Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void saveDBToFile(Database db, char *filename) {
    FILE *fp;
    fp = fopen(filename, "w+"); //clears the file and readys for writing (creates file if it doesn't exist)
    if (fp == NULL) {
        perror("Can't open file");
    } else {
        int i;
        //save CSGTable
        for (i=0; i<61; i++) {
            CSG *current = *(db.CSGTable+i);
            while (current) {
                fprintf(fp, "CSG\t%s\t%d\t%s\n", current->course, current->SID, current->grade);
                current = current->next;
            }
        }
        //save SNAPTable
        for (i=0; i<61; i++) {
            SNAP *current = *(db.SNAPTable+i);
            while (current) {
                fprintf(fp, "SNAP\t%d\t%s\t%s\t%d\n", current->SID, current->name, current->address, current->phone);
                current = current->next;
            }
        }
        //save CPtable
        for (i=0; i<61; i++) {
            CP *current = *(db.CPTable+i);
            while (current) {
                fprintf(fp, "CP\t%s\t%s\n", current->course, current->prereq);
                current = current->next;
            }
        }
        //save CDHTable
        for (i=0; i<61; i++) {
            CDH *current = *(db.CDHTable+i);
            while (current) {
                fprintf(fp, "CDH\t%s\t%s\t%s\n", current->course, current->day, current->hour);
                current = current->next;
            }
        }
        //save CRTable
        for (i=0; i<61; i++) {
            CR *current = *(db.CRTable+i);
            while (current) {
                fprintf(fp, "CR\t%s\t%s\n", current->course, current->room);
                current = current->next;
            }
        }
        fclose(fp);
    }
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
        *(data.SNAPTable+hashval) = createSNAP(snap.SID, snap.name, snap.address, snap.phone);
        (*(data.SNAPTable+hashval))->next = temp;
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
        *(data.CPTable+hashval) = createCP(cp.course, cp.prereq);
        (*(data.CPTable+hashval))->next = temp;
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
        *(data.CDHTable+hashval) = createCDH(cdh.course, cdh.day, cdh.hour);
        (*(data.CDHTable+hashval))->next = temp;
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
        *(data.CRTable+hashval) = createCR(cr.course, cr.room);
        (*(data.CRTable+hashval))->next = temp;
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

//!!!

void delete(Database data, char ***spec, char *rel) {
    if (strcmp("CSG", rel) == 0) {
        deleteCSG(data, spec);
    } else if (strcmp("SNAP", rel) == 0) {
        deleteSNAP(data, spec);
    } else if (strcmp("CP", rel) == 0) {
        deleteCP(data, spec);
    } else if (strcmp("CDH", rel) == 0) {
        deleteCDH(data, spec);
    } else if (strcmp("CR", rel) == 0) {
        deleteCR(data, spec);
    } else {
        printf("Unknown Relation: %s\n", rel);
    }
}

void deleteCSG(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+1))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashSID(atoi(**(spec+1)));

        if (*(data.CSGTable+hashval)) {
            //something is here! build list of matches!
            CSG *prev = *(data.CSGTable+hashval);
            CSG *current = *(data.CSGTable+hashval);

            //check that the all information actually matches
            while(current) {
                if (cmpCSG(*current, spec) == 0) {
                    //info match! insert to retval!
                    CSG *deleted = current;

                    if (prev == *(data.CSGTable+hashval)) {
                        *(data.CSGTable+hashval) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->grade);
                    free(deleted);
                } else {
                    if (current != *(data.CSGTable+hashval)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    } else {
        //no SID given, iterate over table and find matches
        int i;
        for (i = 0; i < 61; i++) {
            CSG *current = *(data.CSGTable+i);
            CSG *prev = *(data.CSGTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCSG(*current, spec) == 0) {
                    //info match! insert to retval!
                    CSG *deleted = current;

                    if (prev == *(data.CSGTable+i)) {
                        *(data.CSGTable+i) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->grade);
                    free(deleted);
                } else {
                    if (current != *(data.CSGTable+i)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    }
}

void deleteSNAP(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+1))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+1));

        if (*(data.SNAPTable+hashval)) {
            //something is here! build list of matches!
            SNAP *current = *(data.SNAPTable+hashval);
            SNAP *prev = *(data.SNAPTable+hashval);

            //check that the all information actually matches
            while(current) {
                if (cmpSNAP(*current, spec) == 0) {
                    //info match! insert to retval!
                    SNAP *deleted = current;

                    if (prev == *(data.SNAPTable+hashval)) {
                        *(data.SNAPTable+hashval) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->name);
                    free(deleted->address);
                    free(deleted);
                } else {
                    if (current != *(data.SNAPTable+hashval)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    } else {
        //no SID given, iterate over table and find matches
        int i;
        for (i = 0; i < 61; i++) {
            SNAP *current = *(data.SNAPTable+i);
            SNAP *prev = *(data.SNAPTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpSNAP(*current, spec) == 0) {
                    //info match! insert to retval!
                    SNAP *deleted = current;

                    if (prev == *(data.SNAPTable+i)) {
                        *(data.SNAPTable+i) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->name);
                    free(deleted->address);
                    free(deleted);
                } else {
                    if (current != *(data.SNAPTable+i)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    }
}

void deleteCP(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+0))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+0));

        if (*(data.CPTable+hashval)) {
            //something is here! build list of matches!
            CP *current = *(data.CPTable+hashval);
            CP *prev = *(data.CPTable+hashval);

            //check that the all information actually matches
            while(current) {
                if (cmpCP(*current, spec) == 0) {
                    //info match! insert to retval!
                    CP *deleted = current;

                    if (prev == *(data.CPTable+hashval)) {
                        *(data.CPTable+hashval) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->prereq);
                    free(deleted);
                } else {
                    if (current != *(data.CPTable+hashval)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    } else {
        //no SID given, iterate over table and find matches
        int i;
        for (i = 0; i < 61; i++) {
            CP *current = *(data.CPTable+i);
            CP *prev = *(data.CPTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCP(*current, spec) == 0) {
                    //info match! insert to retval!
                    CP *deleted = current;

                    if (prev == *(data.CPTable+i)) {
                        *(data.CPTable+i) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->prereq);
                    free(deleted);
                } else {
                    if (current != *(data.CPTable+i)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    }
}

void deleteCDH(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+0))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+0));

        if (*(data.CDHTable+hashval)) {
            //something is here! build list of matches!
            CDH *current = *(data.CDHTable+hashval);
            CDH *prev = *(data.CDHTable+hashval);

            //check that the all information actually matches
            while(current) {
                if (cmpCDH(*current, spec) == 0) {
                    //info match! insert to retval!
                    CDH *deleted = current;

                    if (prev == *(data.CDHTable+hashval)) {
                        *(data.CDHTable+hashval) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->day);
                    free(deleted->hour);
                    free(deleted);
                } else {
                    if (current != *(data.CDHTable+hashval)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    } else {
        //no SID given, iterate over table and find matches
        int i;
        for (i = 0; i < 61; i++) {
            CDH *current = *(data.CDHTable+i);
            CDH *prev = *(data.CDHTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCDH(*current, spec) == 0) {
                    //info match! insert to retval!
                    CDH *deleted = current;

                    if (prev == *(data.CDHTable+i)) {
                        *(data.CDHTable+i) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->day);
                    free(deleted->hour);
                    free(deleted);
                } else {
                    if (current != *(data.CDHTable+i)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    }
}

void deleteCR(Database data, char ***spec) {

    //spec is an array of memory containing pointers to strings. Ideally, spec should be formatted such that each sring is a value to be queried
    if (strcmp("*", *(*(spec+0))) != 0) {
        //an SID was given! Hash on that shit!
        int hashval = hashNotSID(**(spec+0));

        if (*(data.CRTable+hashval)) {
            //something is here! build list of matches!
            CR *current = *(data.CRTable+hashval);
            CR *prev = *(data.CRTable+hashval);

            //check that the all information actually matches
            while(current) {
                if (cmpCR(*current, spec) == 0) {
                    //info match! insert to retval!
                    CR *deleted = current;

                    if (prev == *(data.CRTable+hashval)) {
                        *(data.CRTable+hashval) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->room);
                    free(deleted);
                } else {
                    if (current != *(data.CRTable+hashval)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    } else {
        //no SID given, iterate over table and find matches
        int i;
        for (i = 0; i < 61; i++) {
            CR *current = *(data.CRTable+i);
            CR *prev = *(data.CRTable+i);
            //start checking elements in bucket i, add to retval if appropriate.
            while(current) {
                if (cmpCR(*current, spec) == 0) {
                    //info match! insert to retval!
                    CR *deleted = current;

                    if (prev == *(data.CRTable+i)) {
                        *(data.CRTable+i) = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    free(deleted->course);
                    free(deleted->room);
                    free(deleted);
                } else {
                    if (current != *(data.CRTable+i)) prev = prev->next;
                    current = current->next;
                }
            }
        }
    }
}

char *findGrade(Database data, char *studentName, char *courseName) {
    SNAP *student = lookupSNAP(data, createSpec("*", studentName, "*", "*"));
    //~lets just assume that there are no students with the same name...
    if (student) {
        char buffer[6];

        sprintf(buffer,"%d",student->SID);

        CSG *grade = lookupCSG(data, createSpec(courseName, buffer, "*", "*"));
        if (grade) {
            return grade->grade;
        } else {
            printf("%s is not taking %s.\n", studentName, courseName);
            return NULL;
        }
    } else {
        printf("Student: %s was not found.\n", studentName);
        return NULL;
    }
}

char *whereStudent(Database data, char *studentName, char *tim, char *day) {
    SNAP *student = lookupSNAP(data, createSpec("*", studentName, "*", "*"));
    if (student) {
        CDH *courses = lookupCDH(data, createSpec("*", day, tim, "*"));
        if (courses) {
            char buf[6];
            sprintf(buf,"%d",student->SID);
            CSG *studentCourses = lookupCSG(data, createSpec("*", buf, "*", "*"));

            CDH *daytimeCourses = courses;
            while (studentCourses) {
                while (daytimeCourses) {
                    if (strcmp(studentCourses->course, daytimeCourses->course) == 0) {
                        CR *room = lookupCR(data, createSpec(studentCourses->course, "*", "*", "*"));
                        return room->room;
                    }
                    daytimeCourses = daytimeCourses->next;
                }
                studentCourses = studentCourses->next;
                daytimeCourses = courses;
            }

            //will only be reached if there are not matches
            printf("%s is not taking any courses at %s on %s\n", studentName, tim, day);
            return NULL;
        } else {
            printf("No courses found at %s on %s.\n", tim, day);
            return NULL;
        }
    } else {
        printf("Student: %s was not found.\n", studentName);
        return NULL;
    }
}

void unionDB(Database data1, Database data2, char *rel) {
    if (strcmp("CSG", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CSG *current2 = *(data2.CSGTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                CSG *current1 = *(data1.CSGTable+i);
                if (current1) {
                    //stuff here! check and add!
                    int flag = 1;
                    while (current1) {
                        char buf[6];
                        sprintf(buf,"%d",current2->SID);
                        if (cmpCSG((*current1), createSpec(current2->course, buf, current2->grade, "*")) == 0) {
                            flag = 0;
                        }
                        current1 = current1->next;
                    }
                    if (flag == 1) {
                        //not already here, add
                        CSG *temp = *(data1.CSGTable+i);
                        *(data1.CSGTable+i) = createCSG(current2->course, current2->SID, current2->grade);
                        (*(data1.CSGTable+i))->next = temp;
                    }
                } else {
                    //nothing here in db1! add!
                    *(data1.CSGTable+i) = createCSG(current2->course, current2->SID, current2->grade);
                }
                current2 = current2->next;
            }
        }
    } else if (strcmp("SNAP", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            SNAP *current2 = *(data2.SNAPTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                SNAP *current1 = *(data1.SNAPTable+i);
                if (current1) {
                    //stuff here! check and add!
                    int flag = 1;
                    while (current1) {
                        char buf1[6];
                        sprintf(buf1,"%d",current2->SID);
                        char buf2[8];
                        sprintf(buf2,"%d",current2->phone);
                        if (cmpSNAP((*current1), createSpec(buf1, current2->name, current2->address, buf2)) == 0) {
                            flag = 0;
                        }
                        current1 = current1->next;
                    }
                    if (flag == 1) {
                        //not already here, add
                        SNAP *temp = *(data1.SNAPTable+i);
                        *(data1.SNAPTable+i) = createSNAP(current2->SID, current2->name, current2->address, current2->phone);
                        (*(data1.SNAPTable+i))->next = temp;
                    }
                } else {
                    //nothing here in db1! add!
                    *(data1.SNAPTable+i) = createSNAP(current2->SID, current2->name, current2->address, current2->phone);
                }
                current2 = current2->next;
            }
        }
    } else if (strcmp("CP", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CP *current2 = *(data2.CPTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                CP *current1 = *(data1.CPTable+i);
                if (current1) {
                    //stuff here! check and add!
                    int flag = 1;
                    while (current1) {
                        if (cmpCP((*current1), createSpec(current2->course, current2->prereq, "*", "*")) == 0) {
                            flag = 0;
                        }
                        current1 = current1->next;
                    }
                    if (flag == 1) {
                        //not already here, add
                        CP *temp = *(data1.CPTable+i);
                        *(data1.CPTable+i) = createCP(current2->course, current2->prereq);
                        (*(data1.CPTable+i))->next = temp;
                    }
                } else {
                    //nothing here in db1! add!
                    *(data1.CPTable+i) = createCP(current2->course, current2->prereq);
                }
                current2 = current2->next;
            }
        }
    } else if (strcmp("CDH", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CDH *current2 = *(data2.CDHTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                CDH *current1 = *(data1.CDHTable+i);
                if (current1) {
                    //stuff here! check and add!
                    int flag = 1;
                    while (current1) {
                        if (cmpCDH((*current1), createSpec(current2->course, current2->day, current2->hour, "*")) == 0) {
                            flag = 0;
                        }
                        current1 = current1->next;
                    }
                    if (flag == 1) {
                        //not already here, add
                        CDH *temp = *(data1.CDHTable+i);
                        *(data1.CDHTable+i) = createCDH(current2->course, current2->day, current2->hour);
                        (*(data1.CDHTable+i))->next = temp;
                    }
                } else {
                    //nothing here in db1! add!
                    *(data1.CDHTable+i) = createCDH(current2->course, current2->day, current2->hour);
                }
                current2 = current2->next;
            }
        }
    } else if (strcmp("CR", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CR *current2 = *(data2.CRTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                CR *current1 = *(data1.CRTable+i);
                if (current1) {
                    //stuff here! check and add!
                    int flag = 1;
                    while (current1) {
                        if (cmpCR((*current1), createSpec(current2->course, current2->room, "*", "*")) == 0) {
                            flag = 0;
                        }
                        current1 = current1->next;
                    }
                    if (flag == 1) {
                        //not already here, add
                        CR *temp = *(data1.CRTable+i);
                        *(data1.CRTable+i) = createCR(current2->course, current2->room);
                        (*(data1.CRTable+i))->next = temp;
                    }
                } else {
                    //nothing here in db1! add!
                    *(data1.CRTable+i) = createCR(current2->course, current2->room);
                }
                current2 = current2->next;
            }
        }
    } else {
        printf("Unknown Relation: %s\n", rel);
    }
}

void intersectDB(Database data1, Database data2, char *rel) {
    if (strcmp("CSG", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CSG *current1 = *(data1.CSGTable+i);
            //start checking elements in bucket i, delete from data1 if appropriate.
            while(current1) {
                char buf[6];
                sprintf(buf,"%d",current1->SID);
                CSG *lookup = lookupCSG(data2, createSpec(current1->course, buf, current1->grade, "*"));
                if (!lookup) {
                    deleteCSG(data1, createSpec(current1->course, buf, current1->grade, "*"));
                }
                current1 = current1->next;
            }
        }
    } else if (strcmp("SNAP", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            SNAP *current1 = *(data1.SNAPTable+i);
            //start checking elements in bucket i, delete from data1 if appropriate.
            while(current1) {
                char buf1[6];
                sprintf(buf1,"%d",current1->SID);
                char buf2[8];
                sprintf(buf2,"%d",current1->phone);
                SNAP *lookup = lookupSNAP(data2, createSpec(buf1, current1->name, current1->address, buf2));
                if (!lookup) {
                    deleteSNAP(data1, createSpec(buf1, current1->name, current1->address, buf2));
                }
                current1 = current1->next;
            }
        }
    } else if (strcmp("CP", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CP *current1 = *(data1.CPTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current1) {
                CP *lookup = lookupCP(data2, createSpec(current1->course, current1->prereq, "*", "*"));
                if (!lookup) {
                    deleteCP(data1, createSpec(current1->course, current1->prereq, "*", "*"));
                }
                current1 = current1->next;
            }
        }
    } else if (strcmp("CDH", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CDH *current1 = *(data1.CDHTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current1) {
                CDH *lookup = lookupCDH(data2, createSpec(current1->course, current1->day, current1->hour, "*"));
                if (!lookup) {
                    deleteCDH(data1, createSpec(current1->course, current1->day, current1->hour, "*"));
                }
                current1 = current1->next;
            }
        }
    } else if (strcmp("CR", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CR *current1 = *(data1.CRTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current1) {
                CR *lookup = lookupCR(data2, createSpec(current1->course, current1->room, "*", "*"));
                if (!lookup) {
                    deleteCR(data1, createSpec(current1->course, current1->room, "*", "*"));
                }
                current1 = current1->next;
            }
        }
    } else {
        printf("Unknown Relation: %s\n", rel);
    }
}

void differenceDB(Database data1, Database data2, char *rel) {
    if (strcmp("CSG", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CSG *current2 = *(data2.CSGTable+i);
            //start checking elements in bucket i, delete from data1 if appropriate.
            while(current2) {
                char buf[6];
                sprintf(buf,"%d",current2->SID);
                deleteCSG(data1, createSpec(current2->course, buf, current2->grade, "*"));
                current2 = current2->next;
            }
        }
    } else if (strcmp("SNAP", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            SNAP *current2 = *(data2.SNAPTable+i);
            //start checking elements in bucket i, delete from data1 if appropriate.
            while(current2) {
                char buf1[6];
                sprintf(buf1,"%d",current2->SID);
                char buf2[8];
                sprintf(buf2,"%d",current2->phone);
                deleteSNAP(data1, createSpec(buf1, current2->name, current2->address, buf2));
                current2 = current2->next;
            }
        }
    } else if (strcmp("CP", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CP *current2 = *(data2.CPTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                deleteCP(data1, createSpec(current2->course, current2->prereq, "*", "*"));
                current2 = current2->next;
            }
        }
    } else if (strcmp("CDH", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CDH *current2 = *(data2.CDHTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                deleteCDH(data1, createSpec(current2->course, current2->day, current2->hour, "*"));
                current2 = current2->next;
            }
        }
    } else if (strcmp("CR", rel) == 0) {
        int i;
        for (i = 0; i < 61; i++) {
            CR *current2 = *(data2.CRTable+i);
            //start checking elements in bucket i, add to data1 if appropriate.
            while(current2) {
                deleteCR(data1, createSpec(current2->course, current2->room, "*", "*"));
                current2 = current2->next;
            }
        }
    } else {
        printf("Unknown Relation: %s\n", rel);
    }
}

void projectCSG(CSG *csg, char *attr) {
    //projects all values from the CSG LL onto the attribute attr
    printf("%s:\n", attr);
    if (strcmp("course", attr) == 0) {
        while(csg) {
            printf("%s\n", csg->course);
            csg = csg->next;
        }
    } else if (strcmp("SID", attr) == 0) {
        while(csg) {
            printf("%d\n", csg->SID);
            csg = csg->next;
        }
    } else if (strcmp("grade", attr) == 0) {
        while(csg) {
            printf("%s\n", csg->grade);
            csg = csg->next;
        }
    } else {
        printf("Unknown attribute: %s\n", attr);
    }
}

void projectSNAP(SNAP *snap, char *attr) {
    printf("%s:\n", attr);
    if (strcmp("SID", attr) == 0) {
        while(snap) {
            printf("%d\n", snap->SID);
            snap = snap->next;
        }
    } else if (strcmp("name", attr) == 0) {
        while(snap) {
            printf("%s\n", snap->name);
            snap = snap->next;
        }
    } else if (strcmp("address", attr) == 0) {
        while(snap) {
            printf("%s\n", snap->address);
            snap = snap->next;
        }
    } else if (strcmp("phone", attr) == 0) {
        while(snap) {
            printf("%d\n", snap->phone);
            snap = snap->next;
        }
    } else {
        printf("Unknown attribute: %s\n", attr);
    }
}

void projectCP(CP *cp, char *attr) {
    printf("%s:\n", attr);
    if (strcmp("course", attr) == 0) {
        while(cp) {
            printf("%s\n", cp->course);
            cp = cp->next;
        }
    } else if (strcmp("prereq", attr) == 0) {
        while(cp) {
            printf("%s\n", cp->prereq);
            cp = cp->next;
        }
    } else {
        printf("Unknown attribute: %s\n", attr);
    }
}

void projectCDH(CDH *cdh, char *attr) {
    printf("%s:\n", attr);
    if (strcmp("course", attr) == 0) {
        while(cdh) {
            printf("%s\n", cdh->course);
            cdh = cdh->next;
        }
    } else if (strcmp("day", attr) == 0) {
        while(cdh) {
            printf("%s\n", cdh->day);
            cdh = cdh->next;
        }
    } else if (strcmp("hour", attr) == 0) {
        while(cdh) {
            printf("%s\n", cdh->hour);
            cdh = cdh->next;
        }
    } else {
        printf("Unknown attribute: %s\n", attr);
    }
}

void projectCR(CR *cr, char *attr) {
    printf("%s:\n", attr);
    if (strcmp("course", attr) == 0) {
        while(cr) {
            printf("%s\n", cr->course);
            cr = cr->next;
        }
    } else if (strcmp("grade", attr) == 0) {
        while(cr) {
            printf("%s\n", cr->room);
            cr = cr->next;
        }
    } else {
        printf("Unknown attribute: %s\n", attr);
    }
}

void joinDB(Database data) {
    //joins the CR and CDH relations where course == course
    //this function is not very general, but it does what is asked
    //a more general function could be written, so assume without loss of generality

    int i;
    for (i = 0; i < 61; i++) {
        CR *current = *(data.CRTable+i);
        //start checking elements in bucket i, add to data1 if appropriate.
        while(current) {
            CDH *results = lookupCDH(data, createSpec(current->course, "*", "*", "*"));
            while (results) {
                printf("Course: %s\n", results->course);
                printf("Day: %s\n", results->day);
                printf("Hour: %s\n", results->hour);
                printf("Room: %s\n", current->room);
                results = results->next;
            }
            current = current->next;
        }
    }
}
