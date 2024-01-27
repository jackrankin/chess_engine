#pragma once
#include "move_generator.h"

using namespace std;


class Board {
public:
    int turn;

    map<ll, string> squareMap;
    map<string, ll> backwardsSM;

    ll WP;
    ll WR;
    ll WN;
    ll WB;
    ll WQ;
    ll WK;
    ll BP;
    ll BR;
    ll BN;
    ll BB;
    ll BQ;
    ll BK;

    ll empty;
    ll allW;
    ll allB;
    ll all;
    ll nevermoved;

    static void add_piece(map<char, int>& coord, char board[8][8], char piece, ll bitboard);

    static void print_char_board(Board b); 

    void read_in_char_board(char boardArray[8][8], bool turn); 

    Board(ll WP, ll WK, ll WQ, ll WB, ll WN, ll WR, ll BP, ll BQ, ll BK, ll BN, ll BB, ll BR, bool turn, ll nevermoved); 

    static ll white_attacks(Board *b); 

    static ll black_attacks(Board *b); 

    static char captures(Board *b, ll destination); 

    static vector<Move> move_gen1(Board *b); 

    static void order_moves(vector<Move> &moves_list); 

    static void make_move(Board *b, Move move); 

    static void undo_move(Board *b, Move move); 

    static vector<int> legal_move(Move move, Board *b); 

    static Move read_in_move(Board *b, string move); 
};

