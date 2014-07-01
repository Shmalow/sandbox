#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <cfgmap.h>

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
	if (errno) { \
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno)); \
		result = 1; \
		goto cleanup; \
	}
	
#define LSEEK(offset, mode) \
	lseek(g_fd, offset, mode); \
	if (errno) { \
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno)); \
		result = 1; \
		goto cleanup; \
	} 
	
#define STARTS_WITH(s1, s2) (strncmp(s1, s2, strlen(s2)) == 0)
	
char g_buffer[BUFFER_SIZE];
off_t g_current_header_offset = 8;
off_t last_header_offset = 0;
coff_t g_coff;

IMAGE_FILE_HEADER g_header; // coff header

char **g_section_table = NULL;

int g_fd = 0;

char *g_longnames_member_buffer = NULL;

int inverse_endian(int x) {
	int result = 0;
	result =  (x & 0xff000000) >> 24;
	result += (x & 0x00ff0000) >> 8;
	result += (x & 0x0000ff00) << 8;
	result += (x & 0x000000ff) << 24;
	return result;
}

int inverse_endian2(int x) {
	int result = 0;
	result =  (x & 0xff00) >> 8;
	result += (x & 0x00ff) << 8;
	return result;
}


char *trim(char *str) {
  char *end = NULL;

	// Trim leading space
	while (str[0] == ' ') {
		str++;
	}
	
	if(str[0] == 0) { // All spaces?
		return str;
	}
	
	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && end[0] == ' ') {
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

char *get_longname(int offset) {
	return g_longnames_member_buffer + offset;
}

char *get_archive_name(int offset) {
	int result = 0;
	char *archive_name = NULL;
	
	off_t current_offset = LSEEK(0, SEEK_CUR);
	LSEEK(offset, SEEK_SET);
	char name[16];
	READ(name, 16);
	
	archive_name = PARSE_FIELD(name);
	

	LSEEK(current_offset, SEEK_SET);
cleanup:
	if (result) {
		return "error";
	}
	return archive_name;
}


int ar_parse_header(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	
	printf("current_header_offset=0x%08X\n", (int) g_current_header_offset);
	LSEEK(g_current_header_offset, SEEK_SET);
	ssize_t s = READ(pmember_header, sizeof(IMAGE_ARCHIVE_MEMBER_HEADER));
	
	if (s < sizeof(IMAGE_ARCHIVE_MEMBER_HEADER)) {
		result = EOF;
		printf("End of file.\n");
		goto cleanup;
	}
	printf("---------Header\n");

	char *name = PARSE_FIELD(pmember_header->Name);
	if (name[0] == '/' && (strcmp(name, "/0") == 0 || atoi(name + 1) != 0)) {
		printf("Name: %s (%s)\n", PARSE_FIELD(pmember_header->Name), get_longname(atoi(name + 1)));
	} else {
		printf("Name: %s\n", PARSE_FIELD(pmember_header->Name));
	}
	
	time_t t = atoi(PARSE_FIELD(pmember_header->Date));
	struct tm *tm = localtime(&t);
	printf("Date: %04d/%02d/%02d - %02d:%02d:%02d (%u)\n",
		(1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (unsigned int) t);
	
	printf("UserID: %s\n", PARSE_FIELD(pmember_header->UserID));
	printf("GroupID: %s\n", PARSE_FIELD(pmember_header->GroupID));
	printf("Mode: %s\n", PARSE_FIELD(pmember_header->Mode));
	printf("Size: %s\n", PARSE_FIELD(pmember_header->Size));
	printf("EndHeader: %02X %02X\n", pmember_header->EndHeader[0],  pmember_header->EndHeader[1]);
	printf("current_body_offset=0x%08X\n", (int) g_current_header_offset + sizeof(IMAGE_ARCHIVE_MEMBER_HEADER));
	
	last_header_offset = g_current_header_offset;
	g_current_header_offset += sizeof(IMAGE_ARCHIVE_MEMBER_HEADER) + atoi(PARSE_FIELD(pmember_header->Size));
	if (g_current_header_offset % 2 == 1) {
		g_current_header_offset++;
	}
	
cleanup:
	return result;
}

int ar_parse_first_linker_member(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	unsigned int *offset = NULL;
	char *symbol_name = NULL;
	
	printf("----First Linker Member\n");
	unsigned int nbr_be;
	READ(&nbr_be, 4);
	
	unsigned int nbr = inverse_endian(nbr_be);
	printf("Symbol Nbr: %u\n", nbr);
	
	offset = (unsigned int *) malloc(sizeof(unsigned int) * nbr);
	READ(offset, nbr * 4);
	
	for (int i = 0; i < nbr; i++) {
		offset[i] = inverse_endian(offset[i]);
		printf("offset[%d]: 0x%08X (%s)\n", i, offset[i], get_archive_name(offset[i]));
	}
	
	int size = atoi(PARSE_FIELD(pmember_header->Size));
	size_t symbol_size = sizeof(char) * (size - 4 - (nbr * 4));
	symbol_name = (char *) malloc(symbol_size);
	READ(symbol_name, symbol_size);
	
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
	
	printf("----Second Linker Member\n");
	unsigned int member_nbr;
	READ(&member_nbr, 4);
	printf("Member Nbr:%d\n", member_nbr);

	size_t size = sizeof(unsigned int) * member_nbr;
	offset = (unsigned int *) malloc(size);
	READ(offset, size);
	for (int i = 0; i < member_nbr; i++) {
		printf("offset[%d]=0x%08X\n", i, offset[i]);
	}
	
	unsigned int symbol_nbr;
	READ(&symbol_nbr, 4);
	printf("Symbol Nbr:%d\n", symbol_nbr);
	
	size = sizeof(unsigned short int) * symbol_nbr;
	indice = (unsigned short int *) malloc(size);
	READ(indice, size);
	for (int i = 0; i < symbol_nbr; i++) {
		printf("indice[%d]=%d\n", i, indice[i]);
	}
	
	int total_size = atoi(PARSE_FIELD(pmember_header->Size));
	size = total_size - (4 + 4 * member_nbr + 4 + 2 * symbol_nbr);
	string = (char *) malloc(size);
	READ(string, size);
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

int ar_parse_longnames_member(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	
	printf("----Longnames Member\n");
	int size = atoi(PARSE_FIELD(pmember_header->Size));
	g_longnames_member_buffer = (char *) malloc(size);
	READ(g_longnames_member_buffer, size);
	char *cursor = g_longnames_member_buffer;
	int i = 0;
	while (cursor < g_longnames_member_buffer + size) {
		printf("archive[%d]=%s\n", i, cursor);
		i++;
		cursor += strlen(cursor) + 1;
	}
	
cleanup:
	return result;
}

int ar_parse_coff_header() {
	int result = 0;
	
	READ(&g_header, sizeof(IMAGE_FILE_HEADER));
	
	if (g_header.Machine == IMAGE_FILE_MACHINE_UNKNOWN && g_header.NumberOfSections == 0xFFFF) {
		// Import header
		PIMPORT_HEADER headerp = (PIMPORT_HEADER) &g_header;
		printf("---Import Header\n");
		printf("Sig1: 0x%04X\n", headerp->Sig1);
		printf("Sig2: 0x%04X\n", headerp->Sig2);
		printf("Version: 0x%04X\n", headerp->Version);
		printf("Machine: %s\n", map(SECTION_MACHINE, headerp->Machine));
		time_t t = headerp->CreatedTimeStamp;
		struct tm *tm = localtime(&t);
		printf("Creation date: %04d/%02d/%02d - %02d:%02d:%02d (%u)\n",
			(1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (unsigned int) t);
		printf("Size: %d bytes\n", headerp->Size);
		printf("Ordinal/Hint: %d\n", headerp->Hint);
		
		int type_name = inverse_endian2(headerp->TypeName);
		int type = type_name & 0xc000 >> 14;
		printf("Type: %s\n", map(SECTION_IMPORT_HEADER, type));
		int name = (type_name & 0x3800) >> 11;
		printf("name: %s\n", map(SECTION_IMPORT_NAME_HEADER, name));
		
	} else {
		// Normal coff header
		printf("---COFF Header\n");
		switch (g_header.Machine) {
			case 0x0:
				snprintf(g_buffer, BUFFER_SIZE, "%s", "Unknown machine");
				break;
			case 0x14c:
				snprintf(g_buffer, BUFFER_SIZE, "%s", "i386");
				break;
			default:
				snprintf(g_buffer, BUFFER_SIZE, "%04X", g_header.Machine);
		}
		printf("Machine type: %s\n", map(SECTION_MACHINE, g_header.Machine));
		printf("Number of sections: %d\n", g_header.NumberOfSections);
		
		time_t t = g_header.TimeDateStamp;
		struct tm *tm = localtime(&t);
		printf("Creation date: %04d/%02d/%02d - %02d:%02d:%02d (%u)\n",
			(1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (unsigned int) t);
		printf("Symbol table address: 0x%08x\n", g_header.PointerToSymbolTable);
		printf("Number of symbols: %d\n", g_header.NumberOfSymbols);
		printf("Size of optional header: %d\n", g_header.SizeOfOptionalHeader);
		printf("Characteristics:%s\n",
			list_flags(g_buffer, BUFFER_SIZE, SECTION_CHARACTERISTICS, g_header.Characteristics));
	}
cleanup:
	return result;
}

int ar_parse_coff_section_header(int i) {
	int result = 0;
	
	IMAGE_SECTION_HEADER section_header;
	READ(&section_header, sizeof(IMAGE_SECTION_HEADER));
	
	char name[IMAGE_SIZEOF_SHORT_NAME + 1];
	memset(name, 0, IMAGE_SIZEOF_SHORT_NAME + 1);
	memcpy(name, section_header.Name, IMAGE_SIZEOF_SHORT_NAME);
	printf("**Section name: %s\n", name);
	printf("  VirtualSize: %d bytes (0x%x)\n", section_header.Misc.PhysicalAddress, section_header.Misc.PhysicalAddress);
	printf("  VirtualAddress: 0x%08x\n", section_header.VirtualAddress);
	printf("  SizeOfRawData: %d bytes (0x%x)\n", section_header.SizeOfRawData, section_header.SizeOfRawData);
	printf("  PointerToRawData: 0x%08x\n", section_header.PointerToRawData);
	printf("  PointerToRelocations: 0x%08x\n", section_header.PointerToRelocations);
	printf("  PointerToLinenumbers: 0x%08x\n", section_header.PointerToLinenumbers);
	printf("  NumberOfRelocations: %d\n", section_header.NumberOfRelocations);
	printf("  NumberOfLinenumbers: %d\n", section_header.NumberOfLinenumbers);
	
	// Print the content of the section if the section starts with .idata$...
	if (STARTS_WITH(name, ".idata")) {
		printf("This is an import section.\n");
		if (section_header.SizeOfRawData == 0) {
			printf("Empty content.\n");
		} else if (section_header.SizeOfRawData == 4) {
			unsigned int content;
			off_t current_offset = LSEEK(0, SEEK_CUR);
			LSEEK(g_coff.offset + section_header.PointerToRawData, SEEK_SET);
			READ(&content, section_header.SizeOfRawData);
			LSEEK(current_offset, SEEK_SET);
			printf("Content as int: 0x%08X.\n", content);
		} else if (section_header.SizeOfRawData != 4) {
			off_t current_offset = LSEEK(0, SEEK_CUR);
			LSEEK(g_coff.offset + section_header.PointerToRawData, SEEK_SET);
			memset(g_buffer, 0, BUFFER_SIZE);
			READ(g_buffer, section_header.SizeOfRawData);
			LSEEK(current_offset, SEEK_SET);
			printf("Content as string: %s.\n", g_buffer);
			printf("Content as string+2: %s.\n", g_buffer + 2);
			printf("Content as hexa: \n");
			for (int i = 0; i < section_header.SizeOfRawData; i++) {
				printf("%02X ", g_buffer[i]);
			}
			printf("\n");
		}
	}
	
cleanup:
	return result;
}

int ar_parse_coff_section_table() {
	int result = 0;
	
	printf("---Section Table\n");
	//g_section_table = (char**) malloc(g_header.NumberOfSections * sizeof(char*));
	for (int i = 0; i < g_header.NumberOfSections; i++) {
		TRY(ar_parse_coff_section_header(i));
	}
cleanup:
	return result;
	
}

int ar_parse_object_import_name_str() {
	int result = 0;
	char *string = NULL;
	 
	off_t o = LSEEK(0, SEEK_CUR);
	printf("o=0x%08X\n", (unsigned int) o);
	PIMPORT_HEADER headerp = (PIMPORT_HEADER) &g_header;
	int size = sizeof(char) * headerp->Size;
	printf("size=%d\n", size);
	string = (char *) malloc(size);
	READ(string, size);
	char *cursor = string;
	int i = 0;
	while (cursor < string + size) {
		//printf("cursor=%08X\n", (unsigned int) cursor);
		printf("symbol[%d]=%s\n", i, cursor);
		i++;
		cursor += strlen(cursor) + 1;
	}
	
cleanup:
	FREE(string);
	return result;
}

int ar_parse_object_member(PIMAGE_ARCHIVE_MEMBER_HEADER pmember_header) {
	int result = 0;
	g_coff.offset = LSEEK(0, SEEK_CUR);
	
	printf("----Object Member\n");
	
	TRY(ar_parse_coff_header());
		
	if (g_header.Machine == IMAGE_FILE_MACHINE_UNKNOWN && g_header.NumberOfSections == 0xFFFF) {
		TRY(ar_parse_object_import_name_str());
	} else {
		TRY(ar_parse_coff_section_table());
	}
cleanup:
	return result;
}

int read_archive() {
	int result = 0;
	
	init_map();
	ar_signature signature;
	memset(&signature, 0, sizeof(ar_signature));

	g_fd = open(filename, O_RDONLY | O_BINARY);
	if (g_fd == -1) {
		fprintf(stderr, "There is an error. errno=%d (%s)\n", errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	READ(&signature, sizeof(ar_signature));
	
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
			TRY(ar_parse_longnames_member(&member_header));
		} else if ((name[0] == '/') && (atoi(name + 1) != 0)) { // Name is /n
			TRY(ar_parse_object_member(&member_header));
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