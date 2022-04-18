/**
 * @file Stack_Puzzle.cpp
 * @author heribertocv at GitHub
 * @brief 
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#if !defined(STACK_PUZZLE)
#define STACK_PUZZLE

#include "node_puzzle.hpp"
#include "baby_crab_cone_puzzle.hpp"

const int MAX_SIZE_STACK = 1000;

typedef struct{
    Node_Puzzle container[MAX_SIZE_STACK];
    int top;
}Stack;

Stack CreateStack( void )
{
    Stack theStack;
    theStack.top = 0;
    return theStack;
}

int size_stack( Stack theStack )
{   return theStack.top;    }

int push( Stack *theStack, Node_Puzzle theNode  )
{
    if( theStack->top >= MAX_SIZE_STACK )
        return 0;

    copy_node( theNode, theStack->container + theStack->top );
    theStack->top++;

    return 1;
}

int pop( Stack *theStack, Node_Puzzle *theNode  )
{
    if( theStack->top <= 0 )
        return 0;

    theStack->top--;
    copy_node( *(theStack->container + theStack->top), theNode );

    return 1;
}

int top( Stack theStack, Node_Puzzle *theNode )
{
    if( theStack.top <= 0 )
    return 0;

    copy_node( *(theStack.container + theStack.top-1), theNode );

    return 1;
}

int in_container( Stack theStack, int status )
{
    for(int ii=0; ii< theStack.top; ++ii)
        if( theStack.container[ii].status == status )
            return 1;
    
    return 0;
}


void display_stack( Stack theStack )
{
    printf(" ");
    for(int ii = 0; ii < theStack.top; ++ii)
        printf("-------------|");
    printf("\n");
    for(int ii = 0; ii < theStack.top; ++ii){
        printf("| %d ",theStack.container[ii].level);
        display_status(theStack.container[ii].status);
        // printf(" %d ", theStack.container[ii].move);
    }
    printf("\n ");
    for(int ii = 0; ii < theStack.top; ++ii)
        printf("-------------|");
    printf("\n\n");
}

#endif // STACK_PUZZLE
