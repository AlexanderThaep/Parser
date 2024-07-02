#if !defined(INTERFACE)
#define INTERFACE

#define INTERFACE_BUFFER_SIZE 256

int shell_is_interactive;

void initInterface();
void welcomeScreen();
void prompt();

char *processTerminal(char *input_buffer, int actual_buffer_size);
char *processFile(char *input_buffer, int actual_buffer_size);

#endif