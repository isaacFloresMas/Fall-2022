# Assignment 7: A (Huffman Coding) Tree Grows in Santa Cruz

This program compresses and decompresses a file using Huffman coding. The first program named encode compresses a file by dumping its Huffman tree and code into an output file. The second program named decode decompresses a file by rebuilding the dumped tree and following the code until the original file size is produced.

## Files to have in directory when running the 'encode' & 'decode' programs:

Ensure that the files **encode.c**, **decode.c**, **defines.h**, **header.h**, **node.c**, **node.h**, **pq.c**, **pq.h**, **code.c**, **code.h**, **io.c**, **io.h**, **stack.c**, **stack.h**, **huffman.c**, **huffman.h**, & **Makefile** are in your current directory before running encode and decode. 

Makefile compiles and builds the entire program. Encode.c encodes a file using Huffman coding and puts the compressed data into a specified output location. Decode.c decodes a compressed file and puts the decompressed data into a specified location. Node.c creates a node structure that is used to build a Huffman tree, a stack, and a priority queue. Pq.c creates a priority queue such that the highest priority node is the one with the smallest frequency. It is used when building a Huffman tree. Code.c is a stack of bits that is used to trace how to reach a symbol in a Huffman tree. Io.c handles input and output procedures used when encoding and decoding files. Stack.c is a stack of nodes that is used to rebuild a Huffman tree. Huffman.c contains functions that build a Huffman tree, create code tables, dump Huffman trees, rebuild Huffman trees, and delete Huffman trees. Each .h file allows functions from different files to be used in other files. Defines.h contains static data used in multiple files. Header.h contains data needed when compressing and decompressing data.

## How to build encode & decode:

##### Run the following to build the 'encode' & 'decode' programs:

$ make

or

$ make all

##### Run the following to build the 'encode' program by itself:

$ make encode

##### Run the following to build the 'decode' program by itself:

$ make decode

##### If you want to remove any files the compiler generated except the executable files run:

$ make clean

##### If you want to remove any files the compiler generated including the executable files run and temporary files:

$ make spotless

## How to run encode:

There are many options for running **encode**. You can put no options or all of them. After building the files, you must run the file by writing the following in a terminal:

 $ ./encode -(*option*)
 
##### Options are:

• -h: Prints out a help message describing the purpose of the program and the command-line options it accepts, exiting the program afterwards.

• -i infile: Specifies the input file to encode using Huffman coding. The default input is stdin.

• -o outfile: Specifies the output file to write the compressed input to. The default output is stdout.

• -v: Prints compression statistics. These statistics include the uncompressed file size, the compressed file size, and the space saving percentage. 

## How to run decode:

There are many options for running **decode**. You can put no options or all of them. After building the files, you must run the file by writing the following in a terminal:

 $ ./decode -(*option*)
 
##### Options are:

• -h: Prints out a help message describing the purpose of the program and the command-line options it accepts, exiting the program afterwards.

• -i infile: Specifies the input file to decode using Huffman coding. The default input is stdin.

• -o outfile: Specifies the output file to write the decompressed input to. The default output is stdout.

• -v: Prints compression statistics. These statistics include the compressed file size, the decompressed file size, and the space saving percentage.
