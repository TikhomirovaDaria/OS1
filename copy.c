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

pid_t CreateProsess(int n);
int CopyFile (char* source, char* dest);
void MakePath(char* currentDir, char* nextDir, char* result);
int CopyDirectory (char* source, char* dest);
void SignalHandler(int sig);
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

    char* source = (char*)malloc(1024);
    char* dest = (char*)malloc(1024);
    int status;
	
    pid_t pid = CreateProsess(proc);

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
   	 while ((nread = read(in, buff, sizeof(block))) > 0)
     	write(out,block,nread);

   	close(out);
    close(in);
    free(buff);
    return 0;
}

// Make path to inside directory
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
	if(mkdir(dest, S_IRWXU|S_IRWXO) == -1
		&& errno == ENOENT)
		return -1;

    char* fSource = (char*)malloc(1024);
    char* fDest = (char*)malloc(1024);

    struct dirent* catalog;
    while((catalog = readdir(dirSource))!=NULL) {
        if (!strcmp(catalog->d_name,".") || !strcmp(catalog->d_name,".."))
            continue;

        MakePath(source, catalog->d_name, fSource);
        MakePath(dest, catalog->d_name, fDest);

        if (catalog->d_type == DT_DIR)
            CopyDirectory(fSource, fDest);

		if (access(fDest,0)==-1){
        	CopyFile(fSource, fDest);
        	count++;
		}
    }

    free(fSource);
    free(fDest);
    return 0;
}

// Handler
void SignalHandler(int sig){
	if(getpid() != getpgrp())
		printf ("PROCESS[%d]: %d files copied\n", getpid(), count);
}
