#include<stdio.h>
#include<unistd.h>

int main() {
 	char *argv[] = {"date", NULL};
	execv("/bin/date", argv);
	printf("hello");
	return 0;
}

