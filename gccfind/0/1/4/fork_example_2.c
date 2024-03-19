int main(){
	int i;
	for (i = 0; i < 3; i++)
		if (fork() == 0)
		   while(1);
}

