#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX 20

struct Blockchain
{
    char *accountNumber;
    char *description;
    char *previousKey;
    char *currentKey;
};

static char *rand_string(char *str, size_t size)
{
    srand(time(NULL));
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size)
    {
        --size;
        for (size_t n = 0; n < size; n++)
        {
            int key = rand() % (int)(sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char *rand_string_alloc(size_t size)
{
    char *s = malloc(size + 1);
    if (s)
    {
        rand_string(s, size);
    }
    return s;
}

int main()
{
    char *fileName = "block/temp.bin";
    char *mode = "w+";
    char *latest = "block/latestfile.txt";
    char buff[100];
    struct Blockchain b;

    printf("Enter account number: ");
    scanf("%s", buff);
    b.accountNumber = buff;
    printf("Enter description: ");
    scanf("%s", buff);
    b.description = buff;

    FILE *f;
    FILE *pf;
    if ((f = fopen(fileName, mode)) == NULL)
        exit(1);

    if ((pf = fopen(latest, mode)) == NULL)
        exit(1);

    fscanf(pf, "%s", buff);
    b.previousKey = buff;
    strcpy(buff, rand_string_alloc(25));
    b.currentKey = buff;
    fwrite(&b, sizeof(struct Blockchain), 1, f);
    fputs(buff, pf);

    fclose(f);
    fclose(pf);

    char newfile[] = "block/";
    strcat(newfile, b.currentKey);
    strcat(newfile, ".bin");
    rename(fileName, newfile);

    return 0;
}