all:
	gcc -c -o julia.o julia.c
	gcc -shared -o julia.dll julia.o
	gcc -c -o testjulia.o testjulia.c
	gcc -o testjulia.exe testjulia.o essai2.dll
	
clean:
	rm -f *.exe *.dll *.o
	
	