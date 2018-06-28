CC = gcc
CFLAGS = -g -o 
HEADER := $(wildcard *.h)
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET = calculater.exe
VPATH = lib
LIB = ./lib/libcalc.a

all:$(TARGET) 

$(TARGET):$(OBJECTS) 
	$(CC) $(OBJECTS) $(CFLAGS) $@ $(LIB)

%.o:%.c
	$(CC) -c $< $(CFLAGS) $@

$(OBJECTS):$(HEADER)

clean:
	rm $(OBJECTS)

cleanA:
	rm $(OBJECTS) $(TARGET)