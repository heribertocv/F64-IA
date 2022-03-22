#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX_TURNS = 9;

const int SIZE_BOARD = 10; 

typedef struct{
    char board[SIZE_BOARD];
    char players[2][64];
    int player_turn;
    int num_turns;
    int have_winner;
}TicTacToe;

TicTacToe create_game()
{
    TicTacToe theGame;

    strcpy( theGame.board, "         ");
    strcpy( theGame.players[0], "Player X");
    strcpy( theGame.players[1], "Player O");
    theGame.player_turn = 0;
    theGame.num_turns = 0;
    theGame.have_winner = 0;

    return theGame;
}

void display_status(TicTacToe theGame)
{
    // WARNING: clear terminal is OS depend
    system("clear");  // case *NIX

    printf("\t------------------------\n");
    printf("\t       TIC TAC TOE\n");
    printf("\t------------------------\n");

    printf("\n");    
    for(int ii=0; ii<3;++ii){
        printf("\t\t %c | %c | %c \n", theGame.board[ii*3], theGame.board[ii*3+1], theGame.board[ii*3+2]);
        if( ii==2 )
            break;
        printf("\t\t-----------\n");
    }
    printf("\n");
}

void input_move( TicTacToe theGame, int *cell_to_mark )
{
    int r, c;
    printf("\n  (Type ROW and COLUMN coords in range [0,2])\n");
    printf("Which cell would you choose %s? : ", theGame.players[ theGame.player_turn ]);
    scanf( "%d %d", &r, &c );

    *cell_to_mark = 0<=r && r<=2 && 0<=c && c<=2 ? r*3 + c : -1;
    //printf("%d %d %d\n", r , c, *cell_to_mark);

    if( *cell_to_mark == -1 )
        printf("\tERROR: Unknow cell position\n");
}

int is_winner_move(TicTacToe *theGame)
{
    int base[]   = {0, 3, 6, 0, 1, 2, 0, 2 };
    int offset[] = {1, 1, 1, 3, 3, 3, 4, 2 };
    int *it_base = base;
    int *it_offset = offset;
    char token1, token2, token3;

    for( int ii=0; ii<8; ++ii, it_base++, it_offset++){
        token1 = theGame->board[ *it_base ];
        token2 = theGame->board[ *it_base + *it_offset];
        token3 = theGame->board[ *it_base + (*it_offset)*2];

        if( token1 != ' ' &&  token2 != ' ' && token3 != ' ' &&
            token1 == token2 && token2 == token3 ){
            return 1;
        }
    }

    return 0;
}

int is_empty_cell(TicTacToe *theGame, int cell_to_mark)
{
    return theGame->board[cell_to_mark] == ' ';
}

void update_status( TicTacToe *theGame, int cell_to_mark)
{
    char marks[] = "XO";

    if( cell_to_mark == -1 ||
        !is_empty_cell(theGame, cell_to_mark) ){
        return;
    }

    theGame->board[ cell_to_mark ] = marks[ theGame->player_turn ];
    theGame->num_turns++;
    
    if( is_winner_move( theGame) )  
        theGame->have_winner = 1;
    else 
        theGame->player_turn = theGame->num_turns % 2;
}

int is_game_over( TicTacToe theGame )
{
    return theGame.have_winner || theGame.num_turns >= 9; 
}

void display_score( TicTacToe theGame )
{
    printf("\t----------------------------\n");
    if( theGame.have_winner )
        printf("\t  %s is the WINNER!!!\n", theGame.players[ theGame.player_turn ] );
    else
        printf("TIED GAME\n");
    printf("\t\tTotal turns: %d\n", theGame.num_turns);
    printf("\t----------------------------\n");
}

int main (void)
{
    int cell_to_mark;

    TicTacToe game = create_game();
    display_status( game );

    while( 1 ){
        input_move( game, &cell_to_mark );
        update_status( &game, cell_to_mark);
        display_status( game );

        if( is_game_over( game ) ) 
            break;
    }

    display_score( game );

    return 0;
}