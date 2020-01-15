IDIR =./include
CC=gcc
CFLAGS=-g -Wall -O2 -Wno-unused-function -I$(IDIR)

SDIR=./src/

LIBS=-lm

all: fastDemux

fastDemux: $(SDIR)fastdemux.c
	$(CC) $(CFLAGS) $^ -o $@ -lz -lm -g


