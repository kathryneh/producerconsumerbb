CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=HW3.c buffer.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=katierocks

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
