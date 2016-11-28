#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

int main(int argc, char const *argv[]) {

    Database db = *createDatabase();

    //file reading tests
    printf("%s\n", "==========LOADING============");
    printf("%s\n", "Loading database from \"registrar_db.txt\"...");
    insertToDBFromFile(db, "registrar_db.txt");

    //lookup test 1
    printf("%s\n", "==========LOOKUP 1============");
    printf("%s\n", "Looking up all CSG entries where StudentID = 12345");
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

    printf("%s\n", "===========LOOKUP 2============");

    //lookup test 2 - example 8.12 from FOCS (note: our database has more elements in it than the 'registrar' database in FOCS. As such, more results will be fetched. All results will meet the specification.)
    printf("%s\n", "Selecting all CSG entries where course = \"CS101\"");
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
    printf("%s\n", "============SAVING==============");
    printf("%s\n", "Saving current database to \"some file.txt\"");
    saveDBToFile(db, "some file.txt");

    //delete test
    printf("%s\n", "==========DELETING============");
    printf("%s\n", "Deleting all records of student 67890 in CS101");
    deleteCSG(db, createSpec("CS101", "67890", "*", "DC"));
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
    printf("%s\n", "==========ADVANCED QUERIES============");
    printf("C. Brown got a %s in CS101\n", findGrade(db, "C. Brown", "CS101"));
    printf("C. Brown is in %s at 10AM on Tu\n", whereStudent(db, "C. Brown", "10AM", "Tu"));

    //union test
    printf("%s\n", "==========UNION============");
    printf("%s\n", "Unioning CSG entries from another database into the main database");
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
    printf("%s\n", "Finding the difference between the main database and another database");
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
    printf("%s\n", "Finding the intersection between the main database and another database");
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
    printf("%s\n", "Projecting all entries in the CSG table where course = CS101 onto StudentID");
    Database registrar = *createDatabase(); //re-created to ensure that all data is correct (union, insersect, and join can do weird things...)
    insertToDBFromFile(registrar, "registrar_db.txt");
    projectCSG(lookupCSG(registrar, createSpec("CS101", "*", "*", "*")), "SID");

    //join test - example 8.14 from FOCS
    printf("%s\n", "==========JOIN============");
    printf("%s\n", "Joining CR and CDH where course = course");
    joinDB(registrar);

    return 0;
}
