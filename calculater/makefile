CC = gcc
CFLAGS = -o 
HEADER = $(wildcard *.h)
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = calculate.exe
DIR := lib/libcalc.a

all:$(TARGET) $(HEADER)

$(TARGET):$(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $@ $(DIR)

%.o:%.c
	$(CC) -c $< $(CFLAGS) $@

clean:
	rm $(OBJECTS)

cleanA:
	rm $(OBJECTS) $(TARGET)