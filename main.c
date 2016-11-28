#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

int main(int argc, char const *argv[]) {

    Database db = *createDatabase();

    //file reading tests
    insertToDBFromFile(db, "test1.txt");
    insertToDBFromFile(db, "registrar_db.txt");

    //lookup test 1
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

    //lookup test 2 - example 8.12 from FOCS (note: our database has more elements in it than the 'registrar' database in FOCS. As such, more results will be fetched. All results will meet the specification.)
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

    //file saving test
    //saveDBToFile(db, "some file.txt");

    //delete test
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

    //advanced query test
    printf("C. Brown got a %s in CS101\n", findGrade(db, "C. Brown", "CS101"));
    printf("C. Brown is in %s at 10AM on Tu\n", whereStudent(db, "C. Brown", "10AM", "Tu"));

    //union test
    printf("%s\n", "==========UNION============");
    Database db2 = *createDatabase();
    insertToDBFromFile(db2, "other_db.txt");
    unionDB(db, db2, "CSG");

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

    //difference test
    printf("%s\n", "==========DIFFERENCE============");
    differenceDB(db, db2, "CSG");
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

    //intersect test
    printf("%s\n", "==========INTERSECT============");
    insertToDBFromFile(db, "other_db.txt"); //just to ensure that there are tuples in common
    intersectDB(db, db2, "CSG");
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

    //projection test - example 8.13 from FOCS
    printf("%s\n", "==========PROJECTION============");
    Database registrar = *createDatabase(); //re-created to ensure that all data is correct (union, insersect, and join can do weird things...)
    insertToDBFromFile(registrar, "registrar_db.txt");
    projectCSG(lookupCSG(registrar, createSpec("CS101", "*", "*", "*")), "SID");

    //join test - example 8.14 from FOCS
    printf("%s\n", "==========JOIN============");
    joinDB(registrar);

    return 0;
}
