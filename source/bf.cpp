#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "bf.h"

#define MAX_DATA_SIZE 30000

static char data[MAX_DATA_SIZE + 1];
static int dp, ip;

void bf_init()
{
    for (int i = 0; i < MAX_DATA_SIZE; i++)
        data[i] = 0;
    ip = 0;
    dp = 0;
}

int bf_evaluate(FILE *file)
{
    bf_init();

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    signed char *inst = (signed char *)malloc(filesize + 1);
    if (inst == NULL)
    {
        printf("Failed to allocate memory buffer\n");
        return 1;
    }

    size_t result = fread(inst, 1, filesize, file);
    if (result != filesize)
    {
        printf("Error reading the file.\n");
        // fclose(file);
        free(inst);
        return 1;
    }
    inst[result] = EOF;

    for (ip = 0; inst[ip] != EOF; ip++)
    {
        static int left = 0;
        switch (inst[ip])
        {
        case '>':
            dp++;
            continue;
        case '<':
            dp--;
            continue;
        case '+':
            data[dp]++;
            continue;
        case '-':
            data[dp]--;
            continue;
        case '.':
            printf("%d\n", data[dp]);
            continue;
        case ',':
            data[dp] = getchar();
            continue;
        case '[':
            if (data[dp] == 0)
            {
                ip++;
                while (left != 0 || inst[ip] != ']')
                {
                    if (inst[ip] == '[')
                        left++;
                    if (inst[ip] == ']')
                        left--;
                    ip++;
                }
            }
            continue;
        case ']':
            if (data[dp] != 0)
            {
                ip--;
                while (left != 0 || inst[ip] != '[')
                {
                    if (inst[ip] == '[')
                        left++;
                    if (inst[ip] == ']')
                        left--;
                    ip--;
                }
            }
        default:
            continue;
        }
    }
    free(inst);

    return 0;
}

void bf_hello()
{
    printf("Hello Brainfuck!\n");
}
