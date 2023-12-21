#include "globals.h"

using namespace std;

class BitManipulation {
   public:
    static ll reverse_bits(ll bits) {

        ll m2 = 0xFFFF0000FFFF0000;
        ll m3 = 0xFF00FF00FF00FF00;
        ll m4 = 0xF0F0F0F0F0F0F0F0;
        ll m5 = 0xCCCCCCCCCCCCCCCC;
        ll m6 = 0xAAAAAAAAAAAAAAAA;

        bits =  ((     bits) >> 32) | ((bits)         << 32); // no need to and bc just half
        bits =  ((m2 & bits) >> 16) | ((bits & (~m2)) << 16);
        bits =  ((m3 & bits) >> 8)  | ((bits & (~m3)) << 8);
        bits =  ((m4 & bits) >> 4)  | ((bits & (~m4)) << 4);
        bits =  ((m5 & bits) >> 2)  | ((bits & (~m5)) << 2);
        bits =  ((m6 & bits) >> 1)  | ((bits & (~m6)) << 1);

        return bits;
    }

    static ll horizontal_mirror(ll bits) {
        ll a = 0x5555555555555555;
        ll b = 0x3333333333333333;
        ll c = 0xF0F0F0F0F0F0F0F;

        bits = ((bits >> 1) & a) + 2 * (bits & a);
        bits = ((bits >> 2) & b) + 4 * (bits & b);
        bits = ((bits >> 4) & c) + 16 * (bits & c);

        return bits;
    }

    static ll aeight_hone_mirror(ll a) {
        ll d1 = 0x5500550055005500 << 1;
        ll d2 = 0x3333000033330000 << 2;
        ll d3 = 0xf0f0f0f000000000;

        ll otherBits = (a << 36) & d3;
        a = otherBits ^ ((a >> 36) & (d3 >> 36) ^ ((a & (d3 >> 32)) ^ (a & (d3 >> 4))));

        otherBits = ((a << 18) & d2);
        a = otherBits ^ ((a >> 18) & (d2 >> 18)) ^ (a & (d2 >> 2) ^ (a & d2 >> 16));

        otherBits = (a << 9) & d1;
        a = otherBits ^ ((a >> 9) & (d1 >> 9)) ^ (a & (d1 >> 1) ^ (a & (d1 >> 8)));

        return a;
    }

    static ll aone_height_mirror(ll a) {
        ll d1 = 0x5500550055005500;
        ll d2 = 0x3333000033330000;
        ll d3 = 0x0f0f0f0f00000000;
        ll otherBits = 0;

        otherBits = (a << 28) & d3;
        a = otherBits ^ ((a >> 28) & (d3 >> 28)) ^ ((a & (d3 << 4)) ^ (a & (d3 >> 32)));

        otherBits = ((a << 14) & d2);
        a = otherBits ^ ((a >> 14) & (d2 >> 14)) ^ (a & (d2 << 2) ^ (a & (d2 >> 16)));

        otherBits = (a << 7) & d1;
        a = otherBits ^ ((a >> 7) & (d1 >> 7)) ^ (a & (d1 << 1) ^ (a & (d1 >> 8)));

        return a;
    }

    static ll rotate_counterclock(ll a) {
        return BitManipulation::aone_height_mirror(BitManipulation::horizontal_mirror(a));
    }

    static ll rotate_clock(ll a) {
        return BitManipulation::aeight_hone_mirror(BitManipulation::horizontal_mirror(a));
    }

    static ll pop_LSB(ll *bits) {
        ll res = *bits ^ ((*bits - 1) & *bits);
        *bits = *bits ^ res;
        return res;
    }

    static void printBitboard(ll bits) {
        bitset<64> b(bits);
        string bitsets = b.to_string();
        for (int i = 0; i < 64; i++) {
            if (i % 8 == 0) {
                cout << endl;
            }
            cout << bitsets[i];
            cout << " ";
        }
        cout << endl;
    }
};
