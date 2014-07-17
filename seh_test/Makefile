all:
	gcc -Wall -Werror -c -I /usr/local/include/seh -o seh_test.o seh_test.c
	gcc -Wall -Werror -o seh.exe seh_test.o -lseh 
	
clean:
	rm -f *.exe *.o