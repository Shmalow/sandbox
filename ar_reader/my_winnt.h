#ifndef MY_WINNT_H
#define MY_WINNT_H // MY_WINNT_H
#define IMAGE_ARCHIVE_START_SIZE 8
#define IMAGE_ARCHIVE_START "!<arch>\n"

//#define BYTE unsigned char
typedef unsigned char BYTE, *PBYTE;
typedef unsigned short int WORD, *PWORD;
typedef short SHORT, *PSHORT;
typedef unsigned int DWORD, *PDWORD;

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

#pragma pack(push, 2)
typedef struct _IMAGE_SYMBOL {
	union {
		BYTE ShortName[8];
		struct {
			DWORD Short;
			DWORD Long;
		} Name;
		PBYTE LongName[2];
	} N;
	DWORD Value;
	SHORT SectionNumber;
	WORD Type;
	BYTE StorageClass;
	BYTE NumberOfAuxSymbols;
} IMAGE_SYMBOL, *PIMAGE_SYMBOL;
#pragma pack(pop)

// maps
#define SECTION_MACHINE					0
#define SECTION_CHARACTERISTICS			1

#define SECTION_IMPORT_HEADER			100
#define SECTION_IMPORT_NAME_HEADER		101

#define SECTION_SECTION_NUMBER_VALUE	200
#define SECTION_STORAGE_CLASS			201

#define SECTION_SECTION_CHARACTERISTICS 301

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


// Symbol table (SectionNumber)
#define IMAGE_SYM_UNDEFINED	0
#define IMAGE_SYM_ABSOLUTE (-1)
#define IMAGE_SYM_DEBUG	(-2)

// Symbol table (StorageClass)
#define IMAGE_SYM_CLASS_END_OF_FUNCTION		(-1)
#define IMAGE_SYM_CLASS_NULL				0
#define IMAGE_SYM_CLASS_AUTOMATIC			1
#define IMAGE_SYM_CLASS_EXTERNAL			2
#define IMAGE_SYM_CLASS_STATIC				3
#define IMAGE_SYM_CLASS_REGISTER			4
#define IMAGE_SYM_CLASS_EXTERNAL_DEF		5
#define IMAGE_SYM_CLASS_LABEL				6
#define IMAGE_SYM_CLASS_UNDEFINED_LABEL		7
#define IMAGE_SYM_CLASS_MEMBER_OF_STRUCT	8
#define IMAGE_SYM_CLASS_ARGUMENT			9
#define IMAGE_SYM_CLASS_STRUCT_TAG			10
#define IMAGE_SYM_CLASS_MEMBER_OF_UNION		11
#define IMAGE_SYM_CLASS_UNION_TAG			12
#define IMAGE_SYM_CLASS_TYPE_DEFINITION		13
#define IMAGE_SYM_CLASS_UNDEFINED_STATIC	14
#define IMAGE_SYM_CLASS_ENUM_TAG			15
#define IMAGE_SYM_CLASS_MEMBER_OF_ENUM		16
#define IMAGE_SYM_CLASS_REGISTER_PARAM		17
#define IMAGE_SYM_CLASS_BIT_FIELD			18
#define IMAGE_SYM_CLASS_FAR_EXTERNAL		68 /* Not in PECOFF v8 spec */
#define IMAGE_SYM_CLASS_BLOCK				100
#define IMAGE_SYM_CLASS_FUNCTION			101
#define IMAGE_SYM_CLASS_END_OF_STRUCT		102
#define IMAGE_SYM_CLASS_FILE				103
#define IMAGE_SYM_CLASS_SECTION				104
#define IMAGE_SYM_CLASS_WEAK_EXTERNAL		105
#define IMAGE_SYM_CLASS_CLR_TOKEN			107

// Flags for Section's characteristics
#define IMAGE_SCN_TYPE_REG					0
#define IMAGE_SCN_TYPE_DSECT				1
#define IMAGE_SCN_TYPE_NOLOAD				2
#define IMAGE_SCN_TYPE_GROUP				4
#define IMAGE_SCN_TYPE_NO_PAD				8
#define IMAGE_SCN_TYPE_COPY					16
#define IMAGE_SCN_CNT_CODE					32
#define IMAGE_SCN_CNT_INITIALIZED_DATA		64
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA	128
#define IMAGE_SCN_LNK_OTHER					256
#define IMAGE_SCN_LNK_INFO					512
#define IMAGE_SCN_TYPE_OVER					1024
#define IMAGE_SCN_LNK_REMOVE				2048
#define IMAGE_SCN_LNK_COMDAT				4096
#define IMAGE_SCN_GPREL						0x8000 /*  Valid only for IA64 */
#define IMAGE_SCN_MEM_FARDATA				0x8000 /* Not in PECOFF v8 spec */
#define IMAGE_SCN_MEM_PURGEABLE				0x20000
#define IMAGE_SCN_MEM_16BIT					0x20000
#define IMAGE_SCN_MEM_LOCKED				0x40000
#define IMAGE_SCN_MEM_PRELOAD				0x80000
#define IMAGE_SCN_ALIGN_1BYTES				0x100000
#define IMAGE_SCN_ALIGN_2BYTES				0x200000
#define IMAGE_SCN_ALIGN_4BYTES				0x300000
#define IMAGE_SCN_ALIGN_8BYTES				0x400000
#define IMAGE_SCN_ALIGN_16BYTES				0x500000
#define IMAGE_SCN_ALIGN_32BYTES				0x600000
#define IMAGE_SCN_ALIGN_64BYTES				0x700000
#define IMAGE_SCN_ALIGN_128BYTES			0x800000
#define IMAGE_SCN_ALIGN_256BYTES			0x900000
#define IMAGE_SCN_ALIGN_512BYTES			0xa00000
#define IMAGE_SCN_ALIGN_1024BYTES			0xb00000
#define IMAGE_SCN_ALIGN_2048BYTES			0xc00000
#define IMAGE_SCN_ALIGN_4096BYTES			0xd00000
#define IMAGE_SCN_ALIGN_8192BYTES			0xe00000
#define IMAGE_SCN_LNK_NRELOC_OVFL			0x1000000
#define IMAGE_SCN_MEM_DISCARDABLE			0x2000000
#define IMAGE_SCN_MEM_NOT_CACHED			0x4000000
#define IMAGE_SCN_MEM_NOT_PAGED				0x8000000
#define IMAGE_SCN_MEM_SHARED				0x10000000
#define IMAGE_SCN_MEM_EXECUTE				0x20000000
#define IMAGE_SCN_MEM_READ					0x40000000
#define IMAGE_SCN_MEM_WRITE					0x80000000

#endif // MY_WINNT_H
