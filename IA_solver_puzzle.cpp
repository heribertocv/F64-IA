#if !defined(IA_SOLVER_PUZZLE)
#define IA_SOLVER_PUZZLE

#include "baby_crab_cone_puzzle.hpp"
#include "stack_puzzle.hpp"
#include "queue_puzzle.hpp"

#define FOUND_PATH 0xFF
#define CONTINUE_SEARCH 0xAA
#define DISCARD_TOP_NODE 0xBB


typedef struct{
    int moves[5];
    int num_moves;
    int level;
}Potential_Moves;

void display_potential_moves( Potential_Moves theMoves )
{
    printf("Potential Moves: \n");
    for(int ii=0; ii<theMoves.num_moves; ++ii )
        print_code_state( theMoves.moves[ii] );
}

int __side_mom( int game_status )
{
    return game_status & MAMA_LEFT ?  LEFT : RIGHT;
}

int __on_same_side_whitout_mom( int game_status, int actor1, int actor2  )
{
    int side_mama = __side_mom(game_status);
    int side_actor1 = game_status & (LEFT<<offset_by_actor(actor1)) ? LEFT : RIGHT;
    int side_actor2 = game_status & (LEFT<<offset_by_actor(actor2)) ? LEFT : RIGHT;
    
    return side_actor1 == side_actor2 && side_mama != side_actor1;
}

int __side_actor(int game_status, int actor)
{
    return game_status & (LEFT<<offset_by_actor(actor)) ? LEFT : RIGHT;
}

int __on_same_side_mom( int game_status, int actor)
{
    return __side_mom(game_status) == __side_actor(game_status, actor);
}

int __next_status( int game_status, int actor)
{
    int theNextStatus = game_status ^= MAMA; 

    if( actor != MAMA )
        theNextStatus ^= actor;

    if( __on_same_side_whitout_mom( theNextStatus, BABY, CONE) ){
        theNextStatus = SPOILS_DINNER_STATUS;
    }else if( __on_same_side_whitout_mom( theNextStatus, BABY, CRAB) )
        theNextStatus = BITES_BABY_STATUS;

    return theNextStatus;
}

Potential_Moves get_potential_moves( int game_status, int level )
{
    Potential_Moves the_potential_moves;

    int actor = MAMA;
    the_potential_moves.moves[0] = MAMA;
    the_potential_moves.num_moves = 1;
    the_potential_moves.level = level;

    // find actors in MOM´s side
    int offset=1;
    for(int ii=1; ii<4; ++ii ){
        actor <<= 2;
        if( __on_same_side_mom(game_status, actor) ){
            the_potential_moves.moves[offset++] = actor;
            the_potential_moves.num_moves++;       
        }
    }

    return the_potential_moves;
}

int try_add_moves_to_search( int game_status, Stack *theStack, Potential_Moves theMoves)
{
    int pushed_moves = 0;
    for( int ii=0; ii<theMoves.num_moves; ++ii)
    {
        int status = __next_status(game_status, theMoves.moves[ii] );

        if( status == BITES_BABY_STATUS || status == SPOILS_DINNER_STATUS ){
            continue;
        }
        
        Node_Puzzle node = { theMoves.moves[ii], status, theMoves.level };

        if( status == FINAL_STATE ){
            push(theStack, node );
            pushed_moves++;
            return FOUND_PATH;
        }else if( !in_container( *theStack, status ) ){
            push(theStack, node );
            pushed_moves++;
        }
    }

    //printf("pushed moves %d\n", pushed_moves);

    return pushed_moves ? CONTINUE_SEARCH : DISCARD_TOP_NODE;
}

int top_expand_path( Stack *theStack )
{
    int level;
    int game_status;
    Node_Puzzle top_stack;
    Potential_Moves moves;
    int status_search;

    // Pick the Node from the Stack for deep search 
    top( *theStack, &top_stack );
    game_status = top_stack.status;
    level = top_stack.level + 1;

    moves = get_potential_moves( game_status, level );
    status_search = try_add_moves_to_search( game_status, theStack, moves);

    return status_search;
}

int AI_solver_by_DFS(  int solution[], int max_iters )
{    
    int status_path;
    Node_Puzzle discard_node;

    Stack stack_search = CreateStack();
    Node_Puzzle root={.move=NO_MOVE, .status = INITIAL_STATE, .level=0};
    push( &stack_search, root );

    printf("\n\nlevel : 0\n");
    display_stack( stack_search );

    for(int iter=1; iter < max_iters; ++iter)
    {
        status_path = top_expand_path( &stack_search );

        if(  status_path == FOUND_PATH ){
            printf("\n\t********************************************\n");
            printf("\t\t IA FOUND_PATH!!!\n");
            printf("\t********************************************\n");
            break;
        }else if( status_path == DISCARD_TOP_NODE )
            pop(&stack_search, &discard_node);
        else
            CONTINUE_SEARCH;
        
        printf("\n\nlevel ??? : %d\n", iter);
        display_stack( stack_search );
    }

    display_stack( stack_search );

    // TODO: movement recovery from the stack and dump to solution[]

    return 0;
}

int expand_next_level_path( Queue *theQueue )
{
  // TODO: 
    return 0; 
}

int AI_solver_by_BFS( int solution[], int max_depth )
{
    int status_path;

    Queue queue_search = CreateQueue();
    Node_Puzzle root={.move=NO_MOVE, .status = INITIAL_STATE, .level=0};
    enqueue( &queue_search, root );

    printf("\n\nlevel : 0\n");
    display_queue( queue_search ); 

    for(int level=1; level < max_depth; ++level)
    {
        status_path = expand_next_level_path( &queue_search );
        
        if(  status_path == FOUND_PATH ){
            printf("\n\t********************************************\n");
            printf("\t\t IA FOUND_PATH!!!\n");
            printf("\t********************************************\n");
            break;
        }else
            CONTINUE_SEARCH;
    }

    return 0;
}


int main( void) 
{
    int moves_solution[16];
    int max_iterations = 100;
    printf("\n\n\t-------------------------------------\n");
    printf("\t\t IA Solver for Mama's Puzzle \n");
    printf("\t-------------------------------------\n\n");

    AI_solver_by_DFS(moves_solution, max_iterations);


    return 0;
}



#endif // MACRO
