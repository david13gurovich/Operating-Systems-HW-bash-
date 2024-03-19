#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

void done(){	
	printf("see ya!\n");
}
int main(){
	int status;	
	atexit(done);	
	if(fork())	
	{		
		wait(&status);		
		printf("parent PID = %d\n",getpid());		
		printf("exit status= %d\n",WEXITSTATUS(status));		_Exit(73);	
	}else{		
		sleep(2);		
		printf("child PID = %d\n",getpid());		
		exit(55);
	}
}
