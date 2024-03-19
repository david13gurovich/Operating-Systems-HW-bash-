//David Gurovich 318572047

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h> 


int compareFiles(char * pathToCorrect,char* pathToResult)
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) 
    {    
        char *argv[] = {"./comp.out", pathToCorrect, pathToResult,  NULL};
        int ret_code = execvp("./comp.out", argv);
        if (ret_code == -1) 
            exit(-1);
    }
    else 
    {   
        wait(&status);
        return status;
    }


}

int runCFile(char* cFileName,char *pathToExecution,char* argumentsFrom,char* rightOutputFrom)
{
    int bytesRead;
    char buffer[150];
    int errorInWrite = 0;
    char * pwdEx = getcwd(buffer,150);
    char  pathToNewResultFile[150];
    strcpy(pathToNewResultFile,pwdEx);
    char slash[4] = "/";
    strcat(pwdEx,slash);
    strcat(pwdEx,rightOutputFrom);
    int fdCheck = open(pwdEx,O_RDONLY);
    if(fdCheck == -1)
    {
        if(write(2,"Output file not exist\n",22) == -1)
            errorInWrite = -1;
        exit(-1);
    }
    close(fdCheck);
    strcat(pathToNewResultFile,"/result.txt");
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) 
    {    
        int fdArgs = open(argumentsFrom,O_RDONLY);
        if(fdArgs == -1)
        {
            if(write(2,"Input file not exist\n",21) == -1)
                errorInWrite = -1;
            exit(-1);
        }
        dup2(fdArgs,0);
        int fdResult = open(pathToNewResultFile,O_CREAT | O_RDWR,S_IRWXU);
        dup2(fdResult,1);
        char *argv[] = {pathToExecution,NULL};
        int ret_code = execvp(pathToExecution, argv);
        close(fdArgs);
        close(fdResult);
    }
    else 
    {   
        wait(&status);
        if( status != 0)
        {
            perror("error in: fork\n");

        }   
        int isDifferent = compareFiles(pwdEx,pathToNewResultFile); // 1 -same , 2-different ,3 -similar
        if (remove(pathToNewResultFile) != 0) 
             if(write(2,"error in: remove\n",17) == -1)
                        errorInWrite = -1;
        return isDifferent;
        //to csv
    }
}

