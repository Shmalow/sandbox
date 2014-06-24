all:
	gcc -c -o julia.o julia.c
	gcc -c -o julia2.o julia2.c
	ar cru julia.a julia.o julia2.o
	gcc -c -o testjulia.o testjulia.c
	gcc -o testjulia.exe testjulia.o julia.a
	
clean:
	rm -f *.exe *.dll *.o *.a
	
	