#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#define MAX 20
#define PORT 8080
#define SA struct sockaddr

struct User
{
    char ID[20];
    char fname[20];
    char lname[20];
    char user[20];
    char password[20];
} loggedIn;

struct Account
{
    char accNo[MAX];
    char holder[MAX];
    double balance;
    char dob[MAX];
    char mobile[MAX];
    char gender[MAX];
    char type[MAX];
    char address[50];
};

struct Blockchain
{
    char accountNumber[MAX];
    char description[100];
    char previousKey[25];
    char currentKey[25];
};

static char *rand_string(char *str, size_t size)
{
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

void omitToBlockChain(char accNo[], char desc[])
{
    char *fileName = "block/temp.bin";
    char *latest = "block/latestfile.txt";
    char buff[100];
    struct Blockchain b;

    strcpy(b.accountNumber, accNo);
    strcpy(b.description, desc);

    FILE *f;
    FILE *pf;
    if ((f = fopen(fileName, "w+")) == NULL)
        exit(1);

    if ((pf = fopen(latest, "r+")) == NULL)
        exit(1);

retakeKey:
    fgets(buff, 25, pf);
    strcpy(b.previousKey, buff);
    strcpy(buff, rand_string_alloc(25));
    strcpy(b.currentKey, buff);
    if (strcmp(b.currentKey, b.previousKey) == 0)
        goto retakeKey;
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
    rename(fileName, newFile);
}

struct Account returnAccount(char accNo[])
{
    struct Account a;
    int flag = 0;
    FILE *f;
    if ((f = fopen("bank.bin", "ab+")) == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }
    struct Account res;
    while (fread(&a, sizeof(struct Account), 1, f))
    {
        if (strcmp(accNo, a.accNo) == 0)
        {
            res = a;
            flag = 1;
            break;
        }
    }

    fclose(f);

    return res;
}

int checkAccountAlreadyExists(char accNo[])
{
    struct Account a;
    int flag = 0;
    FILE *f;
    if ((f = fopen("bank.bin", "ab+")) == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }
    struct Account res;
    while (fread(&a, sizeof(struct Account), 1, f))
    {
        if (strcmp(accNo, a.accNo) == 0)
        {
            res = a;
            flag = 1;
            break;
        }
    }

    fclose(f);

    return flag;
}

void createAccount(int connfd)
{
    printf("Create Account called!");
    struct Account a;
    char buff[MAX];
    char bufff[MAX];
retakeData:
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    if (checkAccountAlreadyExists(buff))
    {
        bzero(bufff, sizeof(buff));
        strcpy(bufff, "false");
        write(connfd, bufff, sizeof(bufff));
        goto retakeData;
    }
    else
    {
        bzero(bufff, sizeof(bufff));
        strcpy(bufff, "true");
        write(connfd, bufff, sizeof(bufff));
    }
    strcpy(a.accNo, buff);
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    strcpy(a.holder, buff);
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    strcpy(a.gender, buff);
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    strcpy(a.dob, buff);
    read(connfd, buff, sizeof(buff));
    strcpy(a.address, buff);
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    strcpy(a.type, buff);
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    strcpy(a.mobile, buff);
    a.balance = 0.0;

    FILE *f;
    if ((f = fopen("bank.bin", "ab+")) == NULL)
    {
        printf("Error opening the file!");
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
        exit(1);
    }

    if (fwrite(&a, sizeof(struct Account), 1, f))
    {
        bzero(buff, sizeof(buff));
        strcpy(buff, "true");
        write(connfd, buff, sizeof(buff));
    }
    else
    {
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
    }
    fclose(f);

    char desc[] = "Created account ";
    strcat(desc, a.accNo);
    omitToBlockChain(a.accNo, desc);
}

void depositAccount(int connfd)
{
    char bufff[MAX];
    char buff[MAX];
    float balance;
    FILE *f;
    FILE *nf;
    const char *file1 = "temp.bin";
    const char *file2 = "bank.bin";
    const char *mode1 = "r";
    const char *mode2 = "w";
doesNotExist:
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff)); // for account number
    if (!checkAccountAlreadyExists(buff))
    {
        bzero(bufff, sizeof(buff));
        strcpy(bufff, "false");
        write(connfd, bufff, sizeof(bufff));
        goto doesNotExist;
    }
    else
    {
        bzero(bufff, sizeof(bufff));
        strcpy(bufff, "true");
        write(connfd, bufff, sizeof(bufff));
    }
    read(connfd, &balance, sizeof(balance), 0); // for balance

    if ((nf = fopen(file1, mode2)) == NULL)
    {
        printf("Error creating the file!");
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
        exit(1);
    }

    if ((f = fopen(file2, "ab+")) == NULL)
    {
        printf("Error opening the file!");
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
        exit(1);
    }
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, f))
    {
        if (strcmp(buff, acc.accNo) == 0)
        {
            acc.balance += balance;
            fwrite(&acc, sizeof(struct Account), 1, nf);
        }
        else
        {
            fwrite(&acc, sizeof(struct Account), 1, nf);
        }
    }

    fclose(f);
    fclose(nf);
    remove(file2);
    rename(file1, file2);
    printf("Success!\n");

    char desc[] = "Deposited to account ";
    strcat(desc, buff);
    omitToBlockChain(buff, desc);

    bzero(buff, sizeof(buff));
    strcpy(buff, "true");
    write(connfd, buff, sizeof(buff));
}

