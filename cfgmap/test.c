#include "cfgmap.h"

#define SECTION_A 0
#define CODE_A1 132
#define CODE_A2 133
#define CODE_A3 134
#define SECTION_B 1
#define CODE_B1 132
#define CODE_B2 133
#define SECTION_C 2
#define CODE_C1 132
#define CODE_C2 133
#define CODE_C3 134

extern int map_counter;

void init_map() {
	set_record(SECTION_A, CODE_A1, "Coucou");
	set_record(SECTION_A, CODE_A2, "Hello");
	set_record(SECTION_A, CODE_A3, "Salut");
	
	set_record(SECTION_B, CODE_B1, "Julia");
	set_record(SECTION_B, CODE_B2, "Yannis");
	
	set_record(SECTION_C, CODE_C1, "Jean-Louis");
	set_record(SECTION_C, CODE_C2, "Suzana");
}

int main() {
	init_map();
	printf("map_counter=%d\n", map_counter);
	printf("%s\n", map(SECTION_A, CODE_A1));
	printf("%s\n", map(SECTION_A, CODE_A2));
	printf("%s\n", map(SECTION_B, CODE_B1));
	char *answer = has_value(SECTION_C, CODE_C2) ? "Yes" : "No";
	printf("Does the SECTION_C has the string Yannis? %s\n", answer);

	return 0;
}