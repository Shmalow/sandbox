#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>


typedef struct {
	double x;
	double y;
} point_t;

double calculate(point_t p1, point_t p2) {
	double a = p1.x - p2.x;
	double b = p1.y - p2.y;
	double c = sqrt((a*a) + (b*b));

	return c;
}

int save(const char *filename, int nbr, ...) {
	int result = 0;
	
	int fd = open(filename, O_WRONLY | O_CREAT | O_BINARY);
	if (fd == -1)	{
		fprintf(stderr, "There is an error (open). errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}
	
	va_list params;
	va_start(params, nbr);
	
	
	for (int i = 0; i < nbr; i++) {
		point_t p = va_arg(params, point_t);
		ssize_t s = write(fd, &p, sizeof(point_t));
		if (s == -1)	{
			fprintf(stderr, "There is an error (write). errno=%d (%s)\n", errno, strerror(errno));
			result = 1;
			goto cleanup;
		}
	}
	va_end(params);

cleanup:
	if (fd != -1) {
		close(fd);
		chmod(filename, 0644);
	}
	return result;
}

int main() {
	int result = 0;
	point_t q1;
	q1.x = 4;
	q1.y = 1;
	point_t q2;
	q2.x = 2;
	q2.y = 3;
	
	printf("The distance between points a(%.1f,%.1f) and b(%.1f,%.1f) is %.2f\n", q1.x, q1.y, q2.x, q2.y, calculate(q1, q2));
	printf("sizeof(point_t)=%d\n", sizeof(point_t));
	result = save("kiki.pt", 3, q1, q2, q2);
	return result;
}
	