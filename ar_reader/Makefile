CFLAGS = -Wall -Werror -std=gnu99 -I/usr/local/include

all:
	gcc $(CFLAGS) -c -o main.o main.c
	gcc $(CFLAGS) -c -o init_map.o init_map.c
	gcc $(CFLAGS) -c -o ar_reader.o ar_reader.c
	ar cru julia.a main.o ar_reader.o
	gcc -Wall -std=gnu99 -o ar_reader.exe ar_reader.o main.o init_map.o -lcfgmap -L/usr/local/lib

clean:
	rm -f *.exe *.pt *.txt *.o *.a *.i *.lib
	
install: all
	mkdir -p /usr/local/bin
	cp -f ar_reader.exe /usr/local/bin
	mkdir -p /usr/local/share/man/man1
	cp -f ./doc/ar_reader.1 /usr/local/share/man/man1
	
