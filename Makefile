CC = gcc
CFLAGS = -Wall -Wextra -g

# Files
SOURCES = main.c txt_num.c test_txt_num.c
HEADERS = txt_num.h
OBJECTS = $(SOURCES:.c=.o)
MAIN_PROG = money_converter
TEST_PROG = test_money_converter

# Targets
.PHONY: all clean test

all: $(MAIN_PROG) $(TEST_PROG)

$(MAIN_PROG): main.o txt_num.o
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_PROG): test_txt_num.o txt_num.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

test: $(TEST_PROG)
	./$(TEST_PROG)

clean:
	rm -f $(OBJECTS) $(MAIN_PROG) $(TEST_PROG) money_cache.txt