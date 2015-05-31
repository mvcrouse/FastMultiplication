OPT= -O 

CC = gcc-4.8

all: interface

interface: interface.o multiply.o addition.o subtraction.o division.o arrayExpansion.o
	$(CC) $(OPT) -o result interface.o multiply.o addition.o subtraction.o division.o arrayExpansion.o

interface.o: interface.c multiply.h
	$(CC) $(OPT) -w -c interface.c

multiply.o: multiply.c multiply.h
	$(CC) $(OPT) -w -c multiply.c

addition.o: addition.c addition.h
	$(CC) $(OPT) -w -c addition.c

subtraction.o: subtraction.c subtraction.h
	$(CC) $(OPT) -w -c subtraction.c

division.o: division.c division.h
	$(CC) $(OPT) -w -c division.c

arrayExpansion.o: arrayExpansion.c arrayExpansion.h
	$(CC) $(OPT) -w -c arrayExpansion.c

clean: 
	make -i clear
	clear
clear:
	rm *~
	rm *.o main32
	rm result
