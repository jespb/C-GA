

## directorias
SDIR = src
IDIR = include
ODIR = obj
#CC = clang
FLAGS = -I$(IDIR) -Wall


all: objdir compile clean

objdir:
	mkdir -p $(ODIR)

compile: arguments.o individual.o geneticoperators.o GA.o main.o
	$(CC) $(ODIR)/GA.o $(ODIR)/geneticoperators.o $(ODIR)/individual.o $(ODIR)/main.o -o exec_GA  -lm

GA.o: $(SDIR)/GA.c $(IDIR)/GA.h $(IDIR)/geneticoperators.h $(IDIR)/individual.h
	$(CC) -c $(SDIR)/GA.c $(FLAGS) -o $(ODIR)/GA.o

geneticoperators.o: $(SDIR)/geneticoperators.c $(IDIR)/geneticoperators.h $(IDIR)/individual.h
	$(CC) -c $(SDIR)/geneticoperators.c $(FLAGS) -o $(ODIR)/geneticoperators.o

individual.o: $(SDIR)/individual.c $(IDIR)/individual.h
	$(CC) -c $(SDIR)/individual.c $(FLAGS) -o $(ODIR)/individual.o

arguments.o: $(SDIR)/arguments.c
	$(CC) -c $(SDIR)/arguments.c $(FLAGS) -o $(ODIR)/arguments.o

main.o: $(SDIR)/main.c $(IDIR)/GA.h
	$(CC) -c $(SDIR)/main.c $(FLAGS) -o $(ODIR)/main.o 

clean:
	rm -r obj/ 
	#rm exec_GA