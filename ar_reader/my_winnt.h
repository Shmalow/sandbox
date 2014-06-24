#ifndef MY_WINNT_H
#define MY_WINNT_H // MY_WINNT_H
#define IMAGE_ARCHIVE_START_SIZE 8
#define IMAGE_ARCHIVE_START "!<arch>\n"

//#define BYTE unsigned char
typedef unsigned char BYTE;

typedef struct _IMAGE_ARCHIVE_MEMBER_HEADER {
	BYTE Name[16];
	BYTE Date[12];
	BYTE UserID[6];
	BYTE GroupID[6];
	BYTE Mode[8];
	BYTE Size[10];
	BYTE EndHeader[2];
} IMAGE_ARCHIVE_MEMBER_HEADER,*PIMAGE_ARCHIVE_MEMBER_HEADER;

#endif // MY_WINNT_H