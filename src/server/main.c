#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include"stream.h"
#include"video.h"
#include "socket_server.h"
#include "structs.h"





int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        printf("Error: invalid argument(s)\n");
        return 0;
    }
    const int PORT = atoi(argv[1]);
    const int CHANNEL_NUM = atoi(argv[2]);
    char** file_names = malloc(sizeof(char*)*CHANNEL_NUM);
    pthread_t *channel_threads = malloc(sizeof(pthread_t)*2*CHANNEL_NUM);
    pthread_t server_thread;
    Stream** channels = malloc(sizeof(Stream*)*CHANNEL_NUM);
    Stream *temp_stream;    

    // init channels
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        printf("%s will be play in channel %d\n", argv[3+i], i);
        file_names[i] = argv[3+i];
        channels[i] = init_stream(file_names[i], i);
    }

    Server *server = init_server(channels, PORT);    


    // create threads
    if( (pthread_create( &server_thread, NULL, &serve, server)) )
    {
        printf("Thread creation failed\n");
    }
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        pthread_create( &channel_threads[i], NULL, &producer_thread, channels[i]);
        pthread_create( &channel_threads[i+1], NULL, &consumer_thread, channels[i]);
    }
    





    pthread_join( server_thread, NULL);
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        pthread_join( channel_threads[i], NULL);
        pthread_join( channel_threads[i+1], NULL);
    }

    printf("thread ends...\n");

    return 0;
}