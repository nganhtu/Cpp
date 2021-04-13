#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define USERNAME_SIZE 100
#define BUFF_SIZE 100

char username[USERNAME_SIZE];
bool isLoggedIn = false;

void recordToLog(const char *mode, const char *message, bool success)
{
    FILE *logPtr = fopen("log_20184000.txt", "a");
    if (logPtr == NULL)
    {
        printf("Cannot open log file. Error code: %d\n", errno);
    }
    else
    {
        time_t currTime;
        time(&currTime);
        char timeStr[100];
        strcpy_s(timeStr, 100, ctime(&currTime));
        timeStr[strlen(timeStr) - 1] = '\0';

        fputs("[", logPtr);
        fputs((const char *)timeStr, logPtr);
        fputs("] $ ", logPtr);
        fputs(mode, logPtr);
        fputs(" $ ", logPtr);
        fputs(message, logPtr);
        fputs(" $ ", logPtr);
        if (success)
        {
            fputs("+OK", logPtr);
        }
        else
        {
            fputs("-ERR", logPtr);
        }
        fputs("\n", logPtr);
    }
    fclose(logPtr);
}

void login()
{
    bool success = false;
    char name[USERNAME_SIZE] = "";
    char nameTmp[BUFF_SIZE] = "";
    while (1)
    {
        printf("Enter your username: ");
        scanf("%s", name);
        fflush(stdin);
        if (strlen(name) > 1024)
        {
            printf("Error: input name is too long.\n");
        }
        else
        {
            break;
        }
    }
    if (isLoggedIn)
    {
        printf("You are logged in already with username %s.\n", username);
        recordToLog("1", (const char *)name, false);
        return;
    }
    else
    {
        FILE *accPtr = fopen("account.txt", "r");
        if (accPtr == NULL)
        {
            printf("Cannot open database file. Error code: %d\n", errno);
        }
        else
        {
            char buff[BUFF_SIZE];
            bool accExist = false;
            while (fgets(buff, BUFF_SIZE, accPtr) != NULL)
            {
                int spacePos = 0;
                while (spacePos < strlen(buff))
                {
                    if (buff[spacePos] != ' ')
                    {
                        spacePos++;
                    }
                    else
                    {
                        break;
                    }
                }
                char active;
                memcpy_s(nameTmp, BUFF_SIZE, buff, spacePos);
                nameTmp[spacePos] = '\0';
                active = buff[spacePos + 1];
                if (strcmp((const char *)nameTmp, (const char *)name) == 0)
                {
                    accExist = true;
                    if (active == '0')
                    {
                        printf("Cannot log in. Account is locked.\n");
                    }
                    else if (active == '1')
                    {
                        printf("Successfully logged in to account %s.\n", nameTmp);
                        strcpy_s(username, USERNAME_SIZE, nameTmp);
                        isLoggedIn = true;
                        success = true;
                    }
                    break;
                }
            }
            if (!isLoggedIn && !accExist)
            {
                printf("Cannot log in. Username does not exist.\n");
            }
        }
        fclose(accPtr);
    }
    if (success)
    {
        recordToLog("1", (const char *)nameTmp, true);
    }
    else
    {
        recordToLog("1", (const char *)nameTmp, false);
    }
}

void postmess()
{
    bool success = false;
    char message[1024] = "";
    printf("Enter message to post: ");
    fgets(message, 1024, stdin);
    message[strlen(message) - 1] = '\0';
    fflush(stdin);
    if (isLoggedIn)
    {
        printf("Message successfully posted.\n");
        success = true;
    }
    else
    {
        printf("Error: you are not logged in.\n");
    }
    if (success)
    {
        recordToLog("2", (const char *)message, true);
    }
    else
    {
        recordToLog("2", (const char *)message, false);
    }
}

void logout()
{
    bool success = false;
    if (isLoggedIn)
    {
        char tmp[USERNAME_SIZE];
        strcpy_s(tmp, USERNAME_SIZE, (const char *)username);
        strcpy_s(username, USERNAME_SIZE, "");
        isLoggedIn = false;
        printf("Successfully log out from account %s.\n", tmp);
        success = true;
    }
    else
    {
        printf("Error: you are not logged in.\n");
    }
    if (success)
    {
        recordToLog("3", "", true);
    }
    else
    {
        recordToLog("3", "", false);
    }
}

void exit()
{
    printf("Program is closing...\n");
    recordToLog("4", "", true);
    exit(0);
}

int main()
{
    while (1)
    {
        printf("Menu:\n1.\tLog in\n2.\tPost message\n3.\tLog out\n4.\tExit\n");
        int mode = 0;
        while (mode < 1 || mode > 4)
        {
            printf("Choose mode: ");
            scanf("%d", &mode);
            fflush(stdin);
            if (mode < 1 || mode > 4)
            {
                printf("Error: your mode is %d, not in range [1, 4].\n", mode);
                fflush(stdin);
            }
        }
        switch (mode)
        {
        case 1:
            login();
            break;
        case 2:
            postmess();
            break;
        case 3:
            logout();
            break;
        case 4:
            exit();
            break;
        default:
            printf("unexpected error?!");
            break;
        }
    }

    return 0;
}
