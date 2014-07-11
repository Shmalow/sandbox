#include <stdio.h>

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[37m"


int main () {
	int result = 0;
	printf("%sHell%so Wo%srld!%s", BLUE, WHITE, RED, RESET);

	return result;
}
