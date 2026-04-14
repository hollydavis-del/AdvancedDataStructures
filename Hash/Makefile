INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := A3
DEPS 		 := ${INCLUDE_DIRS}/customErrorClass.h

.PHONY: clean all

all: A3

clean:
	rm $(OBJS) A3
A3: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)