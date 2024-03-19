// David Gurovich 318572047

#include <stdio.h>
#include "signal.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

struct stat st;
char *num1;
char *num2;
char *operation;

void alarmHandler(int signal)
{
    printf("Client closed because no response was received from the server for 30 seconds\n");
}

void handSig(int singal)
{
    alarm(0); // canceling alarm
    signal(SIGUSR1, handSig);
    char fileName[100] = "to_client_";
    int pid = getpid();
    char stringPid[7];
    sprintf(stringPid, "%d", pid);
    strcat(fileName, stringPid);
    FILE *fptr;
    if ((fptr = fopen(fileName, "rb")) == NULL)
    {
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    struct stat st;
    stat(fileName, &st);
    int size = st.st_size;
    char buffer[400];
    fread(buffer, 400, 1, fptr);
    buffer[size] = '\0';
    printf("%s\n", buffer);
    fclose(fptr);
    if (remove(fileName) != 0)
    {
        printf("ERROR_FROM_EX4\n");
        exit(0);
    }
}

void openFile()
{
    FILE *file;
    int count = 0;
    int wentin = 0;
    while (count < 10 && wentin == 0)
    {
        if (access("to_srv.txt", F_OK) == -1)
        {
            if (file = fopen("to_srv.txt", "w"))
            {
                wentin = 1;
                char writeToFile[400];
                char stringPid[7];
                int pid = getpid();
                sprintf(stringPid, "%d", pid);
                strcat(writeToFile, stringPid);
                strcat(writeToFile, "\n");
                strcat(writeToFile, num1);
                strcat(writeToFile, "\n");
                strcat(writeToFile, operation);
                strcat(writeToFile, "\n");
                strcat(writeToFile, num2);
                strcat(writeToFile, "\n");
                fputs(writeToFile, file);
                fclose(file);
            }
        }
        else
        {
            count++;
            srand(time(0));
            unsigned int tmp = rand();
            tmp %= 6;
            sleep(tmp);
        }
    }
    if (wentin == 0 && count == 10)
    {
        printf("ERROR_FROM_EX4\n");
        exit(0);
    }
}

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    int pid = getpid();
    signal(SIGALRM, alarmHandler);
    signal(SIGUSR1, handSig);
    if (atoi(argv[1]) == 0 || (atoi(argv[2]) == 0 && strcmp(argv[2], "0") != 0) || (atoi(argv[3]) == 0 && strcmp(argv[3], "0") != 0) || (atoi(argv[4]) == 0 && strcmp(argv[4], "0") != 0))
    {
        printf("ERROR_FROM_EX4\n");
        exit(1);
    }
    int server_pid = atoi(argv[1]);
    num1 = argv[2];
    operation = argv[3];
    num2 = argv[4];
    openFile();
    if (kill(server_pid, SIGUSR1) == -1)
    {
        if (access("to_srv.txt", F_OK) != -1)
        {
            if (remove("to_srv.txt") != 0)
            {
                printf("ERROR_FROM_EX4\n");
            }
            printf("ERROR_FROM_EX4\n");
            exit(1);
        }
    }
    alarm(30);
    pause();
    exit(0);
}
