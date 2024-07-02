#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>

#include <Interface.h>
#include <Token.h>
#include <Error.h>

/* Keep track of attributes of the shell.  */

static pid_t shell_pgid;
static int shell_terminal;
static char *currentDirectory;
static struct termios shell_tmodes;

/* A large deal originates from
https://github.com/jmreyes/simple-c-shell/blob/master/simple-c-shell.c
https://www.gnu.org/software/libc/manual/html_node/Initializing-the-Shell.html 
https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_06_01
*/

void welcomeScreen()
{
    printf("\n\t============================================\n");
    printf("\t               RexEngine Shell\n");
    printf("\t--------------------------------------------\n");
    printf("\t             Licensed under DeezNuts\n");
    printf("\t============================================\n");
    printf("\n\n");
}

void initInterface()
{
    /* See if we are running interactively.  */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty(shell_terminal);

    if (shell_is_interactive)
    {
        /* Loop until we are in the foreground.  */
        while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
            kill(-shell_pgid, SIGTTIN);

        /* Ignore interactive and job-control signals.  */
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        /* Put ourselves in our own process group.  */
        shell_pgid = getpid();
        if (setpgid(shell_pgid, shell_pgid) < 0)
        {
            error("Couldn't put the shell in its own process group", FATAL);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp(shell_terminal, shell_pgid);

        /* Save default terminal attributes for shell.  */
        tcgetattr(shell_terminal, &shell_tmodes);

        currentDirectory = (char *)calloc(INTERFACE_BUFFER_SIZE * 4, sizeof(char));
    }
    shell_pgid = getpid();
}

char *processFile(char *input_buffer, int actual_buffer_size)
{

    return (char *)NULL;
}

char *processTerminal(char *input_buffer, int actual_buffer_size)
{
    char *secondary_buffer;
    unsigned int secondary_size;
    unsigned int index = 0;
    char c;
    while (input_buffer != NULL)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            input_buffer[index] = '\0';
            lex(input_buffer);
            return input_buffer;
        }

        input_buffer[index] = c;
        index++;

        if (index >= actual_buffer_size)
        {
            secondary_size = actual_buffer_size * 2;
            secondary_buffer = realloc(input_buffer, secondary_size);
            if (secondary_buffer == NULL)
            {
                warn("Input lost, unable to realloc buffer!");
                index = actual_buffer_size - 1;
                continue;
            }
            input_buffer = secondary_buffer;
            actual_buffer_size = secondary_size;
        }
    }
    return (char *)NULL;
}

void prompt()
{
    // We print the prompt in the form "<user>@<host> <cwd> >"
    // char hostname[INTERFACE_BUFFER / 2] = "";
    // gethostname(hostname, sizeof(hostname));
    // printf("%s@%s %s > ", getenv("LOGNAME"), hostname, getcwd(currentDirectory, INTERFACE_BUFFER * 2));

    printf("%s >", getcwd(currentDirectory, INTERFACE_BUFFER_SIZE * 2));
}