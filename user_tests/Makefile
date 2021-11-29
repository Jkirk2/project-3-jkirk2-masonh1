EXEC := user_test

MAIN := user_test
UTILS := utils
FILE1 := buffer_user
FILE2 := buffer

SRCS := $(MAIN).c $(UTILS).c $(FILE1).c
DPNS := $(SRCS) $(MAIN).h $(UTILS).h $(FILE2).h 

CC := gcc
CFLAGS := -g --std=c17 -lpthread -lrt

all: $(EXEC)

$(EXEC): $(DPNS)
	$(CC) -o $(EXEC) $(SRCS) $(CFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm *.o
