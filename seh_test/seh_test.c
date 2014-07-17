#include <windows.h>
#include <stdio.h>
#include <seh.h>       /* The LibSEH header needs to be included */

int ExceptionFilter(unsigned int code, unsigned int excToFilter) {
	if(code == excToFilter) {
		return EXCEPTION_EXECUTE_HANDLER;
	} else {
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

int main() {
	int x = 0;
	int y = 4;
	__seh_try {  /* __try becomes __seh_try */
		y /= x;
	}
	__seh_except(ExceptionFilter(GetExceptionCode(), EXCEPTION_INT_DIVIDE_BY_ZERO)) {
		printf("Divide by zero exception.\n");
	}
	__seh_end_except   /* This must terminate all __seh_except blocks */ 

	return 0;
}
