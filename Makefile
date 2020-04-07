# Name of the project
PROJ_NAME = mytop

#Complier and linker
CC = gcc

#Flafs for compiler

CC_FLAGS = -lncurses


all: $(PROJ_NAME) image

$(PROJ_NAME):
	$(CC) -o main.o main.c $< $(CC_FLAGS)

image:
	./main.o

clean:
	rm -rf *.o *~ mytop