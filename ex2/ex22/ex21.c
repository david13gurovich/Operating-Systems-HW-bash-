//David Gurovich 318572047

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    char c1;
    char c2;
    char buffer[150];
    char buffer2[150];
    int differece=1;
    int file1Relative =0;
    int file2Relative =0;
    int fd1;
    int fd2;
    char slash[1] ="/";
    char *pathToFile1 = getcwd(buffer,150);
    char *pathToFile2 = getcwd(buffer2,150);
    if(argv[1][0] != '/')
    {
        file1Relative =1;
        strcat(pathToFile1,"/");
        strcat(pathToFile1,argv[1]);
    }
    if(argv[2][0] != '/')
    {
        file2Relative =1;
        strcat(pathToFile2,"/");
        strcat(pathToFile2,argv[2]);
    }
    if(file1Relative)
        fd1 = open(pathToFile1, O_RDONLY);
    else
        fd1 = open(argv[1], O_RDONLY);
    if(file2Relative)
        fd2 = open(pathToFile2, O_RDONLY);
    else
        fd2 = open(argv[2], O_RDONLY);
    if (fd1 == -1 || fd2 == -1) 
    {
        write(2,"error in: open\n",15);
        exit(-1);
    }
    int read1 =read(fd1, &c1, sizeof(char));
    int read2 =read(fd2, &c2, sizeof(char));
    while(read1 != EOF && read2 == read1 && read1 != 0)
    {
        if(c1 != c2 )                      //not matching
        {
            differece =3;
            while(c1 == ' ' || c1 == '\n')      //going through every char untilthe end of file or letter
            {
                read1 = read(fd1, &c1, sizeof(char));
                if(read1 == EOF || read1 ==0)
                {
                        if(c2 != ' ' || c2 != '\n')
                            differece =2;
                        else{
                            while(c2 == ' ' || c2 == '\n')
                            {
                                read2 = read(fd2, &c2, sizeof(char));
                                if(read2 == EOF || read2 ==0)           //both files ended with spaces and \n
                                {
                                    differece = 3;
                                    break;
                                }
                            }
                        }
                }
            }
            while(c2 == ' ' || c2 == '\n')               //same methology but sttarting with the second file first
            {
                read2 = read(fd2, &c2, sizeof(char));
                if(read2 == EOF || read2 ==0)
                {
                        if(c1 != ' ' || c1 != '\n')
                            differece =2;
                        else{
                            while(c2 == ' ' || c2 == '\n')
                            {
                                read1 = read(fd1, &c1, sizeof(char));
                                if(read1 == EOF || read1 == 0)
                                {
                                    differece = 3;
                                    break;
                                }
                                char str1[2] = {c1 , '\0'};
                            }
                        }
                }
            }    
        c1 = tolower((unsigned char)c1);
        c2 = tolower((unsigned char)c2);
        char str1[2] = {c1 , '\0'};
        char str2[2] = {c2 , '\0'};
        if(strcmp(str1,str2))           //case insensitive check
        {
            differece = 2;
            break;
        }
        }
        read1 =read(fd1, &c1, sizeof(char));
        read2 =read(fd2, &c2, sizeof(char));
    }
    close(fd1);
    close(fd2);
    exit(differece);
}
