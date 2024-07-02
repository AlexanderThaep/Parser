#if !defined(ERROR)
#define ERROR

#define UNDEFINED 0x000
#define GENERIC 0x001
#define FATAL 0x002

void error(char *message, unsigned int type);
void warn(char *message);

#endif