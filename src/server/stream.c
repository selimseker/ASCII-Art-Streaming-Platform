#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include"stream.h"
#include"video.h"
#include"queue.h"
#include "structs.h"


Stream* init_stream(char *filename, int channel_id){

    Stream *stream = malloc(sizeof(Stream));
    stream->producer = init_producer(filename);
    stream->consumer = init_consumer();
    stream->channel_id = channel_id;
    
    // (*blck).buffer = malloc (?????);
    stream->BUFFER = queueConstructor();

    // init semaphores
    sem_init(&(stream->full), 0, 0);
    sem_init(&(stream->empty), 0, FRAME_IN_BUFFER_SIZE);
    sem_init(&(stream->mutex), 0, 1);
    return stream;
}

Producer *init_producer(char *filename){
    Producer *prod = malloc(sizeof(Producer));
    prod->filename = filename;

    // load the video on thread exec-time
    // prod->video = load_video()
    return prod;
}

Consumer *init_consumer(){
    Consumer *cons = malloc(sizeof(Consumer));
    cons->active_workers = 0;
    cons->out_buffer = NULL;
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        cons->workers[i] = NULL;
    }
    
    return cons;
}


void producer_thread(Stream *stream){
    
    Producer *prod = stream->producer;


    // ########### READ THE FRAMES ########### 
    prod->video = load_video(prod->filename);

    while (1)
    {


        sem_wait(&(stream->empty));
        sem_wait(&(stream->mutex));


        // share the data here        
        Frame *temp_frame = prod->video->FRAMES[prod->video->frame_counter];
        // Frame *frame_cpy = copy_Frame(temp_frame);
        enqueue(stream->BUFFER, (void*) temp_frame, 0, sizeof(Frame*));
        prod->video->frame_counter = (prod->video->frame_counter + 1)%prod->video->frame_num;        
        
        sem_post(&(stream->full));
        sem_post(&(stream->mutex));
    }
}


void consumer_thread(Stream *stream){


    while (1)
    {

        if (stream->consumer->active_workers == 0)
        {
            continue;
        }

        sem_wait(&(stream->full));
        sem_wait(&(stream->mutex));
        // if (stream->consumer->active_workers != 0)
        // {
        Frame *recieved_frame = (Frame*) dequeue(stream->BUFFER);
        stream->consumer->out_buffer = recieved_frame->data;

        // // workers here(?)
        for (int i = 0; i < stream->consumer->active_workers; i++)
        {

            if (stream->consumer->workers[i] == NULL)
            {
                continue;
            }
            else
            {

                sem_post(&(stream->consumer->workers[i]->mutex));
                usleep(800);
            }
                
        }
        sem_post(&(stream->empty));
        sem_post(&(stream->mutex));
    }

}