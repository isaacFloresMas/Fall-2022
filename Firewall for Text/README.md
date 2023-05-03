# Assignment 6: The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists and Hash Tables

The programs included all work together to create a firewall for text. The file banhammer scans input from the user and parses each word. Each word is checked against a Bloom filter filled with hashed data from oldspeak and badspeak words. If the word is possibly in the Bloom filter, it is looked up in a hash table. If the word is in the hash table, it is added to a list of oldspeak or badspeak words. Once the user has finished their input text, they are given a counseling message depending on if they used oldspeak, badspeak, or both. If the user didn't use any oldspeak or badspeak word, no message is displayed.

## Files to have in directory when running the 'banhammer' program:

Ensure that the files **banhammer.c**, **messages.h**, **cityhash.c**, **cityhash.h**, **ht.c**, **ht.h**, **ll.c**, **ll.h**, **node.c**, **node.h**, **bf.c**, **bf.h**, **bv.c**, **bv.h**, **parser.c**, **parser.h** & **Makefile** are in your current directory before running banhammer. 

Makefile compiles and builds the entire program. Banhammer.c is the main file that uses all the other C files to create the firewall. Cityhash.c contains the hash function used to input oldspeak and badspeak keys in the Bloom filter and hash table. Ht.c contains the implementation for a hash table that pairs an oldspeak word to its newspeak translation, if any. Ll.c contains the implementation for a linked list which is used to hold the oldspeak words and their newspeak translations in the hash table. Linked lists avoid hash collision issues. Node.c contains the implementation for a node which is what a linked list is made up of. Bf.c contains the implementation for a Bloom filter which is used to check if an oldspeak or badspeak word is possibly contained in the hash table. Bv.c contains the implementation for a bit vector which is what a Bloom filter is comprised of. Parser.c parses every word from a given file. Each .h file allows functions from different files to be used in other files. The messages.h file contains the counseling messages displayed when the user uses oldspeak or badspeak.

## How to build banhammer:

##### Run the following to build the 'banhammer' program:

$ make

or

$ make all

or 

$ make banhammer

##### If you want to remove any files the compiler generated except the executable file run:

$ make clean

##### If you want to remove any files the compiler generated including the executable file run:

$ make spotless

## How to run banhammer:

There are many options for running **banhammer**. You can put no options or all of them. After building the files, you must run the file by writing the following in a terminal:

 $ ./banhammer -(*option*)
 
##### Options are:

• -t *size*: specifies that the hash table will have *size* entries (default *size* is 10000).

• -f *size*: specifies that the Bloom filter will have *size* entries (default *size* is 2^19 ).

• -m: enables the *move-to-front* rule (default is disabled).
      
• -s: will print out statistics from the program instead of the banhammer messages.

• -h: displays program synopsis and usage.