void withdrawAccount(int connfd)
{
    char bufff[MAX];
    char buff[MAX];
    float balance;
    FILE *f;
    FILE *nf;
    const char *file1 = "temp.bin";
    const char *file2 = "bank.bin";
    const char *mode1 = "r";
    const char *mode2 = "w";
doesNotExist:
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff)); // for account number
    if (!checkAccountAlreadyExists(buff))
    {
        bzero(bufff, sizeof(buff));
        strcpy(bufff, "false");
        write(connfd, bufff, sizeof(bufff));
        goto doesNotExist;
    }
    else
    {
        bzero(bufff, sizeof(bufff));
        strcpy(bufff, "true");
        write(connfd, bufff, sizeof(bufff));
    }
    read(connfd, &balance, sizeof(balance), 0); // for balance

    if ((nf = fopen(file1, mode2)) == NULL)
    {
        printf("Error creating the file!");
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
        exit(1);
    }

    if ((f = fopen(file2, "ab+")) == NULL)
    {
        printf("Error opening the file!");
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
        exit(1);
    }
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, f))
    {
        if (strcmp(buff, acc.accNo) == 0)
        {
            acc.balance -= balance;
            fwrite(&acc, sizeof(struct Account), 1, nf);
        }
        else
        {
            fwrite(&acc, sizeof(struct Account), 1, nf);
        }
    }

    fclose(f);
    fclose(nf);
    remove(file2);
    rename(file1, file2);
    printf("Success!\n");

    char desc[] = "Withdrawn from account ";
    strcat(desc, buff);
    omitToBlockChain(buff, desc);

    bzero(buff, sizeof(buff));
    strcpy(buff, "true");
    write(connfd, buff, sizeof(buff));
}

void checkBalance(int connfd)
{
    char bufff[MAX];
    char buff[MAX];
    float balance;
    FILE *f;
    const char *file = "bank.bin";
    const char *mode = "r";
doesNotExist:
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff)); // for account number
    if (!checkAccountAlreadyExists(buff))
    {
        bzero(bufff, sizeof(bufff));
        strcpy(bufff, "false");
        write(connfd, bufff, sizeof(bufff));
        goto doesNotExist;
    }
    else
    {

        bzero(bufff, sizeof(bufff));
        strcpy(bufff, "true");
        write(connfd, bufff, sizeof(bufff));
    }

    if ((f = fopen(file, mode)) == NULL)
    {
        printf("Error opening the file!");
        bzero(buff, sizeof(buff));
        strcpy(buff, "false");
        write(connfd, buff, sizeof(buff));
        exit(1);
    }

    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, f))
    {
        if (strcmp(buff, acc.accNo) == 0)
        {
            balance = acc.balance;
            break;
        }
    }

    write(connfd, &balance, sizeof(balance), 0);

    fclose(f);

    printf("Success!\n");
}

void postAuthentication(int connfd)
{
    char buff[MAX];
    bzero(buff, sizeof(buff));
    strcpy(buff, loggedIn.fname);
    strcat(buff, " ");
    strcat(buff, loggedIn.lname);
    write(connfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
    while (strcmp(buff, "logout") != 0)
    {
        if (strcmp(buff, "createAccount") == 0)
        {
            printf("Create Account \n");
            createAccount(connfd);
        }
        else if (strcmp(buff, "checkBalance") == 0)
        {
            printf("Checking balance \n");
            checkBalance(connfd);
        }
        else if (strcmp(buff, "depositAccount") == 0)
        {
            printf("Deposit to account \n");
            depositAccount(connfd);
        }
        else if (strcmp(buff, "withdrawFromAccount") == 0)
        {
            printf("Withdraw from account \n");
            withdrawAccount(connfd);
        }
        else
        {
            printf("Did not receive command. %s\n", buff);
            break;
        }

        bzero(buff, sizeof(buff));
        read(connfd, buff, sizeof(buff));
    }
}

void authenticate(int connfd)
{
    char buff[MAX];
    int flag = 0;
    FILE *f;
fetchID:
    bzero(buff, MAX);
    read(connfd, buff, sizeof(buff));
    if ((f = fopen("users.bin", "rb")) == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }

    struct User u;
    while (fread(&u, sizeof(struct User), 1, f))
    {
        if (strcmp(u.ID, buff) == 0)
        {
            strcpy(loggedIn.ID, u.ID);
            strcpy(loggedIn.user, u.user);
            strcpy(loggedIn.password, u.password);
            strcpy(loggedIn.fname, u.fname);
            strcpy(loggedIn.lname, u.lname);
            flag = 1;
            break;
        }
    }
    bzero(buff, MAX);
    if (flag == 0)
    {
        strncpy(buff, "false", 5);
        write(connfd, buff, sizeof(buff));
        goto fetchID;
    }
    else
    {
        strncpy(buff, "true", 4);
        write(connfd, buff, sizeof(buff));
    }

fetchUser:
    flag = 0;

    bzero(buff, MAX);
    read(connfd, buff, sizeof(buff));
    if (strncmp(buff, loggedIn.user, MAX) == 0)
        flag = 1;

    if (flag == 0)
    {
        strncpy(buff, "false", 5);
        write(connfd, buff, sizeof(buff));
        goto fetchUser;
    }
    else
    {
        strncpy(buff, "true", 4);
        write(connfd, buff, sizeof(buff));
    }

fetchPassword:
    flag = 0;

    bzero(buff, MAX);
    read(connfd, buff, sizeof(buff));
    if (strncmp(buff, loggedIn.password, MAX) == 0)
        flag = 1;

    if (flag == 0)
    {
        strncpy(buff, "false", 5);
        write(connfd, buff, sizeof(buff));
        goto fetchPassword;
    }
    else
    {
        strncpy(buff, "true", 4);
        write(connfd, buff, sizeof(buff));
    }
    fclose(f);

    postAuthentication(connfd);
}

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 10)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    authenticate(connfd);

    close(sockfd);
}