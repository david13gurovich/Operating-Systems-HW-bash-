//David Gurovich 318572047

#include<stdio.h>
#include "signal.h"
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include <time.h>


int loop =0;

void alarmHandler(int signal)
{
    loop =1;
    printf("The server was closed because no service request was received for the last 60 seconds\n");
}

char* substring(char *destination, const char *source, int beg, int n)
{
    // extracts `n` characters from the source string starting from `beg` index
    // and copy them into the destination string
    while (n > 0)
    {
        *destination = *(source + beg);
 
        destination++;
        source++;
        n--;
    }
 
    // null terminate destination string
    *destination = '\0';
 
    // return the destination string
    return destination;
}

void calculate(char * pidC,int num1,int num2,int operation)
{
    FILE *file;
    int flag =0;
    char  nameOfFile[100] = "to_client_";
    strcat(nameOfFile, pidC);
    int result;
    if(operation ==1)  //+
    {
        result = num1 + num2;
    }
    else if(operation ==2)  //-
    {
        result = num1 - num2;
    }
    else if(operation ==3)  //*
    {
        result = num1 * num2;
    }
    else if(operation ==4) // /
    {
        if(num2 == 0)
        {
           flag =1;
        }
        else
        {
        double check = (double) (num1 / num2);
        result = (int) check;
        }
    }
    else{
        printf("ERROR_FROM_EX4\n");
        return;
    }
    if (access(nameOfFile, F_OK) == -1)
    {
        if (file = fopen(nameOfFile, "w"))
        {
            char writeToFile[400];
            char  resultString[100];
            resultString[0] = 0;
            sprintf(resultString, "%d", result); 
            if(flag == 1)
                strcpy(resultString,"CANNOT_DIVIDE_BY_ZERO"); 
            fputs(resultString, file);
            fclose(file);
            kill(atoi(pidC),SIGUSR1);
        }
    }
    else
    {
        printf("ERROR_FROM_EX4\n");
    }
}

void readFromFile( char * buffer)
{
    const char s[2] = "\n";
    char *token;
   char * clientPid;
    /* get the first token */
    clientPid=strtok(buffer, s);
    //substring(clientPid,clientPid, 1, strlen(clientPid));
    token = strtok(NULL, s);
    int num1 = atoi(token);
    token = strtok(NULL, s);
    int operation = atoi(token);
    token = strtok(NULL, s);
    int num2 = atoi(token);
    calculate(clientPid,num1,num2,operation);
}

void handSig(int singal){
    alarm(60);
	signal(SIGUSR1,handSig);
    if (fork() == 0)
    {
    FILE * fptr;
    char buffer[400];

    if ((fptr = fopen("to_srv.txt", "rb")) == NULL)
    {
       printf("ERROR_FROM_EX4\n");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    fread(buffer, 400, 1, fptr);
    fclose(fptr);
    if (remove("to_srv.txt") != 0)
        printf("ERROR_FROM_EX4\n");
    readFromFile(buffer);
    exit(0);                           //son thread done
    }
}



int main(){
    signal(SIGALRM,alarmHandler);
	signal(SIGUSR1,handSig);
    if (access("to_srv.txt", F_OK) == 0) {
        if (remove("to_srv.txt") != 0)
            printf("ERROR_FROM_EX4\n");
    }
    alarm(60);
    while(loop != 1)
    {

    }
	exit(0);
}
