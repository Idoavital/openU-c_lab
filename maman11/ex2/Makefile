
TARGET  = ex2
CC      = gcc
CFALG   = -g -Wall -ansi -pedantic -std=c90
LDFLAG  = 
INCLUDE = -I.

DEPS    = match.h
OBJ     = ex2.o match.o 


#################################################
# Ruls
#################################################
%.o: %.c $(DEPS)
	$(CC) $(CFALG) -c -o $@ $< $(INCLUDE)

$(TARGET): $(OBJ)
	$(CC) $(CFALG) $(INCLUDE) -o $@ $^

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)
