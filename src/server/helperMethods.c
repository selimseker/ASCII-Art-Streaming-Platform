#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int is_equal(char *one, char *two)
{
    int equal = 1;
    if (strlen(one) == strlen(two))
    {
        int i;
        for (i = 0; i < strlen(one); i++)
        {
            if (one[i] != two[i])
            {
                equal = 0;
                break;
            }
        }
    }
    else
    {
        equal = 0;
    }
    return equal;
}

//////RECURSIVE FUNCTION//////
int getMaxMin(int *arr, int length, char *maxOrMin)
{
    if (length == 1)
    {
        return arr[0];
    }
    else if (length == 2)
    {
        if (is_equal(maxOrMin, "maximum"))
        {
            return arr[1 - (arr[0] > arr[1])];
        }
        else
        {
            return arr[1 - (arr[0] < arr[1])];
        }
    }
    int leftlength = (int)(length / 2);

    int rightlength = length - leftlength;
    int *left = malloc(sizeof(int) * leftlength);
    int *right = malloc(sizeof(int) * rightlength);
    int i;
    for (i = 0; i < length; i++)
    {
        if (i < leftlength)
        {
            left[i] = arr[i];
        }
        else
        {
            right[i - leftlength] = arr[i];
        }
    }
    int *final = malloc(sizeof(int) * 2);
    final[0] = getMaxMin(left, leftlength, maxOrMin);
    final[1] = getMaxMin(right, rightlength, maxOrMin);
    int max = getMaxMin(final, 2, maxOrMin);
    free(left);
    free(right);
    free(final);
    return max;
}

int isNumber(char *str)
{
    int number = 1;
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (i == 0 && str[i] == '-')
        {
            continue;
        }

        if (str[i] < 48 || str[i] > 57)
        {
            number = 0;
            break;
        }
    }
    return number;
}

int charToInt(char *data){
    if (isNumber(data))
    {
        return atoi(data);
    }
    else
    {
        return -1;
    }
}

int *getFilledArray(int length, int value)
{
    int *arr = malloc(sizeof(int) * length);
    int i;
    for (i = 0; i < length; i++)
    {
        arr[i] = value;
    }
    return arr;
}

char *allocAndCopyStr(char *src)
{
    char *dest = malloc(sizeof(char) * (strlen(src) + 1));
    strcpy(dest, src);
    return dest;
}