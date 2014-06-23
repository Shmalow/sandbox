#include <stdio.h>
#include <stdlib.h>
#include <libgen.h> // For basename
#include <getopt.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <fcntl.h>

#define VERSION "0.0.1"

char *progname = NULL;
char *file = NULL;

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

	file = argv[optind];
}

int print_hexa2(const char *file) {
	int result = 0;
	int fh = open(file, O_RDONLY | O_BINARY);
	if (fh == -1) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		result = 1;
		goto cleanup;
	}

	unsigned char buffer[17];
	memset(buffer, 0, 17);
	size_t s = 0;
	int address = 0;

	while (s = read(fh, buffer, 16)) {
		if (s == -1)	{
			fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
			result = 1;
			goto cleanup;
		}
		char buffer_hexa[64] = "";
		for (int i = 0; i < s; i++) {
			char space[2] = "";
			if (i % 4 == 0) {
				snprintf(space, 2, " ");
			}
			char str[32];
			int x = (int) buffer[i];
			snprintf(str, 32, " %s%02x", space, x);
			strcat(buffer_hexa, str);
		}
		printf("%08x:%s\n", address, buffer_hexa);
		address += 16;
	}

cleanup:
	if (fh != -1) {
		close(fh);
	}
	return result;
}

int main(int argc, char** argv) {
	int result = 0;
	progname = argv[0];
	
	manage_options(argc, argv);
	
	result = print_hexa2(file);

	return result;
}
