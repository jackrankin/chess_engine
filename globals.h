#ifndef globals
#define globals

#include <iostream>
#include <bitset>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <math.h>
#include <string.h>
#include <map>
#include <vector>
#include <algorithm>

#define ll unsigned long long

int MAX_ENGINE_DEPTH = 2;
int MAX_ENGINE_DEPTH_FINAL = 2;

// lmao
std::string zeroes = "0000000000000000000000000000000000000000000000000000000000000000";

// files
ll A_FILE = 0x8080808080808080;
ll B_FILE = A_FILE >> 1;
ll C_FILE = B_FILE >> 1;
ll D_FILE = C_FILE >> 1;
ll E_FILE = D_FILE >> 1;
ll F_FILE = E_FILE >> 1;
ll G_FILE = F_FILE >> 1;
ll H_FILE = G_FILE >> 1;

// ranks
ll EIGHT_RANK = 0b1111111100000000000000000000000000000000000000000000000000000000;
ll SEVEN_RANK = EIGHT_RANK >> 8;
ll SIX_RANK = SEVEN_RANK >> 8;
ll FIVE_RANK = SIX_RANK >> 8;
ll FOUR_RANK = FIVE_RANK >> 8;
ll THREE_RANK = FOUR_RANK >> 8;
ll TWO_RANK = THREE_RANK >> 8;
ll FIRST_RANK = EIGHT_RANK >> 56;

// diagonals
ll A_EIGHT = 0b1000000000000000000000000000000000000000000000000000000000000000;

// precomputed diagonals
ll DIAG[15] = {
    9223372036854775808U, 4647714815446351872U, 2323998145211531264U, 
    1161999622361579520U, 580999813328273408U, 290499906672525312U, 
    145249953336295424U, 72624976668147840U, 283691315109952U, 
    1108169199648U, 4328785936U, 16909320U, 66052U, 258U, 1U
};

ll ANTI_DIAG[15] = {
    128, 32832, 8405024, 2151686160, 550831656968, 141012904183812,
    36099303471055874U, 9241421688590303745U, 4620710844295151872U,
    2310355422147575808U, 1155177711073755136U, 577588855528488960U,
    288794425616760832U, 144396663052566528U, 72057594037927936U
};

// for capture move ordering
std::map<char,int> piece_val;

// coordinate definitions
std::map<ll,std::string> coordsFor;
std::map<std::string,ll> coordsBack;

struct Move {
    char piece;

    std::string destination;
    std::string location;

    char promotion;
    char captures;

    bool longcastle;
    bool shortcastle;
    bool white;
    bool check;
};

#endif