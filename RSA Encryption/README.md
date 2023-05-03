# Assignment 5: Public Key Cryptography

The programs included all work together to produce random public and private key data into a file. This data is used for rsa encryption and rsa decryption. The public key data can be accessed by anyone but the private key data is kept to the user so that only they can decrypt the encrypted text. The public key data is used to encrypt text from a file or from user input. The encrypted data is placed in a file or printed as output. The private key data is used to decrypt the encrypted text from a file or user input. The decrypted text is written into a file or printed as output.

## Files to have in directory when running 'keygen, encrypt, & decrypt' program:

Ensure that the files **keygen.c**, **encrypt.c**, **decrypt.c**, **numtheory.c**, **numtheory.h**, **randstate.c**, **randstate.h**, **rsa.c**, **rsa.h**, & **Makefile** are in your current directory before running keygen, encrypt, & decrypt. 

Makefile compiles and builds the entire program. Keygen.c makes the public and private key files in a desired location. Encrypt.c encrypts a file and puts the encrypted output into a desired file. Decrypt.c decrypts the encrypted file and puts the text into a desired file. Numtheory.c includes functions that are needed to generate the public and private keys, encrypt data, and decrypt data. Randstate.c includes functions that set the seed and state for the random number generators. It also clears the state for the random number generators. Rsa.c includes functions that make, write, and read public and private key data, encrypts & decrypts files, and rsa signs and verifies signatures. Each .h file allows functions from different files to be used in other files.

## How to build keygen, encrypt, & decrypt:

##### Run the following to build the 'keygen, encrypt, & decrypt' programs altogether:

$ make

or

$ make all

##### Run the following to build the 'keygen, encrypt, or decrypt' programs specifically:

$ make keygen

or

$ make encrypt

or

$ make decrypt

##### If you want to remove any temporary files, .o files, and any executable files run:

$ make clean

##### If you want to remove the default public & private key files run:

$ make cleankeys

## How to run keygen:

There are many options for running **keygen**. You can put no options or all of them. After building the files, you must run the file by writing the following in a terminal:

 $ ./keygen -(*option*)
 
##### Options are:

• -b: specifies the minimum bits needed for the public modulus n (default: 1024).

• -i: specifies the number of Miller-Rabin iterations for testing primes (default: 50).

• -n pbfile: specifies the public key file (default: rsa.pub).

• -d pvfile: specifies the private key file (default: rsa.priv).

• -s: specifies the random seed for the random state initialization (default: the seconds since the
      UNIX epoch, given by time(NULL)).
      
• -v: enables verbose output.

• -h: displays program synopsis and usage.

## How to run encrypt:

There are many options for running **encrypt**. You can put no options or all of them. The location of the public key file should be the location used for keygen. After building the files, you must run the file by writing the following in a terminal:

 $ ./encrypt -(*option*)
 
##### Options are:

• -i: specifies the input file to encrypt (default: stdin).

• -o: specifies the output file to encrypt (default: stdout).

• -n: specifies the file containing the public key (default: rsa.pub).

• -v: enables verbose output.

• -h: displays program synopsis and usage.

## How to run decrypt:

There are many options for running **decrypt**. You can put no options or all of them. One thing to note here is that the output file for encrypt should be the input file to decrypt here. If the output was left as stdout, copy the encrypted data over and use it as stdin. Also, the location of the private key file should be the location used for keygen. After building the files, you must run the file by writing the following in a terminal:

 $ ./decrypt -(*option*)
 
##### Options are:

• -i: specifies the input file to decrypt (default: stdin).

• -o: specifies the output file to decrypt (default: stdout).

• -n: specifies the file containing the private key (default: rsa.priv).

• -v: enables verbose output.

• -h: displays program synopsis and usage.

