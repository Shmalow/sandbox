#include <stdio.h>

void set_record(int section, int code, char *label);

char* map(int section, int code);

char* get_flags(char *buffer, size_t size, int section, int flags);

char* list_flags(char *buffer, size_t size, int section, int flags);

int has_flags(int flag, int flags);

