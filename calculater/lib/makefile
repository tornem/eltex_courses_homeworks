CC = gcc
CFLAGS = -o
SOURCES = $(wildcard *.c)
#OBJECTS = $(patsubst %.c, %.o, $(SOURCES)) 
OBJECTS = $(SOURCES:.c=.o) 
TARGET = libcalc.a

$(TARGET):$(OBJECTS)
	ar -rc $@ $?

%.o:%.c
	$(CC) -c $< $(CFLAGS) $@	

clean:
	rm $(OBJECTS)

cleanA:
	rm $(OBJECTS) $(TARGET)