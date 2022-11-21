#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

struct Blockchain
{
    char accountNumber[20];
    char description[100];
    char previousKey[25];
    char currentKey[25];
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

void writeBlock()
{
    char *fileName = "block/temp.bin";
    char *latest = "block/latestfile.txt";
    char buff[100];
    struct Blockchain b;

    printf("Enter account number: ");
    scanf("%s", buff);
    strcpy(b.accountNumber, buff);
    printf("Enter description: ");
    scanf("%s", buff);
    strcpy(b.description, buff);

    FILE *f;
    FILE *pf;
    if ((f = fopen(fileName, "w+")) == NULL)
        exit(1);

    if ((pf = fopen(latest, "r+")) == NULL)
        exit(1);

    fscanf(pf, "%s", buff);
    strcpy(b.previousKey, buff);
    strcpy(buff, rand_string_alloc(25));
    strcpy(b.currentKey, buff);
    fwrite(&b, sizeof(struct Blockchain), 1, f);
    fclose(pf);

    if ((pf = fopen(latest, "w+")) == NULL)
        exit(1);

    fputs(buff, pf);

    fclose(f);
    fclose(pf);

    char newFile[] = "block/";
    strcat(newFile, b.currentKey);
    strcat(newFile, ".bin");
    printf("%s\n", newFile);
    rename(fileName, newFile);
}

void readBlock(int block)
{
    char *latest = "block/latestfile.txt";
    FILE *f;
    FILE *nf;
    char *mode = "r+";
    char buff[100];
    if ((f = fopen(latest, mode)) == NULL)
        exit(1);

    fscanf(f, "%s", buff);
    struct Blockchain b;
    while (block > 0)
    {
        char blockFile[] = "block/";
        strcat(blockFile, buff);
        strcat(blockFile, ".bin");
        if ((nf = fopen(blockFile, mode)) == NULL)
            break;

        fread(&b, sizeof(struct Blockchain), 1, nf);
        printf("%s\n%s\n%s\n%s\n", b.accountNumber, b.description, b.previousKey, b.currentKey);
        strcpy(buff, b.previousKey);
        fclose(nf);
        block--;
    }
    fclose(f);
}

int main()
{
    // writeBlock();
    int block = 3;
    readBlock(block);
    return 0;
}