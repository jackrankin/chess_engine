#pragma once
#include "bit_operator.h"

using namespace std;

class moveGeneration {
public:
    static ll white_pawn_moves(ll pawns, ll occupied, ll black); 

    static ll white_pawn_captures(ll pawns, ll occupied, ll black); 

    static ll black_pawn_moves(ll pawns, ll occupied, ll white); 

    static ll black_pawn_captures(ll pawns, ll occupied, ll white); 

    static ll king_moves(ll king); 

    static ll knight_moves(ll knights); 

    static ll left_ray(ll movers, ll occupied); 

    static ll right_ray(ll movers, ll occupied); 

    static ll up_ray(ll movers, ll occupied); 

    static ll down_ray(ll movers, ll occupied); 

    static ll down_left_ray(ll movers, ll occupied); 

    static ll up_left_ray(ll movers, ll occupied); 

    static ll down_right_ray(ll movers, ll occupied); 

    static ll up_right_ray(ll movers, ll occupied); 
};
