PWD := $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))
LIBS=-lpthread
LDIR=lib
LOUT=$(LDIR)/libshell2.a
CC=gcc
CFLAGS=-Wall
SDIR=src
SLDIR=$(SDIR)/lib
ODIR=obj
INC=-Iinc

_OBJS=linkedList

OBJS=$(patsubst %,$(ODIR)/%.o,$(_OBJS))
EXTRAS=$(patsubst %,$(SDIR)/lib/%.c,$(_OBJS))
SOBJS=$(patsubst %,$(LDIR)/lib%.so,$(_OBJS))

SHARED_LIBRARY=-L$(PWD)/$(LDIR) $(patsubst %,-l% ,$(_OBJS))

TEST_LIBS=-lcriterion
TEST_EXECUTABLE=bin/test

EXECUTABLE=bin/shell2

all: build $(EXECUTABLE)
	@$(EXECUTABLE)

test: build $(TEST_EXECUTABLE)
	@$(TEST_EXECUTABLE)

$(LDIR)/lib%.so: $(ODIR)/%.o
	$(CC) $< -shared -o $@ 

$(ODIR)/%.o: $(SLDIR)/%.c 
	@$(CC) -c -Wall -Werror -fpic $< -o $@ $(CFLAGS)

build: clean
	@mkdir -p lib bin obj
	
clean:
	@rm -rf lib bin obj

$(EXECUTABLE): $(SOBJS)
	@$(CC) -c src/main.c -o obj/main.o
	@$(CC) obj/main.o $(LIBS) $(SHARED_LIBRARY) -o $(EXECUTABLE) 

$(TEST_EXECUTABLE): $(SOBJS)
	@$(CC) test/*.c $(EXTRAS) $(TEST_LIBS) -o $(TEST_EXECUTABLE)
