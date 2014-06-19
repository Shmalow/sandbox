#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

char *basename(char *progname);
void exit(int status);


char *progname = NULL;
char *param1 = NULL;
char *param2 = NULL;
	
void help(int status) {
	fprintf(stderr, "Usage: %s [-xh] <file>\n", basename(progname));
	exit(status);
}

void manage_options(int argc, char *argv[]) {
	int index;
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "abcd:")) != -1) {
		switch (c) {
			case 'a':
				printf("I have received a.\n");
				break;
			case 'b':
				printf("I have received b.\n");
				break;
			case 'c':
				printf("I have received c.\n");
				break;
			case 'd':
				printf("I have received d with argument %s.\n", optarg);
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

	if (argc - optind != 2) {
		fprintf(stderr, "Need two parameters.\n");
		help(1);
	}

	param1 = argv[optind];
	param2 = argv[optind + 1];
}
	
int main(int argc, char *argv[]) {
	progname = argv[0];
	printf("argc=%d\n", argc);
	for (int i = 0; i < argc; i++) {
		printf("argv[%d]=%s\n", i, argv[i]);
	}
	manage_options(argc, argv);
	printf("param1=%s\n", param1);
	printf("param2=%s\n", param2);
	return 0;
}
