#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>


#include "ar_reader.h"




int read_archive() {
	int result = 0;
	ar_signature signature;
	memset(&signature, 0, sizeof(ar_signature));

	int fd = open(filename, O_RDONLY | O_BINARY);
	if (fd == -1)	{
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	ssize_t s = read(fd, &signature, sizeof(ar_signature));
	if (s == -1)	{
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}
	char buffer[sizeof(ar_signature) + 1];
	memset(buffer, 0, sizeof(ar_signature) + 1);
	memcpy(buffer, &signature, sizeof(ar_signature));
	printf("signature = |%s|\n", buffer);
	
	if (strcmp(buffer, IMAGE_ARCHIVE_START) == 0) {
		printf("This seems to be an archive file.\n");
	} else {
		printf("This is not an archive file.\n");
	}

cleanup:
	if (fd != -1) {
		close(fd);
	}
	return result;
}