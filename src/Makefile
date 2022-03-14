CC = gcc
CFLAGS = -lm -lbsd

edit: main.o data.o calc.o battle.o
	$(CC) $(CFLAGS) -o edit main.o battle.o data.o calc.o 

main.o: main.c battle.h data.h
	$(CC) $(CFLAGS) -c -o main.o main.c
data.o: data.h battle.h
	$(CC) $(CFLAGS) -c -o data.o data.c 
battle.o: battle.h data.h calc.h
	$(CC) $(CFLAGS) -c -o battle.o battle.c
calc.o: 
	$(CC) $(CFLAGS) -c -o calc.o calc.c

cleanup:
	rm -rf *.o
	rm -rf edit
	
