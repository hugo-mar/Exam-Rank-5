#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void bsq(FILE* input) {

	char*	line = NULL;
	size_t	cap = 0;
	ssize_t	n;

	while((n = getline(&line, &cap, input)) != -1) {

		// map parsing
		// process bsq

	}

	free(line);
}


int main(int argc, char** argv) {

	if (argc == 1) {
		bsq(stdin);
	}
	else {
		for (int i = 1; i < argc; ++i) {
			FILE* f = fopen(argv[i], "r");
			if (!f) 
				fputs("map error\n", stderr);
			else {
				bsq(f);
				fclose(f);
			}

			if (i < argc - 1)
				fputs("\n", stdout);
		}
	}

	return 0;
}