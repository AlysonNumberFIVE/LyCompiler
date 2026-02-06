
#include "utils.h"

char *charpush(char *str, char c)
{
    char *newstr;
    size_t length;

    length = strlen(str);
    newstr = (char *)malloc(sizeof(char) * length + 2);
    strcpy(newstr, str);
    newstr[length] = c;
    newstr[length + 1] = '\0';
    return newstr;
}