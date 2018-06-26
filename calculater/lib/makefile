CC = gcc
CFLAGS = -o
SOURCES = $(wildcard *.c)
#OBJECTS = $(patsubst %.c, %.o, $(SOURCES)) 
OBJECTS = $(SOURCES:.c=.o) 
TARGET = libcalc.a

all:$(TARGET)

$(TARGET):$(OBJECTS)
	ar -rc $@ $(OBJECTS)

%.o:%.c
	$(CC) -c $< $(CFLAGS) $@	

clean:
	rm $(OBJECTS)

cleanA:
	rm $(OBJECTS) $(TARGET)