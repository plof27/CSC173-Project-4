#include <stdio.h>
#include <stdlib.h>
#include "Database.h"


int main(int argc, char const *argv[]) {




    Database db = *createDatabase();

    CDH cdh = *(createCDH("CS101", "M", "9AM"));
    insertCDH(db, cdh);

    return 0;
}
