all:
	gcc -c -o julia_c.o julia.c
	dlltool --export-all-symbols -l julia_c.lib -D julia_c.dll julia_c.o
	gcc -shared -o julia_c.dll julia_c.o -Wl,--out-implib=libjulia_c.dll.a
	gcc -c -o testjulia_c.o testjulia.c
	gcc -o testjulia_c.exe testjulia_c.o julia_c.lib
	
	g++ -c -o julia.o julia.c
	dlltool --export-all-symbols -l julia.lib -D julia.dll julia.o
	g++ -shared -o julia.dll julia.o -Wl,--out-implib=libjulia.dll.a
	g++ -c -o testjulia.o testjulia.c
	g++ -o testjulia.exe testjulia.o julia.lib
	
	g++ -c -o testjulia2.o testjulia2.c
	g++ -o testjulia2.exe testjulia2.o julia_c.lib
	
clean:
	rm -f *.exe *.dll *.o *.a *.lib *.def *.exp
	
	