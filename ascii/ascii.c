#include <stdio.h>
#include <ctype.h>

void print_char(int i) {
	char str[4];
	if (isprint(i)) {
		snprintf(str, 4, "%c", i);
	} else {
		snprintf(str, 4, "n/a", i);
	}
	printf("%3d (0x%02X) %s\t", i, i, str);
}

int main(int argc, char** argv) {
	for (int i = 0; i < 64; i++) {
		print_char(i);
		print_char(i + 64);
		print_char(i + 128);
		print_char(i + 192);
		printf("\n");
	}
	return 0;
}
