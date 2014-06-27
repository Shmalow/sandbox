#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct {
	int section;
	int code;
	char *label;
} record;

char s_buffer[BUFFER_SIZE];

record hashmap[1024];
int map_counter = 0;

size_t strlcat(char *dst, char *src, size_t size);

#ifdef __MINGW32__

size_t strlcat(char *dst, char *src, size_t size) {
	size_t l = strlen(dst);
	size_t e = strlen(src) + 1;
	if (e + l > size) {
		e = size - l;
	}
	memcpy(dst + l, src, e);
	dst[size - 1] = 0;
	return l + e;
}

#endif

void set_record(int section, int code, char *label) {
	hashmap[map_counter].section = section;
	hashmap[map_counter].code = code;
	hashmap[map_counter].label = label;
	map_counter++;
}

char* map(int section, int code) {
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section && hashmap[i].code == code) {
			return hashmap[i].label;
		}
	}
	snprintf(s_buffer, BUFFER_SIZE, "Label to be defined: %d", code);
	return s_buffer;
}

char* get_flags(char *buffer, size_t size, int section, int flags) {
	strncpy(buffer, "", size);
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section) {
			if (hashmap[i].code & flags) {
				char buf[1024];
				snprintf(buf, 1024, " <%s>", hashmap[i].label);
				strcat(buffer, buf);
			}
		}
	}
	if (strcmp(buffer, "") == 0) {
		snprintf(buffer, size, "No flag");
	}
	return buffer;
}

char* list_flags(char *buffer, size_t size, int section, int flags) {
	snprintf(buffer, size, "\n");
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section) {
			char buf[1024];
			char *value = "Off";
			if (hashmap[i].code & flags) {
				value = "On";
			}
			snprintf(buf, 1024, "\t%s: %s\n", hashmap[i].label, value);
			strlcat(buffer, buf, BUFFER_SIZE);
		}
	}
	return buffer;
}

int has_flags(int flag, int flags) {
	return flag & flags;
}