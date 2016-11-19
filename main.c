#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

int main(int argc, char const *argv[]) {

    Database db = *createDatabase();

    CDH test = *(createCDH("CS101", "M", "9AM"));
    insertCDH(db, test);

    insertToDBFromFile(db, "test1.txt");
    insertToDBFromFile(db, "test2.txt");

    return 0;
}
