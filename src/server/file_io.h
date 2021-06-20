#if !defined(FILO_IO)
#define FILO_IO

int read_lines(char *inputname, char ***lines);

void freeStringArray(char*** lines, int length);

int split(char *line, char letter, char*** result);

void printToOutput(char* filename, char* data);

#endif // FILO_IO
