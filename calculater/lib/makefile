CC = gcc
CFLAGS = -o 
LFLAGS = -c -fpic
SOURCES = $(wildcard *.c)
#OBJECTS = $(patsubst %.c, %.o, $(SOURCES)) 
OBJECTS = $(SOURCES:.c=.o) 
TARGET = libcalc.so

all:$(TARGET)

$(TARGET):$(OBJECTS)
	$(CC) -shared $(CFLAGS) $@ $(OBJECTS)

%.o:%.c
	$(CC) $(LFLAGS) $< $(CFLAGS) $@	

clean:
	rm $(OBJECTS)

cleanA:
	rm $(OBJECTS) $(TARGET)