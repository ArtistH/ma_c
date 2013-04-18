#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char *string = NULL;

	char *malloc_string = (char *)malloc(10);
	memset(malloc_string, 'a', 10);

	string = malloc_string;

	printf("%s %s\n", string, malloc_string);

	free(string);
	return 0;
}
