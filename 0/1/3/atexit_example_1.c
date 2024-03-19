#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
 void fnExit1 (void) { 
	puts ("Exit function 1."); 
} 

void fnExit2 (void) { 
	puts ("Exit function 2."); 
} 

int main () { 
	atexit (fnExit1); 
	atexit (fnExit2); 
	atexit (fnExit2);
	puts ("Main function."); 
	return 0; 
}

