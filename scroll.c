#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <argp.h>

/*** arg_parse http://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html#Argp-Example-3
 * i dont even know why i wrote all this out i could have just copy pasted but hopefully you understand better in the mroning.
 * it would be good to understand this (would give more understanding of c not just argp -- missed the comments look at link please */
const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "<toqoz@hotmail.com>";
static char doc[] = "Testing out argp for now...";
static char args_doc[] = "ARG1 ARG2";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"quiet",   'q', 0, 0, "Don't produce any output"},
    {"silent",  's', 0, OPTION_ALIAS},
    {"output",  'o', "FILE", 0, "Output to file instead of stdout" },
    { 0 }
};

struct arguments
{
    char *args[2];
    int  silent, verbose;
    char *output_file;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch(key)
    {
        case 'q': case 's':
            arguments->silent = 1;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case 'o':
            arguments->output_file = arg;
            break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 2)
                argp_usage (state);

            arguments->args[state->arg_num] = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 2)
                argp_usage(state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

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
    // Nanosleep requires a timespec struct.
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = interval*10000000;

    /* For each character of a string, print a range of characters
       -- works better than you might think. */
    for(i = 0; string[i] != '\0'; i++)
    {
        nanosleep(&req, &req);
        printf("%.*s\n", 20, &string[i]);
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    struct arguments arguments;
    arguments.silent = 0;
    arguments.verbose = 0;
    arguments.output_file = "-";

    char space = '-';
    char *final;
    char *string = "hello";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // Create a string with spaces added to the front.
    final = append(string, &space, 20);

    scroll(final, 10);
    free(final);

    // End on a new line.
    putchar('\n');

    return(0);
}
