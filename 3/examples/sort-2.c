/* ex-2.c for directory listing using directls.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>
#include "directls.h"

/* main */
int main(int argc, char *argv[])
{
	Directory *dir;
	char buffer[MAXPATHLEN];
	int count;
	int i;

	/* Get the directory listing. */
	if (argc > 1) {
		if ((count = directls(argv[1], &dir)) < 0) {
			fprintf(stdout, "Could not read directory\n");
			return(1);
		}
	} else {
		if ((count = directls(getcwd(buffer, MAXPATHLEN), &dir)) < 0) {
			fprintf(stdout, "Could not read directory\n");
			return(1);
		}
	}

	/* Display the directory listing. */
	for (i = 0; i < count; i++) {
		fprintf(stdout, "%s\n", dir[i].name);
	}
	fprintf(stdout, "%d found \n", count);
	free(dir);

	return 0;
}
