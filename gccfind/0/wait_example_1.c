#include <unistd.h>
#include <sys/types.h>
#include<stdio.h>
int main()
{
	pid_t pid;
	int stat;
	if ((pid = fork()) == 0)
		printf("1\n"); else 	{
		wait(&stat);
		printf("2\n");
	}
}

