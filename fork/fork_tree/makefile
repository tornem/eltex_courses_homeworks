CC = gcc
CFLAGS = -g 
LFLAGS = -lncurses
HEADER := $(wildcard *.h)
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET = six_forks.exe
#VPATH = lib
#LIB = libcalc.a

all:$(TARGET) 
$(TARGET): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ 

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJECTS):$(HEADER)

clean:
	rm $(OBJECTS) $(TARGET)
