# Name of the project
PROJ_NAME=mytop

# .c files
C_SOURCE=$(wildcard *.c)

# .h files
H_SOURCE=$(wildcard *.h)

# Object files
OBJ=$(C_SOURCE:.c=.o)

# Compiler
CC=gcc
BU = ./build

# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall     	\
		 -std=gnu99 \
		 -pedantic 


NCURSES = -lncurses
#
# Compilation and linking
#
all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^ -lncurses

main.o: main.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS) 

%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS) 

run: ./$(PROJ_NAME)
	./$(PROJ_NAME)

clean:
	rm -rf *.o $(PROJ_NAME) *~