void eachSubDir(struct dirent *pDirent,const char* pathName,char * argumentsFrom,char* rightOutputFrom,int fdResults)
{
    DIR *subDir; 
    int errorInWrite=0;
    char  changingPath[150];
    strcpy(changingPath,pathName);
    int isCFileExists = 0;
    char  cFileName[150];
    int result;
	struct dirent *pDirentOfSubDir; 
    if(pDirent->d_type == 4)     //pDirent->d_type = 4 : means it's a directory
    {
        strcat(changingPath,"/");
        strcat(changingPath,pDirent->d_name);
        if ( (subDir = opendir(changingPath)) == NULL)
        {
            if(write(2,"error in: opendir\n",18) == -1)
                        errorInWrite = -1; 
        }
        int count =0;
	    while ( (pDirentOfSubDir = readdir(subDir) ) != NULL ) 
        {
            if(count>=2)                      //bypassing "." and ".."
            {
                int length = strlen(pDirentOfSubDir->d_name);
                if(pDirentOfSubDir->d_name[length-2] == '.' && pDirentOfSubDir->d_name[length-1] == 'c')
                {
                    isCFileExists=1;
                    strcpy(cFileName,pDirentOfSubDir->d_name);
                    break; 
                }
            }
            count++;
        }
        if(isCFileExists)
        {
            char buffer[150];
            char* comeBackToDir =getcwd(buffer,150);
            char  outEnding[1500] = "/a.out";
            char  pathToExecution[150] = "./";
            if(chdir(changingPath) == -1)
                if(write(2,"error in: chdir\n",16) == -1)
                        errorInWrite = -1;
            strcat(changingPath,outEnding);
            strcat(pathToExecution,changingPath);
            pid_t pid;
            int status;
            pid = fork();
            if (pid == 0) 
            {    
                char *argv[] = {"gcc", "-o", "a.out", cFileName, NULL};
                int ret_code = execvp("gcc", argv);
            }
            else 
            {   
                wait(&status);
                if(chdir(comeBackToDir) == -1)
                     if(write(2,"error in: chdir\n",16) == -1)
                        errorInWrite = -1;
                if(status != 0)
                {
                    char  lineInBuffer[150];
                    strcpy(lineInBuffer,pDirent->d_name);
                    char noCFile[150] = ",10,COMPILATION_ERROR\n";
                    strcat(lineInBuffer,noCFile); 
                    if(write(fdResults,lineInBuffer,150) == -1)
                        errorInWrite =-1;
                }   
                else
                {
                    char  lineInBuffer[150];
                    strcpy(lineInBuffer,pDirent->d_name);
                    result = runCFile(cFileName,pathToExecution,argumentsFrom,rightOutputFrom);
                    if(result == 1 || result == 256)
                    {
                        char exellent[150] = ",100,EXCELLENT\n";
                        strcat(lineInBuffer,exellent); 
                        if(write(fdResults,lineInBuffer,150) == -1)
                            errorInWrite =-1;
                    }
                    if(result == 2 || result == 512)
                    {
                        char wrong[150] = ",50,WRONG\n";
                        strcat(lineInBuffer,wrong); 
                        if(write(fdResults,lineInBuffer,150) == -1)
                            errorInWrite =-1;
                    }
                    if(result == 3 || result == 768)
                    {
                        char similar[150] = ",75,SIMILAR\n";
                        strcat(lineInBuffer,similar); 
                        if(write(fdResults,lineInBuffer,150) == -1)
                            errorInWrite =-1;
                    }

                }
            }

        }
        else
        {                                       //adding line to csv - 0,no c file
            char  lineInBuffer[150];
            strcpy(lineInBuffer,pDirent->d_name);
            char noCFile[150] = ",0,NO_C_FILE\n";
            strcat(lineInBuffer,noCFile); 
            if(write(fdResults,lineInBuffer,150)== -1)
                errorInWrite=-1;
        }
	    if(closedir( subDir ) == -1)
        {
             if(write(2,"error in: closedir\n",19) == -1)
                errorInWrite = -1;
        }
    }
}

int main(int argc, char* argv[])
{
    DIR *pDir; 
	struct dirent *pDirent; 
    char  *line1;
    char  *line2;
    char  *line3;
    char delimeter = '\n';
    int bytesRead;
    char c[150];
    int writeError =0;
    char buffer[150];
    char * pathToErrorFile = getcwd(buffer,150);
    strcat(pathToErrorFile,"/errors.txt");
    int fdErrors = open(pathToErrorFile,O_CREAT | O_RDWR,S_IRWXU);
    if (fdErrors == -1 ) 
    {
        if(write(2,"error in: open\n",15) == -1)
            writeError = -1;
        exit(-1);
    }
    char * pathToResultFile = getcwd(buffer,150);
    strcat(pathToResultFile,"/results.csv");
    int fdResults = open(pathToResultFile,O_CREAT | O_RDWR,S_IRWXU);
    if (fdResults == -1 ) 
    {
        if(write(2,"error in: open\n",15) == -1)
            writeError = -1;
        exit(-1);
    }
    dup2(fdErrors,2);
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1 ) 
    {
        if(write(2,"error in: open\n",15) == -1)
            writeError = -1;
        exit(-1);
    }
    bytesRead = read(fd, c, 1500);
    line1 = strtok(c,"\n");
    line2 = strtok(NULL,"\n");
    line3 = strtok(NULL,"\n");
	if ( (pDir = opendir(line1)) == NULL)
    {
        if(write(2,"Not a valid directory\n",23) == -1)
            writeError = -1;
		exit(-1); 
    }

	// looping through the directory, printing the directory entry name
    int count =0;
	while ( (pDirent = readdir(pDir) ) != NULL ) 
    {
        if(count>=2)                      //bypassing "." and ".."
		    eachSubDir(pDirent,line1,line2,line3,fdResults); 
        count++;
    }
    if(write(fdResults,"\n",1) == -1)
        writeError =-1;
	closedir( pDir );
    close(fdResults);
    close(fdErrors);
    close(fd);

}