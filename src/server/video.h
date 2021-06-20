#if !defined(VIDEO)
#define VIDEO
#include "structs.h"

Video *load_video(char *filename);

Frame *copy_Frame(Frame *frame);

void display_frame(Frame *frame);

char *squeeze_frame(char **lines);

void free_frame(Frame *frame);

#endif // VIDEO
