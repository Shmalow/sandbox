all:
	gcc -c -o julia.o julia.c
	gcc -c -o julia2.o julia2.c
	gcc -shared -o julia.dll julia.o julia2.o -Wl,--out-implib=libjulia.dll.a
	mv julia.dll julia.dll.xxx
	gcc -c -o testjulia.o testjulia.c
	gcc -o testjulia.exe testjulia.o -L. -ljulia
	
clean:
	rm -f *.exe *.dll *.o
	
	