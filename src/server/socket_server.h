#if !defined(SOCKET_SERVER)
#define SOCKET_SERVER
#include "structs.h"


Server *init_server(Stream **streams, int port);

Worker *init_worker(int socket_disc, int channel_id, char **buffer_pointer);

void serve(Server *server);

void work(Worker *worker);

void create_worker(int sockfd);

int GetString(int fd, char *buffer, size_t nbytes);

void func(int sockfd);

#endif // !SOCKET_SERVER