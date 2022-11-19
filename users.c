#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
struct User
{
    char ID[20];
    char fname[20];
    char lname[20];
    char user[20];
    char password[20];
};

void createUser()
{
    FILE *f;
    if ((f = fopen("users.bin", "ab+")) == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }

    struct User u;
    char id[20];
    char fname[20];
    char lname[20];
    char user[20];
    char password[20];

    printf("Enter employee id: ");
    scanf("%s", id);
    printf("Enter first name: ");
    scanf("%s", fname);
    printf("Enter last name: ");
    scanf("%s", lname);
    printf("Enter username: ");
    scanf("%s", user);
    printf("Enter password: ");
    scanf("%s", password);

    strncpy(u.ID, id, 20);
    strncpy(u.fname, fname, 20);
    strncpy(u.lname, lname, 20);
    strncpy(u.user, user, 20);
    strncpy(u.password, password, 20);

    fwrite(&u, sizeof(struct User), 1, f);

    fclose(f);

    printf("\n\nUser created! ID generated is %s\n\n", u.ID);
}

void displayUser()
{
    FILE *f;

    if ((f = fopen("users.bin", "rb")) == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }

    struct User u;
    while (fread(&u, sizeof(struct User), 1, f))
    {
        printf("\nId: %s \nFirst name: %s \nLast Name: %s \nUsername: %s \nPassword: %s \n", u.ID, u.fname, u.lname, u.user, u.password);
        break;
    }

    fclose(f);
}

int main()
{
    int choice;
    int n;
    while (1)
    {
        printf("\n\n1. Create a user. \n");
        printf("2. Print User List. \n");
        printf("3. Exit! \n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            createUser();
            break;
        case 2:
            displayUser();
            break;
        case 3:
            printf("Exiting...");
            exit(1);
        default:
            break;
        }
    }
    return 0;
}