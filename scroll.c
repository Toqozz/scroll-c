#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE 2048

void help()
{
    printf("basic scrolling from stdin or other.\n"
           "usage: scroll [-h | -r | -t | -c | -b]\n"
           "       -h Show this help\n"
           "       -r Print with a carriage return instead of a new line\n"
           "       -t Interval between scroll ticks in ms\n"
           "       -c Number of characters to print per scroll (excluding -b)\n"
           "       -b String to put at the very beginning, for easier parsing/piping\n"
           );
    exit(0);
}

// Append a character to a string a set amount of times.
char *append(char *dest, char *add, int times)
{
    int i = 0;
    // calloc(extra space + original length + extra space + 0/)
    char *concat = calloc((times + strlen(dest) + times + 1), 1);
    assert(concat != NULL);

    {
        for(i = 0; i < times; i++)
            strncat(concat, add, 1);
        strncat(concat, dest, strlen(concat) + strlen(dest));
    }
    return concat;
}

void scroll(char *string, char *beginning, int interval, int characters, int carriage)
{
    int i = 0;
    // Nanosleep requires a timespec struct.
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = interval*10000000;

    /* For each character of a string, print a range of characters
       -- works better than you might think. */
    for(i = 0; string[i] != '\0'; i++)
    {
        nanosleep(&req, &req);

        // Sometimes we want to use a carriage return.
        if(carriage == 0) printf("%s%.*s\n", beginning, characters, &string[i]);
        else printf("%s%.*s \b\r", beginning, characters, &string[i]);

        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    char string[BUFSIZE];
    char *beginning = "";
    char *final;
    char space = '-';
    int  interval = 20;
    int  characters = 20;
    int  carriage = 0;

    // Get input from stdin.
    fgets(string, sizeof string, stdin);
    // Strip a new line at the end.
    string[strcspn(string, "\n")] = 0;

    // Parse arguments.
    int  opt;
    while ((opt = getopt(argc, argv, "hrt:c:b:")) != -1) {
        switch(opt)
        {
            case 'h': help(); break;
            case 'r': carriage = 1; break; 
            case 't': interval = strtol(optarg, NULL, 10); break;
            case 'c': characters = strtol(optarg, NULL, 10); break;
            case 'b': beginning = optarg; break;
            default: help();
        }
    }

    // Create a string with spaces added to the front.
    final = append(string, &space, characters); 

    scroll(final, beginning, interval, characters, carriage);
    free(final);

    // End on a new line.
    putchar('\n');

    return(0);
}
