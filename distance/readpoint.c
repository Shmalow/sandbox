#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <libgen.h>
#include <getopt.h>
#include <ctype.h>

#define PATH_BUFFER 1 << 15
#define FILE_BUFFER 5
#define VERSION "0.0.1"

typedef struct {
	double x;
	double y;
} point_t;

char *progname = NULL;
char *filename = NULL;

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

	if (argc - optind != 1) {
		fprintf(stderr, "Need one parameters.\n");
		help(1);
	}

	filename = argv[optind];
}

int readpoint() {
	int result = 0;
	point_t p;

	int fd = open(filename, O_RDONLY | O_BINARY);
	if (fd == -1)	{
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	ssize_t s = 0;
	while (s = read(fd, &p, sizeof(point_t))) {
		if (s == -1)	{
			fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
			result = 1;
			goto cleanup;
		}
		
		printf("point = (%f, %f)\n", p.x, p.y);
	}

cleanup:
	if (fd != -1) {
		close(fd);
	}
	return result;
}

int main(int argc, char** argv) {
	int result = 0;
	manage_options(argc, argv);
	result = readpoint();
	
	return result;
}
