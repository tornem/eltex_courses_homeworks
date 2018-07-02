CC = gcc
CFLAGS = -Wall
SOURCES = $(wildcard *.c)
#OBJECTS = $(patsubst %.c, %.o, $(SOURCES)) 
OBJECTS = $(SOURCES:.c=.o) 
TARGET = libcalc.a

$(TARGET):$(OBJECTS)
	ar -rc $@ $?

%.o:%.c
	$(CC) -c $< -o $@	

clean:
	rm $(OBJECTS) $(TARGET)