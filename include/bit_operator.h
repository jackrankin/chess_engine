#pragma once

#include "globals.h"

using namespace std;

class BitManipulation {
public:
    static ll reverse_bits(ll bits); 

    static ll horizontal_mirror(ll bits); 

    static ll aeight_hone_mirror(ll a); 

    static ll aone_height_mirror(ll a); 

    static ll rotate_counterclock(ll a); 

    static ll rotate_clock(ll a); 

    static ll pop_LSB(ll *bits); 

    static int count_bits(ll bits);

    static void printBitboard(ll bits); 
};










