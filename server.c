#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
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

void createAccount(int connfd)
{
    printf("Function called!");
    struct Account a;
    char buff[MAX];
    bzero(buff, sizeof(buff));
    read(connfd, buff, sizeof(buff));
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
    if ((f = fopen("bin/bank.bin", "ab+")) == NULL)
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
            printf("Comparison Successful!");
            createAccount(connfd);
        }
        else
            printf("Did not receive command. %s\n", buff);

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
    if ((f = fopen("bin/users.bin", "rb")) == NULL)
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