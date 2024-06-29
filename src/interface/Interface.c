#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>

#include <Interface.h>

/* Keep track of attributes of the shell.  */

static pid_t shell_pgid;
static int shell_terminal;
static char *currentDirectory;
struct termios shell_tmodes;

/* A large deal originates from
https://github.com/jmreyes/simple-c-shell/blob/master/simple-c-shell.c
https://www.gnu.org/software/libc/manual/html_node/Initializing-the-Shell.html */

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
            perror("Couldn't put the shell in its own process group");
            exit(EXIT_FAILURE);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp(shell_terminal, shell_pgid);

        /* Save default terminal attributes for shell.  */
        tcgetattr(shell_terminal, &shell_tmodes);

        currentDirectory = (char *)calloc(INTERFACE_BUFFER * 4, sizeof(char));
    }
    shell_pgid = getpid();
}

char *readFile(char *input_buffer, int actual_buffer_size) 
{


    return (char *)NULL;
}

char *readLine(char *input_buffer, int actual_buffer_size)
{
    int index = 0;
    int c;
    while (input_buffer != NULL)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            input_buffer[index] = '\0';
            return input_buffer;
        }

        input_buffer[index] = c;
        index++;

        if (index >= actual_buffer_size)
        {
            actual_buffer_size *= 2;
            input_buffer = realloc(input_buffer, actual_buffer_size);
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

    printf("%s >", getcwd(currentDirectory, INTERFACE_BUFFER * 2));
}