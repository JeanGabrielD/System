CC      = gcc
CFLAGS  = -Wall -Werror
RM      = rm -f


default: all

all: exercice1

exercice1: exercice1.c
	$(CC) $(CFLAGS) -o exercice1 exercice1.c

clean veryclean:
	$(RM) exercice1
