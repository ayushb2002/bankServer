#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 20
#define PORT 8080
#define SA struct sockaddr

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

// void createAccount(int sockfd)
// {
//     struct Account a;
//     char buff[MAX];
//     char address[50];
//     bzero(buff, sizeof(buff));
//     printf("Enter Account Number: ");
//     scanf("%s", buff);
//     strcpy(a.accNo, buff);
//     bzero(buff, sizeof(buff));
//     printf("Enter account holder's first name: ");
//     scanf("%s", buff);
//     strcpy(a.holder, buff);
//     strcat(a.holder, " ");
//     bzero(buff, sizeof(buff));
//     printf("Enter account holder's last name: ");
//     scanf("%s", buff);
//     strcat(a.holder, buff);
//     bzero(buff, sizeof(buff));
//     printf("Enter holder's gender: ");
//     scanf("%s", buff);
//     strcpy(a.gender, buff);
//     bzero(buff, sizeof(buff));
//     printf("Enter holder's date of birth: ");
//     scanf("%s", buff);
//     strcpy(a.dob, buff);
//     printf("Enter holder's address: ");
//     scanf("%s", address);
//     strcpy(a.address, address);
//     bzero(buff, sizeof(buff));
//     printf("Enter account type (savings/current): ");
//     scanf("%s", buff);
//     strcpy(a.type, buff);
//     bzero(buff, sizeof(buff));
//     printf("Enter holder's mobile number: ");
//     scanf("%s", buff);
//     strcpy(a.mobile, buff);
//     a.balance = 0.0;

//     write(sockfd, a, sizeof(a));
//     bzero(buff, sizeof(buff));
//     read(sockfd, buff, sizeof(buff));
//     if (strcmp(buff, "false") == 0)
//         printf("Customer account creation failed!\n");
//     else
//         printf("Customer account successfully created!\n");
// }

void createAccount(int sockfd)
{
    char buff[MAX];
    char temp[MAX];
    bzero(buff, sizeof(buff));
    bzero(temp, sizeof(temp));
    printf("Enter Account Number: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    printf("Enter account holder's first name: ");
    scanf("%s", buff);
    strcpy(temp, buff);
    strcat(temp, " ");
    bzero(buff, sizeof(buff));
    printf("Enter account holder's last name: ");
    scanf("%s", buff);
    strcat(temp, buff);
    write(sockfd, temp, sizeof(temp));
    bzero(buff, sizeof(buff));
    printf("Enter holder's gender: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    printf("Enter holder's date of birth: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    printf("Enter holder's address: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    printf("Enter account type (savings/current): ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    printf("Enter holder's mobile number: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    if (strcmp(buff, "false") == 0)
        printf("Customer account creation failed!\n");
    else
        printf("Customer account successfully created!\n");
}

void depositToAccount()
{
    return;
}

void bankMenu(int sockfd)
{
    system("clear");
    char buff[MAX];
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("Welcome %s\n", buff);
    bzero(buff, sizeof(buff));
    int n;
    while (1)
    {
        printf("\n ****************** Banking system menu ****************** \n");
        printf("1. Create new account for customer. \n");
        printf("2. Check balance for account. \n");
        printf("3. Deposit money to account. \n");
        printf("4. Withdraw money from account. \n");
        printf("5. Customer application for new loan. \n");
        printf("6. List of loans provided from branch. \n");
        printf("7. Details of loan taken by account holder. \n");
        printf("8. Logout \n\n");

        printf("Select your choice: ");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            bzero(buff, sizeof(buff));
            strcpy(buff, "createAccount");
            write(sockfd, buff, sizeof(buff));
            createAccount(sockfd);
            break;
        case 2:
            break;
        case 3:
            bzero(buff, sizeof(buff));
            strcpy(buff, "depositAccount");
            write(sockfd, buff, sizeof(buff));
            depositToAccount();
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            bzero(buff, sizeof(buff));
            strcpy(buff, "logout");
            write(sockfd, buff, sizeof(buff));
            printf("Logging out...");
            exit(1);
        default:
            printf("Wrong choice! Retry... \n");
            break;
        }
    }
}

void authenticate(int sockfd)
{
    char buff[MAX];

receiveId:
    bzero(buff, sizeof(buff));
    printf("\nEnter Employee ID: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    if ((strncmp(buff, "false", 5)) == 0)
    {
        printf("Client does not exist...\n");
        goto receiveId;
    }

recieveUsername:
    bzero(buff, sizeof(buff));
    printf("\nEnter username: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    if ((strncmp(buff, "false", 5)) == 0)
    {
        printf("Client username does not exist...\n");
        goto recieveUsername;
    }
receivePassword:
    bzero(buff, sizeof(buff));
    printf("Enter password: ");
    scanf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));

    if ((strncmp(buff, "false", 5)) == 0)
    {
        printf("\nWrong Password!\n");
        goto receivePassword;
    }

    printf("Authenticated!\n");
    bankMenu(sockfd);
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    authenticate(sockfd);

    // close the socket
    close(sockfd);
}