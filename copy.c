#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>

pid_t CreateProcess(int n);
int CopyFile (char* source, char* dest);
void MakePath(char* currentDir, char* nextDir, char* result);
int CopyDirectory (char* source, char* dest);
void SignalHandler(int sig);
int _mkdir (char* dir);
int count = 0;


int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf ("Wrong input\n");
		return 0;
	}
	int proc = atoi(argv[1]);

    signal (SIGUSR1, SignalHandler);
	printf("MAIN PROCCESS PID: %d\n\n", getpid());

    char* source = (char*)malloc(512);
    char* dest = (char*)malloc(512);
    int status;
	
    pid_t pid = CreateProcess(proc);

    if (!pid) {
		if(CopyDirectory(argv[2], argv[3])){
			printf("Error! PID process: %d\n", getpid());
			exit(-1);
		}
 		kill(getpid(), SIGUSR1);		
		exit(0);
    }

	for(int i=0; i<proc; i++)
		wait(NULL);
    free (source);
    free (dest);
    return 0;
}
// Initial process creates exactly n child-processes
pid_t CreateProcess(int n) {
	pid_t pid = fork();
    for (int i=0; i < n-1 && pid > 0; i++)
    	pid = fork();

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
   	 while ((nread = read(in, buff, sizeof(buff))) > 0)
     	write(out,buff,nread);

   	close(out);
    close(in);
    free(buff);
    return 0;
}

// Make string-path to inside directory
void MakePath(char* currentDir, char* nextDir, char* result){
    strcpy(result, currentDir);
    if (currentDir[strlen(currentDir) - 1] != '/')
       	strcat(result, "/");
    strcat(result, nextDir);
}

// Copy directories recoursively
int CopyDirectory (char* source, char* dest) {
    DIR* dirSource = opendir(source);
    if (!dirSource)
        return -1;
	if(_mkdir(dest) == -1
		&& errno == ENOENT)
		return -1;

    char* fSource = (char*)malloc(512);
    char* fDest = (char*)malloc(512);

    struct dirent* catalog;
    while((catalog = readdir(dirSource))!=NULL) {
        if (!strcmp(catalog->d_name,".") || !strcmp(catalog->d_name,".."))
            continue;

        MakePath(source, catalog->d_name, fSource);
        MakePath(dest, catalog->d_name, fDest);

        if (catalog->d_type == DT_DIR)
            CopyDirectory(fSource, fDest);

		if (access(fDest,0)==-1){
        	if(!CopyFile(fSource, fDest));
        		count++;
		}
    }

    free(fSource);
    free(fDest);
    return 0;
}
//Create path to inside directories
int _mkdir(char *dir) {
	int result = mkdir(dir, S_IRWXU|S_IRWXO);
	if (result == -1) {
		if (errno == EEXIST)
			return 0;
		 
    	char* tmp = (char*)malloc(512);
    	char *p = NULL;
    	size_t len;

    	strcpy(tmp, dir);
    	len = strlen(tmp);
    	if(tmp[len - 1] == '/')
        	tmp[len - 1] = 0;
        	for(p = tmp + 1; *p; p++)
            	if(*p == '/') {
                	*p = 0;
                	mkdir(tmp, S_IRWXU|S_IRWXO);
                	*p = '/';
             	}
    	result = mkdir(tmp, S_IRWXU|S_IRWXO);
    	free(tmp);
    	return result;
	}
}

// Handler
void SignalHandler(int sig){
	if(getpid() != getpgrp())
		printf ("PROCESS[%d]: %d files copied\n", getpid(), count);
}
