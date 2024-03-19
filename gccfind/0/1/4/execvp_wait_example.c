#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int stat,waited,ret_code;
	pid_t pid;
	pid = fork();
	if (pid == 0) 
	{  /* Child */ 
		ret_code = execvp(argv[1],&argv[1]);  
		if (ret_code == -1) 
		{
			perror("exec failed ");
			exit(-1);
		}
		else 
			printf("Banana");
	}
	else 
	{  /* Parent */ 
		printf("Father: after fork, son proc id is %d \n",pid);
		waited = wait(&stat);   /* stat can tell what happened */ 
		printf("Father: Son proc completed,  id is %d \n", waited);
	}
}
