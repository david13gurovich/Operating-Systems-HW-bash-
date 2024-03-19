#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
        pid_t val;
        printf("PID before fork: %d\n",(int)getpid());
	val = fork();
        if(val>0)
        		printf("parent PID: %d\n",(int)getpid());
        else if(val ==0)
        		printf("child PID: %d\n",(int)getpid());
	else {
		//if val==-1 print error to screen
	}
}

