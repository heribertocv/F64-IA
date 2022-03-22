#include <stdio.h>
#include <stdlib.h>

#define NO_MOVE 0x00

#define MAMA  0x03   // 0b00000011
#define BABY  0x0C   // 0b00001100
#define CRAB  0x30   // 0b00110000
#define CONE  0xC0   // 0b11000000

#define LEFT  0x01   // 0b00000001 
#define RIGHT 0x02   // 0b00000010 

const int MAMA_LEFT  = LEFT;
const int MAMA_RIGHT = RIGHT;
const int BABY_LEFT  = LEFT  << 2; 
const int BABY_RIGHT = RIGHT << 2;
const int CRAB_LEFT  = LEFT  << 4;
const int CRAB_RIGHT = RIGHT << 4;
const int CONE_LEFT  = LEFT  << 6;
const int CONE_RIGHT = RIGHT << 6;

const int INITIAL_STATE = BABY_LEFT |
                        CRAB_LEFT |
                        CONE_LEFT |
                        MAMA_LEFT;

const int FINAL_STATE = BABY_RIGHT |
                        CRAB_RIGHT |
                        CONE_RIGHT |
                        MAMA_RIGHT;

const int END_GAME = FINAL_STATE;
const int OK_STATUS = 0;
const int UNKNOW_STATUS = -1; 
const int SPOILS_DINNER_STATUS = -2;
const int BITES_BABY_STATUS = -3;

const int MAX_TRIES = 10;

typedef struct{
    int num_tries;
    int current_state;
    int code_status;
    int moves[MAX_TRIES+1];
}PuzzleGame;

void print_code_state(int n)
{
    printf("current status: ");
    for(int i=0; i<8; ++i)
        printf("%c", n&(1<<i) ? '1' : '0');
    printf("\n");
}

void intro_puzzle(void)
{
    // WARNING: clear terminal is OS depend
    system("clear");  // case *NIX

    printf("\n\n\
\tWelcome to the Puzzle of the Baby, the Crab,and the Cone. \n\
\n\
  One hot summer day, Mama crossed the river to the left \n\
bank take a break from programming and buy an ice cream.\n\
She brought her baby and her pet crab. On the return \n\
she came to the river and was faced with a dilemma: how \n\
could they all cross without disaster? The ferry holds \n\
Mama and just one other item. \n\
\nThe problem is that: \n\
\t* if the baby is left with the cone,it spoils its dinner.\n\
\t* if the baby is left with the crab, the crab bites the baby.\n\
\nPlease help Mama make her journey across the river! \n\n");
}

void input_move( int *move )
{
    printf("\n\t(Type 0 for no one, 1 for baby, 2 for crab, or 3 for cone)\n");
    printf("Who would you like Mama to cross with? : ");
    scanf( "%d", move );
    printf("\n");

    switch( *move ){
        case 1 :
            *move = BABY;
            break;
        case 2 :
            *move = CRAB;
            break;
        case 3: 
            *move = CONE;
            break;
        default:
            *move = MAMA;
    } 

}

void move_error()
{
    printf("Play move unknow!!!\n try again...\n");
}

int side_mom( PuzzleGame theGame )
{
    return theGame.current_state & MAMA_LEFT ?  LEFT : RIGHT;
}

int offset_by_actor(int actor)
{
    int offset = 0;
    switch (actor){
        case BABY:
            offset = 2;
            break;
        case CRAB:
            offset = 4;
            break;
        case CONE:
            offset = 6;
            break;
        default:
            offset = 0;
            break;
    }

    return offset;
}

int side_actor( PuzzleGame theGame, int actor)
{
    return theGame.current_state & (LEFT<<offset_by_actor(actor)) ? LEFT : RIGHT;
}

int on_same_side_whitout_mom( PuzzleGame theGame, int actor1, int actor2)
{
    int side_mama = side_mom(theGame);
    int side_actor1 = theGame.current_state & (LEFT<<offset_by_actor(actor1)) ? LEFT : RIGHT;
    int side_actor2 = theGame.current_state & (LEFT<<offset_by_actor(actor2)) ? LEFT : RIGHT;
    
    return side_actor1 == side_actor2 && side_mama != side_actor1;
}

int on_same_side_mom( PuzzleGame theGame, int actor)
{
    return side_mom(theGame) == side_actor(theGame, actor);
}

int update_status( PuzzleGame *theGame, int actor )
{
    if( !on_same_side_mom(*theGame, actor) ){
        printf("*** Play move is not possible...\n\tMama is not at same bank\n\n" );
        return 0;
    }

    // move MAMA and actor move to the other bank
    theGame->current_state ^= MAMA;

    if( actor != MAMA )
        theGame->current_state ^= actor;
    
    theGame->moves[ theGame->num_tries ] = theGame->current_state;
    theGame->num_tries++;
    
    if( theGame->current_state == FINAL_STATE ||
        theGame->num_tries >= MAX_TRIES )
        theGame->code_status = END_GAME;
    else if( on_same_side_whitout_mom( *theGame, BABY, CONE) ){
        theGame->code_status = SPOILS_DINNER_STATUS;
    }else if( on_same_side_whitout_mom( *theGame, BABY, CRAB) )
        theGame->code_status = BITES_BABY_STATUS;
    else 
        theGame->code_status = OK_STATUS;

    return 1;
}

int is_game_over( PuzzleGame theGame )
{
    return theGame.code_status != OK_STATUS;
} 

void display_status(PuzzleGame theGame )
{
    char buffer[64];
    char pattern_l[] = "\t    %s  ||";
    char pattern_r[] = "\t\t  ||    %s\t";

    printf("\t------------------------ \n");
    printf("\tLEFT BANK || RIGHT BANK \n");
    printf("\t------------------------ \n");
    sprintf(buffer,side_actor(theGame, MAMA) & LEFT ? pattern_l : pattern_r  ,"MAMA");
    printf("%s\n",buffer);
    sprintf(buffer,side_actor(theGame, BABY) & LEFT ? pattern_l : pattern_r  ,"BABY");
    printf("%s\n",buffer);
    sprintf(buffer,side_actor(theGame, CRAB) & LEFT ? pattern_l : pattern_r  ,"CRAB");
    printf("%s\n",buffer);
    sprintf(buffer,side_actor(theGame, CONE) & LEFT ? pattern_l : pattern_r  ,"CONE");
    printf("%s\n",buffer);
    printf("\t------------------------ \n");

}

PuzzleGame create_game()
{
    PuzzleGame theGame;

    theGame.current_state = INITIAL_STATE;
    theGame.num_tries = 0;
    theGame.code_status = OK_STATUS; 

    intro_puzzle(); 
    display_status(theGame);

    return theGame;
}

void display_score( PuzzleGame theGame )
{
    printf("\n\n ------------------------------------------------------\n");
    if( theGame.current_state == FINAL_STATE ){
        printf("\t C O N G R A T U L A T I O N S\n");
        printf("\t\t YOU WIN!!!\n");
        printf("\t\t score %d/%d \n\t\t \n", theGame.num_tries, MAX_TRIES);
    }else{
        printf("  We'll have to stop and help Mama and her family.\n\
    Undoubtedly, she'll need your help again\n");
        printf("\t\t YOU LOST!!!\n"); 
    }
    printf(" ------------------------------------------------------\n\n\n");
}

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