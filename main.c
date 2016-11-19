#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

int main(int argc, char const *argv[]) {
    printf("%s\n", "Hello World!");

    Database db = *createDatabase();
    printf("%p\n", *(db.CSGTable+0));
    if (*(db.CSGTable) == NULL) printf("%s\n", "DIng");

    return 0;
}
