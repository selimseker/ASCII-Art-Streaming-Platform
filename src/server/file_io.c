#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_lines(char *inputname, char ***lines)
{

    FILE *fileptr;
    char letter;
    int lineNum = 0;
    char *line;
    char *templine;
    char **lineArray = malloc(0);
    int lineSize = 0;
    if ((fileptr = fopen(inputname, "r")) == NULL)
    {
        printf("File could not be opened\n");
        free(fileptr);
        free(lineArray);
        return -1;
    }
    while (!feof(fileptr))
    {
        fscanf(fileptr, "%c", &letter);
        if (letter == '\r' || letter == '\n' || feof(fileptr))
        {
            if (lineSize == 0)
            {
                // continue;
                line = " "; 
            }
            lineSize = 0;
            lineNum++;
            lineArray = (char **)realloc(lineArray, sizeof(char *) * lineNum);
            lineArray[lineNum - 1] = line;
            line = NULL;
        }
        else
        {
            if (lineSize == 0)
            {
                line = (char *)malloc(sizeof(char) * 2);
            }
            else
            {
                templine = (char *)malloc(sizeof(char) * (lineSize + 1));
                strcpy(templine, line);
                free(line);

                line = (char *)malloc(sizeof(char *) * (lineSize + 2));
                strcpy(line, templine);
                free(templine);
            }
            *(line + lineSize) = (char)letter;
            *(line + lineSize + 1) = '\0';
            lineSize++;
        }
    }
    fclose(fileptr);
    (*lines) = lineArray;
    lineArray = NULL;
    return lineNum;
}

int split(char *line, char letter, char ***result)
{
    char *word = NULL;
    char **words;
    int counter = 0;
    int wordCounter = 0;
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        if (line[i] == letter)
        {
            if (counter == 0)
            {
                continue;
            }
            if (wordCounter == 0)
            {
                words = malloc(sizeof(char *));
            }
            else
            {
                words = realloc(words, sizeof(char *) * (wordCounter + 1));
            }
            words[wordCounter] = word;
            word = NULL;
            counter = 0;
            wordCounter++;
        }
        else
        {
            if (counter == 0)
            {
                word = malloc(sizeof(char) * 2);
            }
            else
            {
                word = realloc(word, sizeof(char) * (counter + 2));
            }
            word[counter] = line[i];
            word[counter + 1] = '\0';
            counter++;
        }
    }

    if (counter == 0 && wordCounter == 0 && word == NULL)
    {
        *result = NULL;
        return 0;
    }
    if (word != NULL && counter != 0)
    {
        if (wordCounter == 0)
        {
            words = malloc(sizeof(char *));
        }
        else
        {
            words = realloc(words, sizeof(char *) * (wordCounter + 1));
        }
        words[wordCounter] = word;
        word = NULL;
        counter = 0;
        wordCounter++;
    }

    *result = words;
    return wordCounter;
}

void freeStringArray(char ***lines, int length)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        free((*lines)[i]);
    }
    free(*lines);
}

void printToOutput(char *filename, char *data)
{
    FILE *outfile = fopen(filename, "a");
    if (data[strlen(data) - 1] == '\n')
    {
        fprintf(outfile, "%s", data);
    }
    else
    {
        fprintf(outfile, "%s\n", data);
    }
    fclose(outfile);
}
