all:
	gcc -std=gnu99 -c -o main.o main.c
	gcc -std=gnu99 -c -o ar_reader.o ar_reader.c
	ar cru julia.a main.o ar_reader.o
	gcc -std=gnu99 -o ar_reader.exe ar_reader.o main.o

clean:
	rm -f *.exe *.pt *.txt *.o *.a
	
install:
	mkdir -p /usr/local/bin
	cp -f ar_reader.exe /usr/local/bin
	
	