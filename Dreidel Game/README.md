# Assignment 3: I Have a Little Dreidel

The play-dreidel file plays a single game of dreidel. It takes the amount of players, starting number of coins 
for each player, the seed for the random number generator as options. It also allows the user to choose to
have a message display when a player is eliminated from the game. Running the play-dreidel file will play a 
single game of dreidel where the game finishes when 1 player remains. It then will display a message with the name 
of the winner, players in the game, number of starting coins, number of rounds it took the complete, and the seed.

## Files to have in directory when running 'play-dreidel' program:

Ensure that the files **Makefile**, **play-dreidel.c**, **dreidel.c**, **mtrand.c**, **dreidel.h**, and
**mtrand.h** are in your current directory before running play-dreidel. 

Makefile compiles and builds the entire program, play-dreidel.c takes the parameters for a game of 
dreidel and prints the winner, dreidel.c is where the actual game of dreidel is played, mtrand.c is a
random number generator, and dreidel.h/mtrand.h allow C functions in one file to be used on other files.

## How to build play-dreidel:

Run the following to build the 'play-dreidel' program:

$ make

## How to run play-dreidel:

There are many options for running **play-dreidel**. After building the files, you
must run the file by writing the following in a terminal:

 $ ./play-dreidel -(*option*). 
 
##### Options are:
 -p: Amount of players in the game. Must be greater than 1 and less than 9. If no option is given the default value
     is 4.
     
 -c: Starting coins for each player. Must be greater than 0 and less than 21. If no option is given the default value
     is 3.
     
 -s: The seed for the random number generator. Must be positive and no longer than 10 digits. If no option is
     given the default value is 613.
     
 -v: Displays when a player is eliminated and what round they lost on. If no option is given then the elimination   
     message won't appear.

## Other files in the repository:

Other files in the repository are WRITEUP.pdf and DESIGN.pdf. WRITEUP.pdf includes tests and discussion about 
the game of dreidel. This includes the average length of a game and what player position is most likely
to win. DESIGN.pdf contains a general explanation of the code that plays the game of dreidel. It also
has pseudocode for the dreidel.c file which plays the game of dreidel.
