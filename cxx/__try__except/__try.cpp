//#include <stdio.h>
//#include <windows.h>

int main() {
	__asm {
			xor edx, edx
			xor edx, edx
			xor edx, edx
			xor edx, edx
			xor edx, edx
	}
	//printf("Hello\n");
	int c = 1;
	int a = 3;
	int b = 0;
	__try {
		c = a / b;
		//printf("%d/%d=%d", a, b, c);
	} __except (1) {
		__asm {
			xor ebx, ebx
			xor ebx, ebx
			xor ebx, ebx
			xor ebx, ebx
			xor ebx, ebx
		}
		//printf("Exception catched.\n");
	}
		
	return 0;
}

