OPT= -O 
#-m32

CC = gcc-4.8

all: interface #main32

interface: interface.o multiply.o myutil.o addition.o subtraction.o division.o arrayExpansion.o
	$(CC) $(OPT) -o interface interface.o multiply.o myutil.o addition.o subtraction.o division.o arrayExpansion.o -lgmp

main32: main32.o multiply.o myutil.o addition.o subtraction.o division.o arrayExpansion.o
	$(CC) $(OPT) -o main32 main32.o multiply.o myutil.o addition.o subtraction.o division.o arrayExpansion.o -lgmp

main32.o: main32.c multiply.h
	$(CC) $(OPT) -c main32.c

interface.o: interface.c multiply.h
	$(CC) $(OPT) -c interface.c

multiply.o: multiply.c multiply.h
	$(CC) $(OPT) -c multiply.c

myutil.o: myutil.c
	$(CC) $(OPT) -c myutil.c

addition.o: addition.c addition.h
	$(CC) $(OPT) -c addition.c

subtraction.o: subtraction.c subtraction.h
	$(CC) $(OPT) -c subtraction.c

division.o: division.c division.h
	$(CC) $(OPT) -c division.c

arrayExpansion.o: arrayExpansion.c arrayExpansion.h
	$(CC) $(OPT) -c arrayExpansion.c

clean: 
	make -i clear
	clear
clear:
	rm *~
	rm *.o main32

