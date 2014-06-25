#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "ar_reader.h"

#define PARSE_FIELD(x) parse_field(&x, sizeof(x))



char buffer[BUFFER_SIZE];

char *parse_field(const void *field, size_t s) {
	memset(buffer, 0, BUFFER_SIZE);
	memcpy(buffer, field, s);
	return buffer;
}

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
	
	memset(buffer, 0, BUFFER_SIZE);
	memcpy(buffer, &signature, sizeof(ar_signature));
	printf("signature = |%s|\n", buffer);
	
	if (strcmp(buffer, IMAGE_ARCHIVE_START) == 0) {
		printf("This seems to be an archive file.\n");
	} else {
		printf("This is not an archive file.\n");
	}
	
	printf("sizeof(IMAGE_ARCHIVE_MEMBER_HEADER)=%d\n", sizeof(IMAGE_ARCHIVE_MEMBER_HEADER));
	IMAGE_ARCHIVE_MEMBER_HEADER member_header;
	s = read(fd, &member_header, sizeof(IMAGE_ARCHIVE_MEMBER_HEADER));
	
	printf("----------first linker member\n");
	printf("---header\n");

	printf("Name: %s\n", PARSE_FIELD(member_header.Name));
	time_t t = atoi(PARSE_FIELD(member_header.Date));
	struct tm *tm = localtime(&t);
	printf("Date: %04d/%02d/%02d - %02d:%02d:%02d (%u)\n",
		(1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (unsigned int) t);
	
	printf("UserID: %s\n", PARSE_FIELD(member_header.UserID));
	printf("GroupID: %s\n", PARSE_FIELD(member_header.GroupID));
	printf("Mode: %s\n", PARSE_FIELD(member_header.Mode));
	printf("Size: %s\n", PARSE_FIELD(member_header.Size));
	printf("EndHeader: %02X %02X\n", member_header.EndHeader[0],  member_header.EndHeader[1]);
	
cleanup:
	if (fd != -1) {
		close(fd);
	}
	return result;
}