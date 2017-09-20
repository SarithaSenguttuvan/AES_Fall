#Makefile for multitarget build system 

SRCS :=  		\
		main.c  \
		doublyLinkedList.c

CPPFLAGS= $(CTSFLAGS)
CFLAGS = -g -O0 -std=c99 
CC = gcc
BASENAME = main
TARGET = $(BASENAME).elf
MAPFILE = $(BASENAME).map
LDFLAGS = -Wl,-Map=$(MAPFILE)

INCLUDES = -I ../include/common


OBJS = $(SRCS:.c=.o)

%.o : %.c
	$(CC) $(INCLUDES) $(CPPFLAGS) -c $< $(CFLAGS) -o $@ 

%.asm : %.c
	$(CC) $(INCLUDES) $(CPPFLAGS) -S $< -o $@

%.i : %.c
	$(CC) $(INCLUDES) $(CPPFLAGS) -E $< -o $@	

%.dep : %.c
	$(CC) $(CPPFLAGS) $(INCLUDES) -M $< -o $@	


.PHONY: compile-all
compile-all: $(OBJS) $(ASM)

.PHONY: build
build: $(TARGET) $(ASM)

$(TARGET): $(OBJS) $(ASM)
	$(CC) $(OBJS) $(ASM) $(CFLAGS) $(LDFLAGS) -o $@ 

.PHONY: clean
clean:	
	rm -rf *.o *.i *.asm *.dep $(TARGET) $(MAPFILE)   




