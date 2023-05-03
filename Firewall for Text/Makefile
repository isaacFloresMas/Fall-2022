CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic 

.PHONY: all clean spotless format

all: banhammer

banhammer: banhammer.o ht.o ll.o node.o bf.o bv.o parser.o city.o
	$(CC) -o $@ $^
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	
spotless:
	rm -f banhammer *.o

format:
	clang-format -i -style=file !(city).c *.[c] 
