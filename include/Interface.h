#if !defined(INTERFACE)
#define INTERFACE

#define INTERFACE_BUFFER 256

int shell_is_interactive;

void initInterface();
void welcomeScreen();
void prompt();
char *readLine(char *line, int actual_buffer_size);
char *readFile(char *buffer, int actual_buffer_size);

#endif