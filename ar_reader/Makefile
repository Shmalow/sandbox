CFLAGS = -Wall -Werror -std=gnu99

all:
	gcc $(CFLAGS) -c -o main.o main.c
	gcc $(CFLAGS) -c -o ar_reader.o ar_reader.c
	ar cru julia.a main.o ar_reader.o
	gcc -Wall -std=gnu99 -o ar_reader.exe ar_reader.o main.o

clean:
	rm -f *.exe *.pt *.txt *.o *.a *.i
	
install: all
	mkdir -p /usr/local/bin
	cp -f ar_reader.exe /usr/local/bin
	
