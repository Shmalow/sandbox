#include <stdio.h>
#include <string.h>

char *g_str = "Good morning!";

int main() {
	int a = 0;
	char buffer[10];
	
	memcpy(buffer, g_str, strlen(g_str) + 1);

	printf("buffer=%s\n", buffer);
	printf("a=%08X\n", a);
	
	return 0;
}