OPT= -O 
#-m32

all: main32

main32: main32.o multiply.o myutil.o
	gcc-4.8 $(OPT) -o main32 main32.o multiply.o myutil.o -lgmp
# -lrt

main32.o: main32.c multiply.h
	gcc-4.8 $(OPT) -c main32.c

multiply.o: multiply.c multiply.h
	gcc-4.8 $(OPT) -c multiply.c

myutil.o: myutil.c
	gcc-4.8 $(OPT) -c myutil.c

clean: 
	make -i clear

clear:
	rm *~
	rm *.o main32

