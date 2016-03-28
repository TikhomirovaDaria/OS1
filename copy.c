#include <unistd.h>
#include <stdio.h>

pid_t CreateProcess(int n);
int CopyFile (char* source, char* dest);

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

// Copy 1 file
int CopyFile (char* source, char* dest) {
    	int in, out;
    	char* buff = (char*)malloc(1024);
    	int nread;
    	in = open(source, O_RDONLY);
    	out = open (dest, O_WRONLY|O_CREAT, S_IRWXU|S_IRWXO);
    	if (!in || !out)
        	return -1;
   	 while ((nread = read(in, buff, sizeof(block))) > 0)
        	write(out,block,nread);

   	close(out);
    	close(in);
    	free(buff);
    	return 0;
}
