CC      = gcc
CFLAGS  = -Wall -Werror
RM      = rm -f


default: all

all: exercice1 exercice2 exercice3

exercice1: exercice1.c
	$(CC) $(CFLAGS) -o exercice1 exercice1.c

exercice2: exercice2.c
	$(CC) $(CFLAGS) -o exercice2 exercice2.c

exercice3: exercice3.c
	$(CC) $(CFLAGS) -o exercice3 exercice3.c

clean veryclean:
	$(RM) exercice1 exercice2 exercice3
