#if !defined(STRUCTS)
#define STRUCTS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "queue.h"

#define FRAME_IN_BUFFER_SIZE 20
#define MAX_CLIENT 30
#define FRAME_LINE 14 

// typedef struct frame
// {
//     char* lines[FRAME_LINE];
// } Frame;

typedef struct frame
{
    char* data;
} Frame;

typedef struct video
{
    int frame_num;
    int frame_counter;
    Frame **FRAMES;
} Video;

typedef struct producer
{
    char *filename;
    Video *video;
} Producer;

typedef struct worker
{

    int channel_id;
    int socket_disc;
    int message_length;
    char** channel_buffer_pointer;
    sem_t mutex;
    

} Worker;

typedef struct consumer
{
    Worker *workers[MAX_CLIENT];
    int active_workers;
    char *out_buffer;
} Consumer;

typedef struct stream
{
    int channel_id;
    sem_t full;
    sem_t empty;
    sem_t mutex;
    Queue *BUFFER;
    Producer *producer;
    Consumer *consumer;
} Stream;



typedef struct server
{
    Stream **channels;
    struct sockaddr_in address;
    int PORT;
    char buffer[1025]; //data buffer of 1K
                       //set of socket descriptors (for select function)
    fd_set readfds;
    int client_socket[MAX_CLIENT];
    pthread_t worker_threads[MAX_CLIENT];
    int master_socket;

} Server;




#endif // !STRUCTS