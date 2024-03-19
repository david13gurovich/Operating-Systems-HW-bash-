#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
 
void bye(void) { 
	printf("Let’s go home, we learned enough today\n"); 
} 

int main(void) 
{ 
	long a;
	int i; 
	a = sysconf(_SC_ATEXIT_MAX); 
	printf("ATEXIT_MAX = %ld\n", a); 
	i = atexit(bye); 
	if (i != 0) 
	{ 
		fprintf(stderr, "cannot set exit function\n"); 
		exit(EXIT_FAILURE); 
	} 
	exit(EXIT_SUCCESS);
}

