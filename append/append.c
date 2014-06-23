#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	if (argc <= 1) {
		printf("Need one argument.");
		return 0;
	}
	char buffer[10] = "Hello ";
	int i = 0;
	strcat(buffer, argv[1]);
	printf("%s\n", buffer);
	printf("%d\n", i);
	return 0;
}