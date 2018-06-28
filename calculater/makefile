CC = gcc
CFLAGS = -g -o 
HEADER := $(wildcard *.h)
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET = calculater.exe
VPATH = lib
LIB = libcalc.a

all:$(TARGET) 
#определяю libcalc.a как первый пререквезит чтоб его можно было хватануть "$<"
#иначе VPATH вообще не ищет. Тоже не понимаю, почему.
$(TARGET):$(LIB) $(OBJECTS) 
	$(CC) $(OBJECTS) $(CFLAGS) $@ $<

$(LIB):
	$(MAKE) -C $(VPATH)

%.o:%.c
	$(CC) -c $< $(CFLAGS) $@

$(OBJECTS):$(HEADER)

clean:
	rm $(OBJECTS) $(TARGET)
