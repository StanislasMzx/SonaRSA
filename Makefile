CC=clang
CFLAGS=-Wall -Wextra -pedantic -fdiagnostics-color=always -O0 -g3 -fsanitize=address # -Wno-error-implicit-function-declaration
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=serveur client

all: $(ALL_EXECUTABLES)

serveur.o: serveur.c
client.o: client.c

serveur: serveur.o
client: client.o


clean: 
	rm -f *.o ${ALL_EXECUTABLES}

.SILENT: clean
