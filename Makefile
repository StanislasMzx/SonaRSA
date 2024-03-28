CC=clang
CFLAGS=-Wall -Wextra -pedantic -fdiagnostics-color=always -O0 -g3 -fsanitize=address # -Wno-error-implicit-function-declaration
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=serveur client

all: $(ALL_EXECUTABLES)

serveur.o: serveur.c
client.o: client.c

serveur: serveur.o
client: client.o

src/scanner/ip_scan/ip_scan.o: src/scanner/ip_scan/ip_scan.c src/scanner/ip_scan/ip_scan.h src/scanner/ip_scan/ping.h
src/scanner/ip_scan/ip_scan: src/scanner/ip_scan/ip_scan.o src/scanner/ip_scan/ping.o

src/scanner/ip_scan/ping.o: src/scanner/ip_scan/ping.c src/scanner/ip_scan/ping.h


clean: 
	rm -f *.o ${ALL_EXECUTABLES}

.SILENT: clean
