CC=clang
CFLAGS=-Wall -Wextra -pedantic -fdiagnostics-color=always -O0 -g3 -fsanitize=address # -Wno-error-implicit-function-declaration
LDFLAGS+=-fsanitize=address

ALL_EXECUTABLES=src/agent/agent src/manager/client

all: $(ALL_EXECUTABLES)



src/scanner/ip_scan/ip_scan.o: src/scanner/ip_scan/ip_scan.c src/scanner/ip_scan/ip_scan.h src/scanner/ip_scan/ping.h libs/log/log.h
src/scanner/ip_scan/ip_scan: src/scanner/ip_scan/ip_scan.o src/scanner/ip_scan/ping.o libs/log/log.o

src/scanner/port_scan/port_scan.o: src/scanner/port_scan/port_scan.c src/scanner/port_scan/port_scan.h
src/scanner/port_scan/port_scan: src/scanner/port_scan/port_scan.o libs/log/log.o


src/scanner/ip_scan/ping.o: src/scanner/ip_scan/ping.c src/scanner/ip_scan/ping.h

src/manager/client.o: src/manager/client.c
src/manager/client: src/manager/client.o

src/agent/agent: src/agent/agent.o src/scanner/ip_scan/ip_scan.o src/scanner/ip_scan/ping.o src/scanner/port_scan/port_scan.o libs/log/log.o
src/agent/agent: src/agent/agent.o


libs/log/log.o: libs/log/log.c libs/log/log.h


clean: 
	rm -f *.o ${ALL_EXECUTABLES}

.SILENT: clean
