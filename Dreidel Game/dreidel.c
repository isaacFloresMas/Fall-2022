# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "dreidel.h"

int check_winner(int player_coins[], int players);
char *names[8] = {"Aharon", "Batsheva", "Chanah", "David", "Ephraim", "Faige", "Gamaliel", "Hannah"};

char spin_dreidel(void)
{   
    int rand = mtrand_rand64() % 4; 
    if (rand == 0)
        return 'G';
    else if (rand == 1)
	return 'H';
    else if (rand == 2)
	return 'N';
    else
	return 'S';
}

int play_game (int n_players, int coins_per_player, int * n_rounds)
    { 
    int players = n_players;
    int coins = coins_per_player;
    int pot, winner, turn;
    char dreidel_letter;
    int player_coins[players];
    int v_case = (*n_rounds == 1) ? 1 : 0;
    pot = winner = turn = *n_rounds = 0;

    for (int i = 0; i < players; i += 1)
    {
	player_coins[i] = coins;
    }

    for (turn = 0; check_winner(player_coins, players); turn += 1)
    {
	if (player_coins[(turn % players)] >= 0)
	    {	    
	    dreidel_letter = spin_dreidel();
	    if (dreidel_letter == 'N')
            { 	
		continue;		
	    }
    	    else if (dreidel_letter == 'G')
	    {    
        	player_coins[turn % players] += pot;  
	        pot = 0;
	    }
    	    else if (dreidel_letter == 'H')
	    {   

                player_coins[turn % players] += (pot / 2);
                pot -= (pot / 2);	
	    }
    	    else if (dreidel_letter == 'S')
            {
		if (player_coins[turn % players] == 0 && v_case) {
		    player_coins[turn % players] = -1;
		    *n_rounds = (turn % players != 0) ? (turn / players) + 1: turn / players;
		    printf("%s: eliminated in round %d of a %d player game\n", names[turn % players], *n_rounds, players); 
		}
		else if (player_coins[turn % players] == 0 && !v_case) {
		    player_coins[turn % players] = -1;
		}
		else {
                player_coins[turn % players] -= 1;
                pot += 1;
		}	
            }
            }	
    }

    for (int j = 0; j < players; j += 1)
    {
	if (player_coins[j] > 0)
	    winner = j;
    }
    
    *n_rounds = (turn % players != 0) ? (turn / players) + 1: turn / players;
    
    return winner;
    }

int check_winner(int player_coins[], int players)
{
    int still_in = 0;
    for (int i = 0; i < players; i += 1)
    {
	if (player_coins[i] != -1) {
	    still_in += 1;
	}
    }

    if (still_in == 1) {
        return 0;
    }
    else {
	return 1;
    }
}
