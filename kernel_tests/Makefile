EXEC := test

MAIN := test

SRCS := $(MAIN).c
DPNS := $(SRCS)

CC := gcc
CFLAGS := -g --std=c17 -lpthread -lrt

all: $(EXEC)

$(EXEC): $(DPNS)
	$(CC) -o $(EXEC) $(SRCS) $(CFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm *.o
