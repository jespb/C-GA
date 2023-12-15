

## directorias
SDIR = src
IDIR = include
ODIR = obj
FLAGS = -I$(IDIR) -Wall


all: objdir compile clean

objdir:
	mkdir -p $(ODIR)

compile: objects $(IDIR)/*.h
	$(CC) $(ODIR)/*.o -o exec_GA -lm $(FLAGS) $(SDIR)/main.c
	$(CC) $(ODIR)/*.o -o exec_GA_test -lm $(FLAGS) $(SDIR)/main_test.c

objects:$(SDIR)/RNG.c $(IDIR)/*.h
	$(CC) -c $(SDIR)/RNG.c $(FLAGS) -o $(ODIR)/RNG.o
	$(CC) -c $(SDIR)/GA.c $(FLAGS) -o $(ODIR)/GA.o
	$(CC) -c $(SDIR)/geneticoperators.c $(FLAGS) -o $(ODIR)/geneticoperators.o
	$(CC) -c $(SDIR)/individual.c $(FLAGS) -o $(ODIR)/individual.o

clean:
	rm -r obj/ 