#include "Database.h"

int hashSID(int SID) {
    return SID % 61;
}

int hashString(char *string) {
    int i = 0;
    int sum = 0;
    while (*(string+i) != '\0') {
        char c = *(string+i);

        sum += (int)c;
        i++;
    }
    return sum % 61;
}
