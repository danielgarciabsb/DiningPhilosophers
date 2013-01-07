CC=gcc
CFLAGS= -Wall -O2 -lm -ansi -std=c99 -D_GNU_SOURCE -pedantic -g
ADDITIONAL= -lpthread

all: Main.c
  $(CC) $(CFLAGS) Main.c -o "O Incrivel Jantar dos Filosofos" $(ADDITIONAL)
clean:
  rm -f "O Incrivel Jantar dos Filosofos"
