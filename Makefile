CC      = gcc
CFLAGS  = -Wall -Werror
RM      = rm -f


default: all

all: exercice1 exercice2 exercice3 exercice4 exercice5

exercice1: exercice1.c
	$(CC) $(CFLAGS) -o exercice1 exercice1.c

exercice2: exercice2.c
	$(CC) $(CFLAGS) -o exercice2 exercice2.c

exercice3: exercice3.c
	$(CC) $(CFLAGS) -o exercice3 exercice3.c

exercice4: exercice4.c
	$(CC) $(CFLAGS) -o exercice4 exercice4.c
	
exercice5: exercice5.c
	$(CC) $(CFLAGS) -o exercice5 exercice5.c	

clean veryclean:
	$(RM) exercice1 exercice2 exercice3 exercice4 exercice5
