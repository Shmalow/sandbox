#include <stdio.h>
//avec libseh

#define JULIA(a)         a ## coucou();
#define TO_STR(e) #e
#define TTO_STR(e) TO_STR(e)

#define BUFFER_SIZE 1024

#define DEBUG(message, ...)	printf("DEBUG [" __FILE__ ":" TTO_STR(__LINE__) " (%s)]: " message "\n", __FUNCTION__, ##__VA_ARGS__); fflush(stdout)

void hello_coucou() {
	printf("hello coucou\n");
	DEBUG("This is my message");
}

void julia_coucou() {
	printf("hello julia\n");
	DEBUG("This is my message");
}

int add(int a, int b) {
	return a + b;	
}

#define HELLO(message) { \
	__label__ cleanup; \
	printf("start hello %s\n", message); \
	if (0) goto cleanup; \
cleanup: \
	printf("cleanup %s\n", message); \
}

int main() {
	printf("coucou " TO_STR(julia) TO_STR(\n));
	JULIA(hello_);
	JULIA(julia_);
	DEBUG("This is my message %s %d", "julia", 36);
	HELLO("coucou");
	HELLO("hello");
	if (__builtin_constant_p(add(2,3))) {
		DEBUG("This is a builtin constant");
	}
	goto cleanup;
cleanup:
	printf("main cleanup %s\n");
	return 0;	
}
