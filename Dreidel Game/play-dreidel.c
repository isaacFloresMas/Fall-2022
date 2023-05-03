# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "mtrand.h"
# include "dreidel.h"

char *names_list[] = {"Aharon", "Batsheva", "Chanah", "David", "Ephraim", "Faige", "Gamaliel", "Hannah"};

int main(int argc, char **argv)
{
    int opt = 0;
    int n_players = 4;
    int n_coins = 3;
    int n_rounds = 0;
    uint64_t seed = 613;
    while ((opt = getopt(argc, argv, "p:c:s:v")) != -1)
    {
	switch (opt) 
	{    
	case 'p':
	{
	    if (atoi(optarg) < 2 || atoi(optarg) > 8) {
		return 1;				 // non-zero error code
	    }
	    else {
	        n_players = atoi(optarg);
	        break;
	    }
	}
	case 'c':
	{
	    if (atoi(optarg) < 1 || atoi(optarg) > 20) {
                return 1;				 // non-zero error code
            }
	    else {
	        n_coins = atoi(optarg);
	        break;
	    }
	}
	case 's':
	{   
	    seed = strtoul(optarg, NULL, 0);    
	    if (seed <= 0 || seed >= 10000000000) {
		return 1;
	    }                                            
	    else { 
	        break;
	    }
	}
	case 'v':
	{
	    n_rounds = 1;
	    break;
	}
	default:
	{
	    return 1;
	}
        }
    }
    mtrand_seed(seed);      
	     
    printf("%s %d %d %d %lu\n",names_list[play_game(n_players, n_coins, &n_rounds)], n_players, n_coins, n_rounds, seed);
    return 0;
}
