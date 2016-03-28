#include <unistd.h>
#include <stdio.h>

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
