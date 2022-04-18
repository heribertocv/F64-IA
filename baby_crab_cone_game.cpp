# include <stdio.h>
#include "baby_crab_cone_puzzle.hpp"

int main(void)
{
    int move;
    PuzzleGame Puzzle = create_game();

    while( 1 )
    {
        input_move( &move );
        update_status( &Puzzle, move );
        intro_puzzle(); 
        display_status( Puzzle );

        if( is_game_over(Puzzle) ) 
            break;
    }

    display_score( Puzzle );
    
}


