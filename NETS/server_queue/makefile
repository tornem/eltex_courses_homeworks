CC = gcc
CFLAGS = -g 
LFLAGS = -lpthread
HEADER := $(wildcard *.h)
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET = server.exe
#VPATH = lib
#LIB = libcalc.a

all:$(TARGET) 
#определяю libcalc.a как первый пререквезит чтоб его можно было хватануть "$<"
#иначе VPATH вообще не ищет. Тоже не понимаю, почему.
$(TARGET): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJECTS):$(HEADER)

clean:
	rm $(OBJECTS) $(TARGET)
