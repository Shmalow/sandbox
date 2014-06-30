#ifndef MY_WINNT_H
#define MY_WINNT_H // MY_WINNT_H
#define IMAGE_ARCHIVE_START_SIZE 8
#define IMAGE_ARCHIVE_START "!<arch>\n"

//#define BYTE unsigned char
typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;

typedef struct _IMAGE_ARCHIVE_MEMBER_HEADER {
	BYTE Name[16];
	BYTE Date[12];
	BYTE UserID[6];
	BYTE GroupID[6];
	BYTE Mode[8];
	BYTE Size[10];
	BYTE EndHeader[2];
} IMAGE_ARCHIVE_MEMBER_HEADER,*PIMAGE_ARCHIVE_MEMBER_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	WORD Machine;
	WORD NumberOfSections;
	DWORD TimeDateStamp;
	DWORD PointerToSymbolTable;
	DWORD NumberOfSymbols;
	WORD SizeOfOptionalHeader;
	WORD Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

#define IMAGE_SIZEOF_SHORT_NAME 8

typedef struct _IMAGE_SECTION_HEADER {
	BYTE Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		DWORD PhysicalAddress;
		DWORD VirtualSize;
	} Misc;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLinenumbers;
	WORD NumberOfRelocations;
	WORD NumberOfLinenumbers;
	DWORD Characteristics;
} IMAGE_SECTION_HEADER,*PIMAGE_SECTION_HEADER;

#define IMAGE_FILE_MACHINE_UNKNOWN		0x0000

typedef struct _IMPORT_HEADER {
	WORD Sig1;
	WORD Sig2;
	WORD Version;
	WORD Machine;
	DWORD CreatedTimeStamp;
	DWORD Size;
	WORD Hint;
	WORD TypeName;
} IMPORT_HEADER,*PIMPORT_HEADER;

// maps
#define SECTION_MACHINE					0
#define SECTION_CHARACTERISTICS			1

#define SECTION_IMPORT_HEADER			100
#define SECTION_IMPORT_NAME_HEADER		101

// Possible values for IMAGE_FILE_HEADER.Machine
#define IMAGE_FILE_MACHINE_UNKNOWN		0x0000
#define IMAGE_FILE_MACHINE_AM33			0x01d3 /* Matsushita AM33 */
#define IMAGE_FILE_MACHINE_AMD64		0x8664 /* x64 */
#define IMAGE_FILE_MACHINE_ARM			0x01c0 /* ARM little endian */
#define IMAGE_FILE_MACHINE_EBC			0x0ebc /* EFI byte code */
#define IMAGE_FILE_MACHINE_I386			0x014c /* Intel 386 or later processors and compatible processors */
#define IMAGE_FILE_MACHINE_IA64			0x0200 /* Intel Itanium processor family */
#define IMAGE_FILE_MACHINE_M32R			0x9041 /* Mitsubishi M32R little endian */
#define IMAGE_FILE_MACHINE_MIPS16		0x0266 /* MIPS16 */
#define IMAGE_FILE_MACHINE_MIPSFPU		0x0366 /* MIPS with FPU */
#define IMAGE_FILE_MACHINE_MIPSFPU16	0x0466 /* MIPS16 with FPU */
#define IMAGE_FILE_MACHINE_POWERPC		0x01f0 /* Power PC little endian */
#define IMAGE_FILE_MACHINE_POWERPCFP	0x01f1 /* Power PC with floating point support */
#define IMAGE_FILE_MACHINE_R4000		0x0166 /* MIPS little endian */
#define IMAGE_FILE_MACHINE_SH3			0x01a2 /* Hitachi SH3 */
#define IMAGE_FILE_MACHINE_SH3DSP		0x01a3 /* Hitachi SH3 DSP */
#define IMAGE_FILE_MACHINE_SH4			0x01a6 /* Hitachi SH4 */
#define IMAGE_FILE_MACHINE_SH5			0x01a8 /* Hitachi SH5 */
#define IMAGE_FILE_MACHINE_THUMB		0x01c2 /* Thumb */
#define IMAGE_FILE_MACHINE_WCEMIPSV2	0x0169 /* MIPS little-endian WCE v2 */

// Flags for imgage's characteristics
#define IMAGE_FILE_RELOCS_STRIPPED			1 << 0
#define IMAGE_FILE_EXECUTABLE_IMAGE			1 << 1
#define IMAGE_FILE_LINE_NUMS_STRIPPED		1 << 2
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED		1 << 3
#define IMAGE_FILE_AGGRESIVE_WS_TRIM 		1 << 4
#define IMAGE_FILE_LARGE_ADDRESS_AWARE		1 << 5
#define IMAGE_FILE_BYTES_REVERSED_LO		1 << 7
#define IMAGE_FILE_32BIT_MACHINE			1 << 8  
#define IMAGE_FILE_DEBUG_STRIPPED			1 << 9 
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	1 << 10
#define IMAGE_FILE_NET_RUN_FROM_SWAP		1 << 11
#define IMAGE_FILE_SYSTEM					1 << 12
#define IMAGE_FILE_DLL						1 << 13
#define IMAGE_FILE_UP_SYSTEM_ONLY			1 << 14
#define IMAGE_FILE_BYTES_REVERSED_HI		1 << 15

// Import type (in import header)
#define IMPORT_CODE				0
#define IMPORT_DATA				1
#define IMPORT_CONST			2

#define IMPORT_ORDINAL			0
#define IMPORT_NAME				1
#define IMPORT_NAME_NOPREFIX	2
#define IMPORT_NAME_UNDECORATE	3

#endif // MY_WINNT_H