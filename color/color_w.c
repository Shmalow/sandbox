#include <stdio.h>
#include <windows.h>

int main () {
	int result = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	printf("Hell");
	fflush(stdout);
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	printf("o Wo");
	fflush(stdout);
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	printf("rld!\n");
	fflush(stdout);
	
	// Restore original attributes 
	SetConsoleTextAttribute(hConsole, saved_attributes);
	printf("Back to normal");

	return result;
}
