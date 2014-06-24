#include <stdio.h>

typedef int entier;
typedef unsigned int entier_positif;

int add(int a, int b) {
	return a + b;
}

double add2(double a, double b) {
	return a + b;
}

int main() {
	int a = 2;
	int b = a == 3;
	b = (a = printf("coucou\n"));
	printf("b=%d\n", b);
	while (a != 10) {
		printf("coucou a=%d\n", a);
		a++;
	}
	printf("fini\n");
	printf("1 and 2 = %d\n", 1 && 2);
	printf("0 or 2 = %d\n", 0 || 2);
	printf("0 or 0 = %d\n", 0 || 0);
	printf("0 or 0 = %d\n", 0 || 0);
	printf("not 0 = %d\n", !0);
	printf("not 10 = %d\n", !10);
	
	switch(a) {
		case 10:
			printf("Coucou 10\n");
			break;
		case 2:
			printf("Coucou 2\n");
			break;
		case 5: 
			printf("Coucou 5\n");
			break;
		default:
			;
	}
	
	a = 2;
	b = 3;
	printf("%d + %d = %d\n", a, b, add(a, b));
	
	double c = 2.1;
	double d = 5.1;
	printf("%.1f + %.1f = %.1f\n", c, d, add2(c, d));
	entier e = 3;
	printf("e=%d\n", e);
	printf("sizeof(double)=%d\n", sizeof(double));
	printf("sizeof(float)=%d\n", sizeof(float));
	printf("sizeof(void)=%d\n", sizeof(void));
	char f = 65;
	printf("f=%c\n", f);
	return 0;
	
}