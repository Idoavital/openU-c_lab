

TARGET  = main
CC      = gcc
CFALG   = -g -Wall -ansi -pedantic -std=c90
LDFLAG  = 
INCLUDE = -I.

DEPS    = adjacency.h
OBJ     = main.o adjacency.o 


#################################################
# Ruls
#################################################
%.o: %.c $(DEPS)
	$(CC) $(CFALG) -c -o $@ $< $(INCLUDE)

$(TARGET): $(OBJ)
	$(CC) $(CFALG) $(INCLUDE) -o $@ $^

.PHONY: clean

clean:
	rm -f $(OBJ)
