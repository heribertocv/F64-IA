/**
 * @file Node_Puzzle.cpp
 * @author heribertocv at GitHub
 * @brief 
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#if !defined(NODE_PUZZLE)
#define NODE_PUZZLE

#include <stdio.h>

#include "baby_crab_cone_puzzle.hpp"

typedef struct {
    int move;
    int status;
    int level;
}Node_Puzzle;

inline void copy_node(Node_Puzzle src, Node_Puzzle  *dst )
{
    dst->move = src.move;
    dst->status = src.status;
    dst->level = src.level;
}

void display_node( Node_Puzzle node )
{
    printf(" move: %d  ",node.move );
    display_status(node.status);
    printf("  level %d \n", node.level );
}

#endif // NODE_PUZZLE