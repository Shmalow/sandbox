#include <stdio.h>     // For printf, fopen, etc.
#include <libgen.h>    // For basename
#include <getopt.h>    // For getopt, optind, opterr, optopt
#include <ctype.h>     // For isprint
#include <errno.h>     // For errno   
#include <stdlib.h>    // For exit
#include <fcntl.h>     // For open, etc.

#define VERSION "0.0.1"
#define STARTS_WITH(s1, s2) (strncmp(s1, s2, strlen(s2)) == 0)

#define BUFFER_SIZE 1024

#define TRY(x) \
	if (x) { \
		result = 1; \
		goto cleanup; \
	}
	

char *progname = NULL;
char *hexa_str = NULL;

void version() {
	fprintf(stderr, "Version: %s\n", VERSION);
	exit(0);
}

void help(int status) {
	fprintf(stderr, "Usage: %s [-hv] <file>\n", basename(progname));
	exit(status);
}

void manage_options(int argc, char *argv[]) {
	int index;
	int c;
	
	opterr = 0;

	while ((c = getopt(argc, argv, "hv")) != -1) {
		switch (c) {
			case 'h':
				help(0);
				break;
			case 'v':
				version();
				break;
			case '?':
				if (isprint(optopt)) {
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				}
				help(1);
			default:
				help(1);
		}
	}

	if (optind != argc -1) {
		fprintf(stderr, "Need one filename.\n");
		help(1);
	}

	hexa_str = argv[optind];
}

int main(int argc, char** argv) {
	int result = 0;
	manage_options(argc, argv);
	printf("hexa_str=%s\n", hexa_str);
	if (STARTS_WITH(hexa_str, "0x")) {
		hexa_str += 2;
	}
	char buffer[BUFFER_SIZE];
	snprintf(buffer, BUFFER_SIZE, "0x%s", hexa_str);
	printf("buffer=%s\n", buffer);
	int y = 0;
	sscanf(buffer, "%x", &y);
	printf("y=%d\n", y);
	
cleanup:
	return result;
}

