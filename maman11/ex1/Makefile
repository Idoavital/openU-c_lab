
# Set the default target (before any external file import)

TARGET  = ex1
CC      = gcc
CFALG   = -g -Wall -ansi -pedantic -std=c90
LDFLAG  = 
INCLUDE = -I.

DEPS    = sequence.h
OBJ     = ex1.o sequence.o 


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
