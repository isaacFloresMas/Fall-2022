CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic 

.PHONY: all clean spotless format

all: encode decode

encode: encode.o code.o pq.o huffman.o io.o node.o stack.o
	$(CC) -o $@ $^
	
decode: decode.o code.o pq.o huffman.o io.o node.o stack.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	
spotless:
	rm -f encode decode in_temp.txt *.o

format:
	clang-format -i -style=file *.[c] 
