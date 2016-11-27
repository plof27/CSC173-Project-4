#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

int main(int argc, char const *argv[]) {

    Database db = *createDatabase();

    CDH test = *(createCDH("CS101", "M", "9AM"));
    insertCDH(db, test);

    insertToDBFromFile(db, "test1.txt");
    insertToDBFromFile(db, "registrar_db.txt");

    CSG *results = lookupCSG(db, createSpec("*", "12345", "*", "DC"));
    printf("%s", "Results:");
    if (!results) printf("%s", "Nothing Found!");
    printf("\n");
    while(results) {
        printf("Course: %s\n", results->course);
        printf("SID: %d\n", results->SID);
        printf("Grade: %s\n", results->grade);
        printf("Next: %p\n", results->next);
        results = results->next;
    }

    printf("%s\n", "==================================");

    results = lookupCSG(db, createSpec("CS101", "*", "*", "DC"));
    printf("%s", "Results:");
    if (!results) printf("%s", "Nothing Found!");
    printf("\n");
    while(results) {
        printf("Course: %s\n", results->course);
        printf("SID: %d\n", results->SID);
        printf("Grade: %s\n", results->grade);
        printf("Next: %p\n", results->next);
        results = results->next;
    }

    //saveDBToFile(db, "some file.txt");

    deleteCSG(db, createSpec("CS101", "67890", "*", "DC"));

    printf("%s\n", "==========DELETING============");

    results = lookupCSG(db, createSpec("CS101", "*", "*", "DC"));
    printf("%s", "Results:");
    if (!results) printf("%s", "Nothing Found!");
    printf("\n");
    while(results) {
        printf("Course: %s\n", results->course);
        printf("SID: %d\n", results->SID);
        printf("Grade: %s\n", results->grade);
        printf("Next: %p\n", results->next);
        results = results->next;
    }

    printf("C. Brown got a %s in CS101\n", findGrade(db, "C. Brown", "CS101"));

    return 0;
}
