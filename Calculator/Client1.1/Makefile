#############################################################################
# File name:  Makefile
# Author:     Ashley Ung 
# Date:       October 14, 2021
# Class:      CS360
# Assignment: MathPacket
# Purpose:    This is a Makefile for the MathPacket 
#############################################################################

# https://www.gnu.org/software/make/manual/html_node/Special-Targets.html
# https://www.gnu.org/software/make/manual/html_node/Chained-Rules.html#Chained-Rules
.SECONDARY:

all: bin bin/mathPacket_client bin/mathPacket_server bin/mathPacket_errors

bin:
	mkdir -p bin

# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# https://riptutorial.com/makefile/example/21469/automatic-variables

bin/mathPacket_client: bin/mathPacket_client.o
	gcc -o bin/mathPacket_client -g -Wall bin/mathPacket_client.o

bin/mathPacket_client.o: bin src/mathPacket_client.c
	gcc -c -o bin/mathPacket_client.o -g -Wall src/mathPacket_client.c

bin/mathPacket_server: bin/mathPacket_server.o
	gcc -o bin/mathPacket_server -g -Wall bin/mathPacket_server.o

bin/mathPacket_server.o: bin src/mathPacket_server.c
	gcc -c -o bin/mathPacket_server.o -g -Wall src/mathPacket_server.c

bin/mathPacket_errors: bin/mathPacket_errors.o
	gcc -o bin/mathPacket_errors -g -Wall bin/mathPacket_errors.o

bin/mathPacket_errors.o: bin src/mathPacket_errors.c
	gcc -c -o bin/mathPacket_errors.o -g -Wall src/mathPacket_errors.c

#gcc -o $@ -g -Wall $^

valgrind_client: bin/mathPacket_client
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/mathPacket_client 127.0.0.1 8080 1 + 2

valgrind_server: bin/mathPacket_server
	valgrind -v --leak-check=yes --track-origins=yes --leak-check=full --show-leak-kinds=all bin/mathPacket_server 8080

clean:
	rm -f bin/mathPacket_client bin/mathPacket_server bin/mathPacket_errors bin/*.o

printClient: bin
	enscript -C -T 2 -p - -M Letter -Ec --color -fCourier8 src/mathPacket_client.c  | ps2pdf - bin/mathPacket_client.pdf

printServer: bin
	enscript -C -T 2 -p - -M Letter -Ec --color -fCourier8 src/mathPacket_server.c  | ps2pdf - bin/mathPacket_server.pdf

printErrors: bin
	enscript -C -T 2 -p - -M Letter -Ec --color -fCourier8 src/mathPacket_errors.c  | ps2pdf - bin/mathPacket_errors.pdf
