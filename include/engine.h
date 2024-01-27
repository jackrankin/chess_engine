#pragma once
#include "board.h"
using namespace std;

class Heuristic {
public:
    static int material(Board *board); 

    static int checkmate(Board *board); 

    static int control(Board *board);
};

class Engine {
public:
    static Move find_move(Board *board); 

    static int minimax(Board *board, int alpha = -INT_MAX, int beta = INT_MAX, int depth = 0, int big_depth = 0); 

    static double evaluate_board(Board *board); 
};

