#ifndef AR_READER_H
#define AR_READER_H

#include "my_winnt.h"

#define BUFFER_SIZE 1024


int read_archive();

extern char *filename;

typedef struct {
	char buffer[IMAGE_ARCHIVE_START_SIZE];
} ar_signature;



#endif // AR_READER_H
