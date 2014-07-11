#include <stdio.h>

int divide(int a, int b) {
	int c = 0;
	if (b == 0) {
		throw 32;
	}
	c = a / b;
	
	return c;
}

int main() {
	int c = 0;
	int a = 3;
	int b = 0;
	
	try {
		c = divide(a, b);
		printf("coucou\n");
	} catch (const char *e) {
		printf("Exception catched: %s\n", e);
	} catch (int x) {
		printf("Exception catched: %d\n", x);
	} catch (...) {
		printf("Exception catched.\n");
	}
	
	return 0;
}