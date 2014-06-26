#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "ar_reader.h"

#define PARSE_FIELD(x) parse_field(&x, sizeof(x))

#define TRY(x) \
	if (x) { \
		result = 1; \
		goto cleanup; \
	}
	
#define FREE(x) \
	if (x) { \
		free(x); \
		x = NULL; \
	}
	
#define READ(buffer, size) \
	read(g_fd, buffer, size); \
	if (s == -1) { \
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno)); \
		result = 1; \
		goto cleanup; \
	}
	
char g_buffer[BUFFER_SIZE];
off_t next_header_offset = 8;
off_t last_header_offset = 0;

int g_fd = 0;

int inverse_endian(int x) {
	int result = 0;
	result = x >> 24;
	result += (x & 0x00ff0000) >> 8;
	result += (x & 0x0000ff00) << 8;
	result += (x & 0x000000ff) << 24;
	return result;
}

char *trim(char *str) {
  char *end = NULL;

	// Trim leading space
	while (isspace(str[0])) {
		str++;
	}
	
	if(str[0] == 0) { // All spaces?
		return str;
	}
	
	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace(end[0])) {
		end--;
	}
	// Write new null terminator
	end[1] = 0;
	
	return str;
}

char *parse_field(const void *field, size_t s) {
	memset(g_buffer, 0, BUFFER_SIZE);
	memcpy(g_buffer, field, s);
	trim(g_buffer);
	return g_buffer;
}

int ar_parse_header(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	printf("next_header_offset=0x%08X\n", (int) next_header_offset);
	off_t o = lseek(g_fd, next_header_offset, SEEK_SET);
	if (o == -1)	{
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}
	ssize_t s = READ(pmember_header, sizeof(IMAGE_ARCHIVE_MEMBER_HEADER));
	
	if (s < sizeof(IMAGE_ARCHIVE_MEMBER_HEADER)) {
		result = EOF;
		printf("End of file.\n");
		goto cleanup;
	}
	printf("---Header\n");

	printf("Name: %s\n", PARSE_FIELD(pmember_header->Name));
	time_t t = atoi(PARSE_FIELD(pmember_header->Date));
	struct tm *tm = localtime(&t);
	printf("Date: %04d/%02d/%02d - %02d:%02d:%02d (%u)\n",
		(1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (unsigned int) t);
	
	printf("UserID: %s\n", PARSE_FIELD(pmember_header->UserID));
	printf("GroupID: %s\n", PARSE_FIELD(pmember_header->GroupID));
	printf("Mode: %s\n", PARSE_FIELD(pmember_header->Mode));
	printf("Size: %s\n", PARSE_FIELD(pmember_header->Size));
	printf("EndHeader: %02X %02X\n", pmember_header->EndHeader[0],  pmember_header->EndHeader[1]);
	
	last_header_offset = next_header_offset;
	next_header_offset += sizeof(IMAGE_ARCHIVE_MEMBER_HEADER) + atoi(PARSE_FIELD(pmember_header->Size));
	if (next_header_offset % 2 == 1) {
		next_header_offset++;
	}
	
cleanup:
	return result;
}

int ar_parse_first_linker_member(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	unsigned int *offset = NULL;
	char *symbol_name = NULL;
	
	printf("----------First Linker Member\n");
	unsigned int nbr_be;
	ssize_t s = READ(&nbr_be, 4);
	
	unsigned int nbr = inverse_endian(nbr_be);
	printf("Symbol Nbr: %u\n", nbr);
	
	offset = (unsigned int *) malloc(sizeof(unsigned int) * nbr);
	s = READ(offset, nbr * 4);
	
	for (int i = 0; i < nbr; i++) {
		offset[i] = inverse_endian(offset[i]);
		printf("offset[%d]: %d\n", i, offset[i]);
	}
	
	int size = atoi(PARSE_FIELD(pmember_header->Size));
	size_t symbol_size = sizeof(char) * (size - 4 - (nbr * 4));
	symbol_name = (char *) malloc(symbol_size);
	s = READ(symbol_name, symbol_size);
	
	char *cursor = symbol_name;
	int i = 0;
	while (cursor < symbol_name + symbol_size) {
		printf("symbol[%d]=%s\n", i, cursor);
		i++;
		cursor += strlen(cursor) + 1;
	}
	
cleanup:
	FREE(offset);
	FREE(symbol_name);
	return result;
}

int ar_parse_second_linker_member(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	unsigned int *offset = NULL;
	unsigned short int *indice = NULL;
	char *string = NULL;
	
	printf("----------Second Linker Member\n");
	unsigned int member_nbr;
	ssize_t s = READ(&member_nbr, 4);
	printf("Member Nbr:%d\n", member_nbr);

	size_t size = sizeof(unsigned int) * member_nbr;
	offset = (unsigned int *) malloc(size);
	s = READ(offset, size);
	for (int i = 0; i < member_nbr; i++) {
		printf("offset[%d]=0x%08X\n", i, offset[i]);
	}
	
	unsigned int symbol_nbr;
	s = READ(&symbol_nbr, 4);
	printf("Symbol Nbr:%d\n", symbol_nbr);
	
	size = sizeof(unsigned short int) * symbol_nbr;
	indice = (unsigned short int *) malloc(size);
	s = READ(indice, size);
	for (int i = 0; i < symbol_nbr; i++) {
		printf("indice[%d]=%d\n", i, indice[i]);
	}
	
	
	int total_size = atoi(PARSE_FIELD(pmember_header->Size));
	size = total_size - (4 + 4 * member_nbr + 4 + 2 * symbol_nbr);
	string = (char *) malloc(size);
	s = READ(string, size);
	char *cursor = string;
	int i = 0;
	while (cursor < string + size) {
		printf("string[%d]=%s\n", i, cursor);
		i++;
		cursor += strlen(cursor) + 1;
	}
	
	
cleanup:
	FREE(offset);
	FREE(indice);
	FREE(string);
	return result;
}

int ar_parse_object_member(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	
	printf("----------Object Member\n");
	
//cleanup:
	return result;
}

int read_archive() {
	int result = 0;
	ar_signature signature;
	memset(&signature, 0, sizeof(ar_signature));

	g_fd = open(filename, O_RDONLY | O_BINARY);
	if (g_fd == -1) {
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	ssize_t s = READ(&signature, sizeof(ar_signature));
	
	printf("signature = |%s|\n", PARSE_FIELD(signature));
	
	if (strcmp(g_buffer, IMAGE_ARCHIVE_START) == 0) {
		printf("This seems to be an archive file.\n");
	} else {
		printf("This is not an archive file.\n");
	}
	
	IMAGE_ARCHIVE_MEMBER_HEADER member_header;
	int first_time = 1;
	while ((result = ar_parse_header(&member_header)) == 0) {
		char *name = PARSE_FIELD(member_header.Name);
		if (strcmp(name, "/") == 0) {
			if (first_time) {
				first_time = 0;
				TRY(ar_parse_first_linker_member(&member_header));
			} else {
				TRY(ar_parse_second_linker_member(&member_header));
			}
		} else if (strcmp(name, "//") == 0) {
			// TODO: manage the case of the longname.
		} else if ((name[0] == '/') && (atoi(name + 1) != 0)) { // Name is /n
			// TODO: manage the case of the file with long name.
		} else if (name[0] != '/') { // Case of name/
			TRY(ar_parse_object_member(&member_header));
		}
	}
	
cleanup:
	if (g_fd != -1) {
		close(g_fd);
	}
	return result;
}