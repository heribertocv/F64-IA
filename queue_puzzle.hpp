/**
 * @file Queue_Puzzle.cpp
 * @author heribertocv at GitHub
 * @brief 
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#if !defined(QUEUE_PUZZLE)
#define QUEUE_PUZZLE

#include "node_puzzle.hpp"
#include "baby_crab_cone_puzzle.hpp"

const int MAX_SIZE_QUEUE = 1000;

typedef struct{
    Node_Puzzle container[MAX_SIZE_QUEUE];
    int back;
    int front;
}Queue;

Queue CreateQueue( void )
{
    Queue theQueue;
    theQueue.back = -1;
    theQueue.front = -1;
    return theQueue;
}

int size_queue( Queue theQueue )
{ return theQueue.front - theQueue.back; }

int enqueue( Queue *theQueue, Node_Puzzle theNode  )
{
    if( theQueue->front >= MAX_SIZE_QUEUE)
        return 0;

    if( theQueue->front == -1 )
        theQueue->back = theQueue->front = 0;

    copy_node( theNode, theQueue->container + theQueue->front );
    theQueue->front++;

    return 1;
}

int dequeue( Queue *theQueue, Node_Puzzle *theNode  )
{
    if( theQueue->back == -1 || theQueue->front < theQueue->back ){
        theQueue->front = theQueue->back = -1;
        return 0;
    }

    copy_node( *(theQueue->container + theQueue->back), theNode );
    theQueue->back++;

    return 1;
}

int back( Queue theQueue, Node_Puzzle *theNode )
{
    if( theQueue.back == -1 || theQueue.front < theQueue.back )
        return 0;

    copy_node( *(theQueue.container + theQueue.back), theNode );

    return 1;
}

int front( Queue theQueue, Node_Puzzle *theNode )
{
    if( theQueue.front == -1 || theQueue.front < theQueue.back )
        return 0;

    copy_node( *(theQueue.container + theQueue.front-1), theNode );

    return 1;
}


int in_container( Queue theQueue, int status )
{
    for(int ii=theQueue.back; ii< theQueue.front; ++ii)
        if( theQueue.container[ii].status == status )
            return 1;
    
    return 0;
}

void display_queue( Queue theQueue )
{
    printf(" ");
    for(int ii=theQueue.back; ii< theQueue.front; ++ii)
        printf("-------------|");
    printf("\n");
    for(int ii=theQueue.back; ii< theQueue.front; ++ii){
        printf("| %d ",theQueue.container[ii].level);
        display_status(theQueue.container[ii].status);
        // printf(" %d ", theQueue.container[ii].move);
    }
    printf("\n ");
    for(int ii=theQueue.back; ii< theQueue.front; ++ii)
        printf("-------------|");
    printf("\n\n");
}


#endif // QUEUE_PUZZLE
