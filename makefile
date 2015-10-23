CC = clang
CFLAGS = -Wall -Wextra -std=c99
LFLAGS = 
LIBS = -lm -pthreads
SOURCES = sudoku.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = sudoku
.PHONY: clean help

%.exe : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and .exe files"
