CC = gcc #компилятор
CFLAGS = -c -Wall #флаги для компилятора
SOURCES = playing_with_struct.c
OBJECTS = $(SOURCES:.c = .o)
EXECUTABLE = test.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm $(EXECUTABLE)
