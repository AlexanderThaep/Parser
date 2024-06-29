#if !defined(ERROR)
#define ERROR

#define UNDEFINED 0x00
#define GENERIC 0x01
#define FATAL 0x02

void error(char *message, unsigned int type);

#endif