#if !defined(STREAM)
#define STREAM
#include "structs.h"

Stream *init_stream(char *filename, int channel_id);

Producer *init_producer(char *filename);

Consumer *init_consumer();

void producer_thread(Stream *stream);

void consumer_thread(Stream *stream);
#endif // STREAM
