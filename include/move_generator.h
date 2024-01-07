#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "bit_operator.h"

using namespace std;

class moveGeneration {
public:
    static ll white_pawn_moves(ll pawns, ll occupied, ll black) {
        ll moves = ((pawns << 8) & ~occupied);
        moves |= (((((pawns & TWO_RANK) << 8) & ~occupied) << 8) & ~occupied);
        moves |= (pawns << 9) & (~H_FILE & black);
        moves |= (pawns << 7) & (~A_FILE & black);
        return moves;
    }

    static ll white_pawn_captures(ll pawns, ll occupied, ll black) {
        ll moves = 0;
        moves |= (pawns << 9) & (~H_FILE & black);
        moves |= (pawns << 7) & (~A_FILE & black);
        return moves;
    }

    static ll black_pawn_moves(ll pawns, ll occupied, ll white) {
        ll moves = ((pawns >> 8) & ~occupied);
        moves |= (((((pawns & SEVEN_RANK) >> 8) & ~occupied) >> 8) & ~occupied);
        moves |= (pawns >> 9) & (~A_FILE & white);
        moves |= (pawns >> 7) & (~H_FILE & white);
        return moves;
    }

    static ll black_pawn_captures(ll pawns, ll occupied, ll white) {
        ll moves = 0;
        moves |= (pawns >> 9) & (~A_FILE & white);
        moves |= (pawns >> 7) & (~H_FILE & white);
        return moves;
    }

    static ll king_moves(ll king) {
        ll moves = 0;

        moves |= (king << 1) & ~H_FILE;
        moves |= (king << 9) & ~H_FILE;
        moves |= (king << 7) & ~A_FILE;
        moves |= (king >> 8);
        moves |= (king >> 1) & ~A_FILE;
        moves |= (king >> 9) & ~A_FILE;
        moves |= (king >> 7) & ~H_FILE;
        moves |= (king << 8);

        return moves;
    }

    static ll knight_moves(ll knights) {
        ll moves = 0;

        moves |= (knights << 6 & ~(A_FILE | B_FILE));
        moves |= (knights << 10 & ~(G_FILE | H_FILE));
        moves |= (knights << 15 & ~A_FILE);
        moves |= (knights << 17 & ~H_FILE);
        moves |= (knights >> 6 & ~(G_FILE | H_FILE));
        moves |= (knights >> 10 & ~(A_FILE | B_FILE));
        moves |= (knights >> 15 & ~H_FILE);
        moves |= (knights >> 17 & ~A_FILE);

        return moves;
    }

    static ll left_ray(ll movers, ll occupied) {
        return (((occupied | H_FILE) - (movers << 1)) ^ (occupied | H_FILE)) & ~H_FILE;
    }

    static ll right_ray(ll movers, ll occupied) {
        occupied = BitManipulation::reverse_bits(occupied);
        movers = BitManipulation::reverse_bits(movers);
        return BitManipulation::reverse_bits(moveGeneration::left_ray(movers, occupied));
    }

    static ll up_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::rotate_counterclock(occupied);
        ll mover = BitManipulation::rotate_counterclock(movers);
        return BitManipulation::rotate_clock(moveGeneration::left_ray(mover, rotated_board));
    }

    static ll down_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::rotate_clock(occupied);
        ll mover = BitManipulation::rotate_clock(movers);
        return BitManipulation::rotate_counterclock(moveGeneration::left_ray(mover, rotated_board));
    }

    static ll down_left_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & DIAG[i] & ~A_FILE) != 0) {
                ll diag = (((occupied & DIAG[i]) | 0b10000000) * H_FILE) >> 56;
                ll mvs = ((movers & DIAG[i]) * H_FILE) >> 56;
                ll moves = (diag - (mvs << 1)) ^ diag;
                res |= ((moves * H_FILE) & DIAG[i]);
            }
        }
        
        return res;
        
    }

    static ll up_left_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if (movers & ANTI_DIAG[i]) {
                
                ll diag = ((((occupied & ANTI_DIAG[i]) | 0b100000000) * H_FILE) >> 56);
                ll mvs = ((movers & ANTI_DIAG[i]) * H_FILE) >> 56;
                ll moves = moveGeneration::left_ray(mvs, diag); 

                res |= ((moves * H_FILE) & ANTI_DIAG[i]);
            }
        }

        return res;
    }

    static ll down_right_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & ANTI_DIAG[i]) != 0) {
                ll diag = ((occupied & ANTI_DIAG[i]) * H_FILE) >> 56;
                ll mvs = ((movers & ANTI_DIAG[i]) * H_FILE) >> 56;

                diag = BitManipulation::reverse_bits(diag) >> 56;
                mvs = BitManipulation::reverse_bits(mvs) >> 56;

                ll moves = (BitManipulation::reverse_bits((diag - (mvs << 1)) ^ diag) >> 56);

                res |= (moves * H_FILE) & ANTI_DIAG[i];
            }
        }

        return res;
    }

    static ll up_right_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & DIAG[i]) != 0) {
                ll diag = (((occupied & DIAG[i])) * H_FILE) >> 56;
                ll mvs = ((movers & DIAG[i]) * H_FILE) >> 56;

                diag = BitManipulation::reverse_bits(diag) >> 56;
                mvs = BitManipulation::reverse_bits(mvs) >> 56;

                ll moves = BitManipulation::reverse_bits((diag - (mvs << 1)) ^ diag) >> 56;

                res |= (moves * H_FILE) & DIAG[i];
            }
        }

        return res;
    }

};

#endif
