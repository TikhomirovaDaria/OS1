#include <unistd.h>
#include <stdio.h>

pid_t CreateProcess(int n);

int main () {
	char* source = (char*)malloc(1024);
	char* dest = (char*)malloc(1024);

	printf("Enter source: ");
    	scanf ("%s", source);
    	printf("Enter dest: ");
    	scanf ("%s", dest);


	free(sourse);
	free(dest);
	return 0;
}

// Initial process creates exactly n child-processes
pid_t CreateProcess(int n) {
	pid_t pid = fork();
    	for (int i=0; i < n-1 && pid > 0; i++){
       		pid = fork();

    	}
    	return pid;
}
