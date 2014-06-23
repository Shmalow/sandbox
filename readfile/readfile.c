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

#define PATH_BUFFER 1 << 15
#define FILE_BUFFER 5
#define VERSION "0.0.1"

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

int readfile() {
	int result = 0;
	char buffer[FILE_BUFFER + 1];
	FILE *fp;

	fp = fopen(filename,"r"); 
	if (fp == NULL)	{
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	size_t s = 0;
	
	while (s = fread(buffer, sizeof(char), FILE_BUFFER, fp)) {
		buffer[s] = 0;
		printf("%s",buffer);
		//Sleep(2000);
	}
	if (ferror(fp)) {
		fprintf(stderr, "Error while reading the file.\n");
		result = 1;
		goto cleanup;
	}

cleanup:
	if (fp) {
		fclose(fp);
	}
	return result;
}

int readfile2() {
	int result = 0;
	char buffer[FILE_BUFFER + 1];

	int fd = open(filename, O_RDONLY | O_BINARY);
	if (fd == -1)	{
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	ssize_t s = 0;
	while (s = read(fd, buffer, FILE_BUFFER)) {
		if (s == -1)	{
			fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
			result = 1;
			goto cleanup;
		}
		buffer[s] = 0;
		printf("%s",buffer);
		//Sleep(2000);
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
	result = readfile2();
	
	return result;
}