#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

int main(int argc, char const *argv[]) {
    printf("%s\n", "Hello World!");

    Database db = *createDatabase();

    return 0;
}
