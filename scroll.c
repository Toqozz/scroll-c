#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

// Append a character to a string a set amount of times.
char *append(char *dest, char *add, int times)
{
    int i = 0;
    // calloc(extra space + original length + 0/)
    char *concat = calloc((times + strlen(dest) + 1), 1);
    assert(concat != NULL);

    {
        for(i = 0; i < times; i++)
            strncat(concat, add, 1);
        strncat(concat, dest, strlen(concat) + strlen(dest));
    }
    return concat;
}

void scroll(char *string, int interval)
{
    int i = 0;
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = interval*10000000;

    for(i = 0; string[i] != '\0'; i++)
    {
        nanosleep(&req, &req);
        printf("%.*s\n", 20, &string[i]);
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    char space = '-';
    char *final;
    char *string = "hello";

    final = append(string, &space, 20);
    scroll(final, 10);
    free(final);

    // End on a new line.
    putchar('\n');

    return(0);
}
