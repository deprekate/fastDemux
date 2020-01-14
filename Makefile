IDIR =./include
CC=gcc
CFLAGS=-g -Wall -O2 -Wno-unused-function -I$(IDIR)

SDIR=./src/

LIBS=-lm

all: fastDemux

fastderep: $(SDIR)fastDemux.c
	$(CC) $(CFLAGS) $^ -o $@ -lz -lm


