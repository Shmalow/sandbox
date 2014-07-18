#include <stdio.h>

typedef enum {
        ExceptionContinueExecution=3,
        ExceptionContinueSearch,
        ExceptionNestedException,
        ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;

int main() {
	printf("ExceptionContinueExecution=%d\n", ExceptionContinueExecution);
	printf("ExceptionContinueSearch=%d\n", ExceptionContinueSearch);
	printf("ExceptionCollidedUnwind=%d\n", ExceptionCollidedUnwind);
	return 0;
}