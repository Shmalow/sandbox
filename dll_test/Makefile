all:
	gcc -c -o julia.o julia.c
	dlltool --export-all-symbols -l julia.lib -D julia.dll julia.o
	gcc -shared -o julia.dll julia.o -Wl,--out-implib=libjulia.dll.a
	gcc -c -o testjulia.o testjulia.c
	gcc -o testjulia.exe testjulia.o julia.lib
	
clean:
	rm -f *.exe *.dll *.o *.a *.lib *.def *.exp
	
	