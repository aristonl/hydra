CC?=gcc
CFLAGS=-Wall
LFLAGS=
TARGET=libtar.a
AR=ar

all: $(TARGET) exec

tar.o: tar.h tar.cpp
	$(CC) $(CFLAGS) -c tar.cpp
