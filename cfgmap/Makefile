CFLAGS = -std=gnu99 -Wall -Werror

all:
	gcc $(CFLAGS) -c -o cfgmap.o cfgmap.c
	gcc -shared -o cfgmap.dll cfgmap.o -Wl,--out-implib=libcfgmap.dll.a
	gcc $(CFLAGS) -c -o test.o test.c
	gcc -o test.exe test.o cfgmap.dll
	
clean:
	rm -f *.exe *.dll *.o *.a
	
install: all
	mkdir -p /usr/local/bin
	cp -f cfgmap.dll /usr/local/bin
	mkdir -p /usr/local/lib
	cp -f libcfgmap.dll.a /usr/local/lib
	mkdir -p /usr/local/include
	cp -f cfgmap.h /usr/local/include
	