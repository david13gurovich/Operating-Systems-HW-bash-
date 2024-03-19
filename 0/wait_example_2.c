#include <stdio.h>
#include <sys/types.h>

int main()
{
	int status;
	pid_t pid, pid1, pid2;
	if ((pid1 = fork()) == 0)
		printf("in child 1\n");
	else
		if ((pid2 = fork()) == 0)
			printf("in child 2\n");
		else
		{
			pid = wait(&status);
			if (pid == pid1)
				printf("child 1 finished\n");
			if (pid == pid2)
				printf("child 2 finished\n");
			pid = wait(&status);
			if (pid == pid1)
				printf("child 1 finished\n");
			if (pid == pid2)
				printf("child 2 finished\n");
		}
}

