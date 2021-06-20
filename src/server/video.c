#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"file_io.h"
#include"video.h"
#include "structs.h"



Video *load_video(char *filename){
    char **video_lines;

    // file IO
    int line_num = read_lines(filename, &video_lines);
    int frame_num = (int) line_num/FRAME_LINE;
    Video *video = malloc(sizeof(Video));
    video->frame_num = frame_num;
    video->frame_counter = 0;
    video->FRAMES = malloc(sizeof(Frame*)*frame_num);

    // fill the frame list of Video struct line by line (each frame consists of 14 lines)
    for (int f = 0; f < frame_num; f++)
    {
        Frame *frm = malloc(sizeof(Frame));
        char *lines[FRAME_LINE];
        for (int l = 0; l < FRAME_LINE; l++)
        {
            lines[l] = video_lines[(FRAME_LINE*f)+l];
        }
        frm->data = squeeze_frame(lines);
        video->FRAMES[f] = frm;
    }
    return video;
}

Frame *copy_Frame(Frame *old_frame){
    Frame *new_frame = malloc(sizeof(Frame));
    new_frame->data = malloc(sizeof(char)*(strlen(old_frame->data)+1));
    strcpy(new_frame->data, old_frame->data);
    return new_frame;
}

void display_frame(Frame *frame){
    // for (int i = 0; i < FRAME_LINE; i++)
    // {
    //     printf("%s\n", frame->lines[i]);
    // }
    printf("%s\n", frame->data);
    return;
}

char *squeeze_frame(char **lines){
    int length = 0;
    for (int i = 0; i < FRAME_LINE; i++)
    {
        length += strlen(lines[i])+1;
    }
    char *squeezed = malloc(sizeof(char)*(length+1));
    squeezed[0] = '\0';
    for (int i = 0; i < FRAME_LINE; i++)
    {
        strcat(squeezed, lines[i]);
        strcat(squeezed, "\n");
    }
    return squeezed;    
}

void free_frame(Frame *frame){
    free(frame->data);
    free(frame);
    
}