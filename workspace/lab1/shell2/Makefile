LIBS=-lpthread
LOUT=lib/shell2lib.a
CC=gcc
CFLAGS=-Wall
SDIR=src
SLDIR=$(SDIR)/lib
ODIR=obj
INC=-Iinc

_OBJS=linkedList.o
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

TEST_LIBS=-lcriterion
TEST_EXECUTABLE=bin/test

EXECUTABLE=bin/shell2

all: build $(EXECUTABLE)
	@$(EXECUTABLE)

test: build $(TEST_EXECUTABLE)
	@$(TEST_EXECUTABLE)

$(LOUT):  $(OBJS)
	@ar rs $(LOUT) $^ 2> /dev/null

$(ODIR)/%.o : $(SLDIR)/%.c 
	@$(CC) -c $(INC) -o $@ $< $(CFLAGS)

build: clean
	@mkdir -p lib bin obj
	
clean:
	@rm -rf lib bin obj

$(EXECUTABLE): $(LOUT)
	@$(CC) -c src/main.c -o obj/main.o
	@$(CC) obj/main.o $(LOUT) $(LIBS) -o $(EXECUTABLE) 

$(TEST_EXECUTABLE): $(LOUT)
	@$(CC) -c test/*.c -o obj/test.o 
	@$(CC) obj/test.o $(LOUT) $(TEST_LIBS) -o $(TEST_EXECUTABLE)
