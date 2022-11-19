#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define MAX 20

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

void createAccount()
{
    struct Account a;
    char buff[MAX];
    char address[50];
    bzero(buff, sizeof(buff));
    printf("Enter Account Number: ");
    scanf("%s", buff);
    if (checkAccountAlreadyExists(buff))
    {
        printf("Account Already Exists!");
        exit(1);
    }
    strcpy(a.accNo, buff);
    bzero(buff, sizeof(buff));
    printf("Enter account holder's first name: ");
    scanf("%s", buff);
    strcpy(a.holder, buff);
    strcat(a.holder, " ");
    bzero(buff, sizeof(buff));
    printf("Enter account holder's last name: ");
    scanf("%s", buff);
    strcat(a.holder, buff);
    bzero(buff, sizeof(buff));
    printf("Enter holder's gender: ");
    scanf("%s", buff);
    strcpy(a.gender, buff);
    bzero(buff, sizeof(buff));
    printf("Enter holder's date of birth: ");
    scanf("%s", buff);
    strcpy(a.dob, buff);
    printf("Enter holder's address: ");
    scanf("%s", address);
    strcpy(a.address, address);
    bzero(buff, sizeof(buff));
    printf("Enter account type (savings/current): ");
    scanf("%s", buff);
    strcpy(a.type, buff);
    bzero(buff, sizeof(buff));
    printf("Enter holder's mobile number: ");
    scanf("%s", buff);
    strcpy(a.mobile, buff);
    a.balance = 0.0;

    FILE *f;
    if ((f = fopen("bank.bin", "ab+")) == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }

    fwrite(&a, sizeof(struct Account), 1, f);

    fclose(f);

    printf("\n\nAccount has been created! User's balance - 0.00");
}

void returnAccount()
{
    char accNo[MAX];
    int flag = 0;
    struct Account a;
    printf("\nEnter Account Number: ");
    scanf("%s", accNo);
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
    if (flag)
    {
        printf("\nAccount Found! \n");
        printf("Holder: %s \n", res.holder);
        printf("Type: %s \n", res.type);
        printf("Balance: %f \n", res.balance);
    }
    else
        printf("\nAccount does not exist!");

    fclose(f);
}

int main()
{
    // createAccount();
    returnAccount();
    return 0;
}