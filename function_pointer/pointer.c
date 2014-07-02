#include <stdio.h>

typedef int (*pf1_t)(int, int);

int pointer(int a, int b) {
	printf("a=%d and b=%d\n", a, b);
	return 0;
}

int truc(int a, int b) {
	printf("truc: a=%d and b=%d\n", a, b);
	return 0;
}

int hello(pf1_t a, pf1_t b) {
	a(3, 4);
	b(2, 5);
	return 0;
}

int main() {
	pf1_t pf1;
	pf1 = &truc;
	pf1(2, 3);
	hello(truc, pointer);
	return 0;
}