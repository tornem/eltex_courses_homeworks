CC = gcc
CFLAGS = -g 
LFLAGS = -lpthread
HEADER := $(wildcard *.h)
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET = storage_live.exe
#VPATH = lib
#LIB = libcalc.a

all:$(TARGET) 

$(TARGET): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJECTS):$(HEADER)

clean:
	rm $(OBJECTS) $(TARGET)
