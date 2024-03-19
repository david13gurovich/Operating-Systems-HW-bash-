#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void){
	pid_t	pid;
	if ((pid = fork()) < 0) 
		printf("fork error");	
	else	{ 
		if (pid == 0) {	/* first child */
			printf("first child\n");
			if ((pid = fork()) < 0)
				printf ("fork error");
			else {
				if (pid > 0){
				/* parent from second fork == first child */
					if (waitpid(pid, NULL, 0) != pid)
						printf("waitpid error");
					exit(0);	
				}
				/*  We're the second child; */
				sleep(2);
				printf("second child, parent pid = %d\n", getppid());
				exit(0);
			}
		}
		if (waitpid(pid, NULL, 0) != pid)	/* wait for first child */
			printf("waitpid error");
		/** We're the parent (the original process); we continue executing, knowing that we're not the parent of the second child.*/
		printf("original parent done\n");
		exit(0);
	}
}

