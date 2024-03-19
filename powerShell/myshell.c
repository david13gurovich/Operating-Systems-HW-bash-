//David Gurovich 318572047

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

struct History {
  char command[100];
  pid_t pidOfCommand;
} ;

int main(int argc, char* argv[])
{
    int stat,waited,ret_code;
    pid_t pid;
    int execFlag;
    int commandNum =0;
    struct History commands[100];
    int argumentPath=1;
    char * path = getenv("PATH");
    while(argv[argumentPath] != NULL)
    {
        strcat(path,":");
        strcat(path,argv[argumentPath]);
        setenv("PATH",path,1);
        argumentPath++;
    }
    const char* historyString = "history";
    while (1) {
    struct History hist;
    char* arguments[100];
    char inputString[100] ;
    printf("$ ");
    fflush(stdout); // clean buffer
    int i =0;
    // read input
    inputString[0]='\0';   
    scanf(" %[^\n]%*c", inputString);
    arguments[i] = strtok(inputString, " ");
    while(arguments[i] != NULL)
        arguments[++i] = strtok(NULL, " ");
    int k =strcmp(arguments[0],historyString);
    if(!strcmp(arguments[0],historyString) || !strcmp(arguments[0],"cd") || !strcmp(arguments[0],"exit"))
    {
        if(!strcmp(arguments[0],historyString))
        {
            if(arguments[1] == NULL)
            {
                strcpy(hist.command,"history");
                hist.pidOfCommand  = getpid();
                commands[commandNum] = hist;
                commandNum++ ;
                int j;
                for(j=0;j<commandNum;j++)
                    printf("%d %s\n", commands[j].pidOfCommand,commands[j].command);
            }
        }
        if(!strcmp(arguments[0],"cd"))
        {
            if(arguments[1] != NULL && arguments[2] == NULL)
            {
                if(chdir(arguments[1]) != 0 )
                    perror("cd failed");
                else
                {
                    strcpy(hist.command,"cd");
                    hist.pidOfCommand  = getpid();
                    commands[commandNum] = hist;
                    commandNum++ ;
                }
            }
            else
                perror("cd failed");
        }
        if(!strcmp(arguments[0],"exit"))
        {
            exit(0);
        }

        
    }
    else
    {
    pid = fork();
    if (pid == 0) 
    {    
        fflush(stdout); // clean buffer
        ret_code = execvp(arguments[0],arguments);
        if (ret_code == -1) 
        {
           //perror("%s failed", arguments[0]);
            exit(-1);
        }
    }
    else 
    {   
        strcpy(hist.command,inputString);
        hist.pidOfCommand = pid;
        commands[commandNum] = hist;
        commandNum++ ;
        waited = wait(&stat);   
    }
    }
}
}