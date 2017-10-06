CC=gcc
OBJ=thread.o

%.o: %.c
	$(CC) -pthread $^
    
all: $(OBJ)
	./a.out
    
clean: 
	rm a.out
