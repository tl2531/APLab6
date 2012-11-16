#
# Makefile for lab 6, part 2
#

CC  = gcc
CXX = g++

INCLUDES = 
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = 
LDLIBS =

.PHONY: default
default: http-client

.PHONY: clean
clean:
	rm -f *.o *~ a.out core http-client *.html

.PHONY: all
all: clean default