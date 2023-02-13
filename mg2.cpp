#include "g2.h"

using namespace std;

struct Move {
    char piece;

    string destination;
    string location;

    char promotion;
    char captures;

    bool longcastle;
    bool shortcastle;
    bool white;
    bool check;
    ll enpassant;
};

class BitManipulation {
   public:
    static ll reverse_bits(ll bits) {
        ll reversed = 0;
        for (int i = 0; i < 64; i++) {
            if (bits & (ll)pow(2, i)) {
                reversed += (ll)pow(2, 63 - i);
            }
        }
        return reversed;
    }

    static ll horizontal_mirror(ll bits) {
        ll a = 0b0101010101010101010101010101010101010101010101010101010101010101;
        ll b = 0b0011001100110011001100110011001100110011001100110011001100110011;
        ll c = 0b111100001111000011110000111100001111000011110000111100001111;

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

class moveGeneration {
   public:
    static ll white_pawn_moves(ll pawns, ll occupied, ll black, ll enpassant) {
        ll moves = ((pawns << 8) & ~occupied);
        moves |= (((((pawns & TWO_RANK) << 8) & ~occupied) << 8) & ~occupied);
        moves |= (pawns << 9) & (~H_FILE & black);
        moves |= (pawns << 7) & (~A_FILE & black);
        return moves;
    }

    static ll white_pawn_captures(ll pawns, ll occupied, ll black, ll enpassant) {
        ll moves = 0;
        moves |= (pawns << 9) & (~H_FILE & black);
        moves |= (pawns << 7) & (~A_FILE & black);
        return moves;
    }

    static ll black_pawn_moves(ll pawns, ll occupied, ll white, ll enpassant) {
        ll moves = ((pawns >> 8) & ~occupied);
        moves |= (((((pawns & SEVEN_RANK) >> 8) & ~occupied) >> 8) & ~occupied);
        moves |= (pawns >> 9) & (~A_FILE & white);
        moves |= (pawns >> 7) & (~H_FILE & white);
        return moves;
    }

    static ll black_pawn_captures(ll pawns, ll occupied, ll white, ll enpassant) {
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

    static ll left_x_ray(ll movers, ll occupied) {
        ll k = ((occupied | H_FILE) - (movers << 1)) & (occupied | H_FILE);
        return (((k | H_FILE) - (movers << 1)) ^ k) & ~H_FILE;
    }

    static ll right_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::reverse_bits(occupied);
        ll mover = BitManipulation::reverse_bits(movers);
        return BitManipulation::reverse_bits(moveGeneration::left_ray(mover, rotated_board));
    }

    static ll right_x_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::reverse_bits(occupied);
        ll mover = BitManipulation::reverse_bits(movers);
        return BitManipulation::reverse_bits(moveGeneration::left_x_ray(mover, rotated_board));
    }

    static ll up_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::rotate_counterclock(occupied);
        ll mover = BitManipulation::rotate_counterclock(movers);
        return BitManipulation::rotate_clock(moveGeneration::left_ray(mover, rotated_board));
    }

    static ll up_x_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::rotate_counterclock(occupied);
        ll mover = BitManipulation::rotate_counterclock(movers);
        return BitManipulation::rotate_clock(moveGeneration::left_x_ray(mover, rotated_board));
    }

    static ll down_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::rotate_clock(occupied);
        ll mover = BitManipulation::rotate_clock(movers);
        return BitManipulation::rotate_counterclock(moveGeneration::left_ray(mover, rotated_board));
    }

    static ll down_x_ray(ll movers, ll occupied) {
        ll rotated_board = BitManipulation::rotate_clock(occupied);
        ll mover = BitManipulation::rotate_clock(movers);
        return BitManipulation::rotate_counterclock(moveGeneration::left_x_ray(mover, rotated_board));
    }

    static ll down_left_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & DIAG[i]) != 0) {
                ll diag = (((occupied & DIAG[i]) | 0b10000000) * H_FILE) >> 56;
                ll mvs = ((movers & DIAG[i]) * H_FILE) >> 56;
                ll moves = (diag - (mvs << 1)) ^ diag;
                res |= ((moves * H_FILE) & DIAG[i]);
            }
        }

        return res;
    }

    static ll down_left_x_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & DIAG[i]) != 0) {
                ll diag = (((occupied & DIAG[i]) | 0b10000000) * H_FILE) >> 56;
                ll mvs = ((movers & DIAG[i]) * H_FILE) >> 56;

                ll moves = ((diag - (mvs << 1)) & diag);
                moves = ((moves | H_FILE) - (mvs << 1)) ^ moves;

                res |= (((moves & ~H_FILE) * H_FILE) & DIAG[i]);
            }
        }

        return res;
    }

    static ll up_left_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & ANTI_DIAG[i]) != 0) {
                ll diag = (((occupied & ANTI_DIAG[i]) | 0b10000000) * H_FILE) >> 56;
                ll mvs = ((movers & ANTI_DIAG[i]) * H_FILE) >> 56;
                ll moves = (diag - (mvs << 1)) ^ diag;
                res |= ((moves * H_FILE) & ANTI_DIAG[i]);
            }
        }

        return res;
    }

    static ll up_left_x_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & ANTI_DIAG[i]) != 0) {
                ll diag = (((occupied & ANTI_DIAG[i]) | 0b10000000) * H_FILE) >> 56;
                ll mvs = ((movers & ANTI_DIAG[i]) * H_FILE) >> 56;
                ll moves = (diag - (mvs << 1)) & diag;

                moves = (moves - (mvs << 1)) ^ moves;

                res |= ((moves * H_FILE) & ANTI_DIAG[i]);
            }
        }

        return res;
    }

    static ll down_right_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & ANTI_DIAG[i]) != 0) {
                ll diag = (((occupied & ANTI_DIAG[i])) * H_FILE) >> 56;
                ll mvs = ((movers & ANTI_DIAG[i]) * H_FILE) >> 56;

                diag = BitManipulation::reverse_bits(diag);
                mvs = BitManipulation::reverse_bits(mvs);
                diag >>= 56;
                mvs >>= 56;
                ll moves = BitManipulation::reverse_bits((diag - (mvs << 1)) ^ diag) >> 56;

                res |= (moves * H_FILE) & ANTI_DIAG[i];
            }
        }

        return res;
    }

    static ll down_right_x_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & ANTI_DIAG[i]) != 0) {
                ll diag = (((occupied & ANTI_DIAG[i])) * H_FILE) >> 56;
                ll mvs = ((movers & ANTI_DIAG[i]) * H_FILE) >> 56;

                diag = BitManipulation::reverse_bits(diag);
                mvs = BitManipulation::reverse_bits(mvs);
                diag >>= 56;
                mvs >>= 56;

                ll moves = (diag - (mvs << 1)) & diag;
                moves = (moves - (mvs << 1)) ^ moves;

                moves = BitManipulation::reverse_bits(moves) >> 56;

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

                diag = BitManipulation::reverse_bits(diag);
                mvs = BitManipulation::reverse_bits(mvs);
                diag >>= 56;
                mvs >>= 56;
                ll moves = BitManipulation::reverse_bits((diag - (mvs << 1)) ^ diag) >> 56;

                res |= (moves * H_FILE) & DIAG[i];
            }
        }

        return res;
    }

    static ll up_right_x_ray(ll movers, ll occupied) {
        ll res = 0;

        for (int i = 0; i < 15; i++) {
            if ((movers & DIAG[i]) != 0) {
                ll diag = (((occupied & DIAG[i])) * H_FILE) >> 56;
                ll mvs = ((movers & DIAG[i]) * H_FILE) >> 56;

                diag = BitManipulation::reverse_bits(diag);
                mvs = BitManipulation::reverse_bits(mvs);
                diag >>= 56;
                mvs >>= 56;

                ll moves = (diag - (mvs << 1)) & diag;
                moves = (moves - (mvs << 1)) ^ moves;

                moves = BitManipulation::reverse_bits(moves) >> 56;

                res |= (moves * H_FILE) & DIAG[i];
            }
        }

        return res;
    }
};

class Board {
   public:
    int turn;

    map<ll, string> squareMap;
    map<string, ll> backwardsSM;

    // bitboards by color and piecetype
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

    // bitboards for move generation
    ll empty;
    ll allW;
    ll allB;
    ll all;
    ll nevermoved;
    ll enpassant;

    static void print_char_board(Board b) {
        map<char, int> coord;

        coord['a'] = 7;
        coord['b'] = 6;
        coord['c'] = 5;
        coord['d'] = 4;
        coord['e'] = 3;
        coord['f'] = 2;
        coord['g'] = 1;
        coord['h'] = 0;

        char board[8][8];

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = ' ';
            }
        }
        while (b.WK) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.WK)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'K';
        }

        while (b.WP) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.WP)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'P';
        }
        while (b.WR) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.WR)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'R';
        }
        while (b.WN) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.WN)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'N';
        }
        while (b.WB) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.WB)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'B';
        }
        while (b.WQ) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.WQ)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'Q';
        }
        while (b.BK) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.BK)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'k';
        }
        while (b.BP) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.BP)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'p';
        }
        while (b.BR) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.BR)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'r';
        }
        while (b.BN) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.BN)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'n';
        }
        while (b.BB) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.BB)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'b';
        }
        while (b.BQ) {
            string temp = coordsFor[BitManipulation::pop_LSB(&b.BQ)];
            int i = (int)(temp[1]) - 49;
            int j = coord[temp[0]];
            board[i][j] = 'q';
        }

        for (int i = 7; i >= 0; i--) {
            for (int j = 7; j >= 0; j--) {
                if ((i + j) % 2) {
                    // never use 43, just dont
                    cout << "\033[1;44;1m " << board[i][j] << " \033[0m";
                } else {
                    cout << "\033[1;45;1m " << board[i][j] << " \033[0m";
                }
            }
            cout << endl;
        }

        cout << endl;
    }

    void read_in_char_board(char boardArray[8][8], bool turn) {
        // read in a position
        string WPs = zeroes;
        string WRs = zeroes;
        string WNs = zeroes;
        string WBs = zeroes;
        string WQs = zeroes;
        string WKs = zeroes;
        string BPs = zeroes;
        string BRs = zeroes;
        string BNs = zeroes;
        string BBs = zeroes;
        string BQs = zeroes;
        string BKs = zeroes;

        string B = zeroes;
        string W = zeroes;
        string E = zeroes;

        // update the bitboards
        for (int i = 7; i >= 0; i--) {
            for (int j = 0; j < 8; j++) {
                if (boardArray[i][j] <= 90 && boardArray[i][j] != ' ') {
                    W[(i * 8) + j] = '1';
                } else if (boardArray[i][j] != ' ') {
                    B[(i * 8) + j] = '1';
                }

                if (boardArray[i][j] == 'P') {
                    WPs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'R') {
                    WRs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'N') {
                    WNs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'B') {
                    WBs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'Q') {
                    WQs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'K') {
                    WKs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'p') {
                    BPs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'r') {
                    BRs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'n') {
                    BNs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'b') {
                    BBs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'q') {
                    BQs[(i * 8) + j] = '1';
                } else if (boardArray[i][j] == 'k') {
                    BKs[(i * 8) + j] = '1';
                } else {
                    E[(i * 8) + j] = '1';
                }
            }
        }

        // convert from binary
        this->WP = stoull(WPs, 0, 2);
        this->WR = stoull(WRs, 0, 2);
        this->WN = stoull(WNs, 0, 2);
        this->WB = stoull(WBs, 0, 2);
        this->WQ = stoull(WQs, 0, 2);
        this->WK = stoull(WKs, 0, 2);
        this->BP = stoull(BPs, 0, 2);
        this->BR = stoull(BRs, 0, 2);
        this->BN = stoull(BNs, 0, 2);
        this->BB = stoull(BBs, 0, 2);
        this->BQ = stoull(BQs, 0, 2);
        this->BK = stoull(BKs, 0, 2);
        this->allW = stoull(W, 0, 2);
        this->allB = stoull(B, 0, 2);
        this->all = (stoull(W, 0, 2) | stoull(B, 0, 2));
        this->empty = stoull(E, 0, 2);
        this->nevermoved = (stoull(W, 0, 2) | stoull(B, 0, 2));
        this->enpassant = 0;
        this->turn = turn;

        string files = "abcdefgh";
        string ranks = "87654321";
        ll curSquare = 0x8000000000000000;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++, curSquare >>= 1) {
                string coords = string(1, files[j]) + string(1, ranks[i]);
                this->squareMap[curSquare] = coords;
                this->backwardsSM[coords] = curSquare;
            }
        }
    }

    Board(ll WP, ll WK, ll WQ, ll WB, ll WN, ll WR, ll BP, ll BQ, ll BK, ll BN, ll BB, ll BR, bool turn, ll enpassant, ll nevermoved) {
        this->WP = WP;
        this->WR = WR;
        this->WN = WN;
        this->WB = WB;
        this->WQ = WQ;
        this->WK = WK;
        this->BP = BP;
        this->BR = BR;
        this->BN = BN;
        this->BB = BB;
        this->BQ = BQ;
        this->BK = BK;
        this->turn = turn;
        this->enpassant = enpassant;
        this->allW = WK | WQ | WP | WN | WR | WB;
        this->allB = BK | BQ | BP | BN | BR | BB;
        this->all = this->allW | this->allB;
        this->empty = ~this->all;
        this->nevermoved = (nevermoved & this->all);

        string files = "abcdefgh";
        string ranks = "87654321";
        ll curSquare = 0x8000000000000000;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++, curSquare >>= 1) {
                string coords = string(1, files[j]) + string(1, ranks[i]);
                this->squareMap[curSquare] = coords;
                this->backwardsSM[coords] = curSquare;
                coordsFor[curSquare] = coords;
                coordsBack[coords] = curSquare;
            }
        }

        piece_val['P'] = 1;
        piece_val['p'] = 1;
        piece_val['N'] = 3;
        piece_val['n'] = 3;
        piece_val['B'] = 4;
        piece_val['b'] = 4;
        piece_val['R'] = 5;
        piece_val['r'] = 5;
        piece_val['Q'] = 9;
        piece_val['q'] = 9;

        node_count = 1;

    }

    static ll white_attacks(ll WP, ll WK, ll WQ, ll WB, ll WN, ll WR, ll all, ll allB, ll enpassant) {
        ll attacks = 0;

        while (WP) {
            ll cur = BitManipulation::pop_LSB(&WP);
            attacks |= moveGeneration::white_pawn_captures(cur, all, allB, enpassant);
        }

        while (WR) {
            ll cur = BitManipulation::pop_LSB(&WR);
            attacks |= moveGeneration::left_ray(cur, all);
            attacks |= moveGeneration::right_ray(cur, all);
            attacks |= moveGeneration::down_ray(cur, all);
            attacks |= moveGeneration::up_ray(cur, all);
        }

        while (WB) {
            ll cur = BitManipulation::pop_LSB(&WB);
            attacks |= moveGeneration::up_left_ray(cur, all);
            attacks |= moveGeneration::up_right_ray(cur, all);
            attacks |= moveGeneration::down_left_ray(cur, all);
            attacks |= moveGeneration::down_right_ray(cur, all);
        }

        while (WQ) {
            ll cur = BitManipulation::pop_LSB(&WQ);
            attacks |= moveGeneration::left_ray(cur, all);
            attacks |= moveGeneration::right_ray(cur, all);
            attacks |= moveGeneration::down_ray(cur, all);
            attacks |= moveGeneration::up_ray(cur, all);
            attacks |= moveGeneration::up_left_ray(cur, all);
            attacks |= moveGeneration::up_right_ray(cur, all);
            attacks |= moveGeneration::down_left_ray(cur, all);
            attacks |= moveGeneration::down_right_ray(cur, all);
        }

        while (WN) {
            ll cur = BitManipulation::pop_LSB(&WN);
            attacks |= moveGeneration::knight_moves(cur);
        }

        while (WK) {
            ll cur = BitManipulation::pop_LSB(&WK);
            attacks |= moveGeneration::king_moves(cur);
        }
        return attacks;
    }

    static ll black_attacks(ll BP, ll BQ, ll BK, ll BN, ll BB, ll BR, ll all, ll allW, ll enpassant) {
        ll attacks = 0;

        while (BP) {
            ll cur = BitManipulation::pop_LSB(&BP);
            attacks |= moveGeneration::black_pawn_captures(cur, all, allW, enpassant);
        }

        while (BR) {
            ll cur = BitManipulation::pop_LSB(&BR);
            attacks |= moveGeneration::left_ray(cur, all);
            attacks |= moveGeneration::right_ray(cur, all);
            attacks |= moveGeneration::down_ray(cur, all);
            attacks |= moveGeneration::up_ray(cur, all);
        }

        while (BB) {
            ll cur = BitManipulation::pop_LSB(&BB);
            attacks |= moveGeneration::up_left_ray(cur, all);
            attacks |= moveGeneration::up_right_ray(cur, all);
            attacks |= moveGeneration::down_left_ray(cur, all);
            attacks |= moveGeneration::down_right_ray(cur, all);
        }

        while (BQ) {
            ll cur = BitManipulation::pop_LSB(&BQ);
            attacks |= moveGeneration::left_ray(cur, all);
            attacks |= moveGeneration::right_ray(cur, all);
            attacks |= moveGeneration::down_ray(cur, all);
            attacks |= moveGeneration::up_ray(cur, all);
            attacks |= moveGeneration::up_left_ray(cur, all);
            attacks |= moveGeneration::up_right_ray(cur, all);
            attacks |= moveGeneration::down_left_ray(cur, all);
            attacks |= moveGeneration::down_right_ray(cur, all);
        }

        while (BN) {
            ll cur = BitManipulation::pop_LSB(&BN);
            attacks |= moveGeneration::knight_moves(cur);
        }

        while (BK) {
            ll cur = BitManipulation::pop_LSB(&BK);
            attacks |= moveGeneration::king_moves(cur);
        }

        return attacks;
    }

    static char captures(Board *b, ll destination) {
        if (b->turn) {
            if (destination & b->BP) {
                return 'p';
            } else if (destination & b->BQ) {
                return 'q';
            } else if (destination & b->BR) {
                return 'r';
            } else if (destination & b->BB) {
                return 'b';
            } else if (destination & b->BN) {
                return 'n';
            } 
        } else {
            if (destination & b->WP) {
                return 'P';
            } else if (destination & b->WQ) {
                return 'Q';
            } else if (destination & b->WR) {
                return 'R';
            } else if (destination & b->WB) {
                return 'B';
            } else if (destination & b->WN) {
                return 'N';
            } 
        }
        return ' ';
    }

    static vector<Move> move_gen1(Board *b) {
        vector<Move> moves_list;
        if (b->turn) {
            ll wp = b->WP;
            ll moves = 0;
            while (wp) {
                ll cur = BitManipulation::pop_LSB(&wp);
                moves |= moveGeneration::white_pawn_moves(cur, b->all, b->allB, b->enpassant);
                if (b->enpassant && (cur & FIVE_RANK) != 0) {

                    if ((((b->enpassant >> 1) & ~A_FILE) & cur) != 0 ||
                        (((b->enpassant << 1) & ~H_FILE) & cur) != 0) {

                        struct Move mv;
                        mv.piece = 'P';
                        mv.location = b->squareMap[cur];
                        mv.destination = b->squareMap[b->enpassant << 8];
                        mv.shortcastle = false;
                        mv.longcastle = false;
                        mv.captures = 'p';
                        mv.enpassant = b->enpassant;
                        mv.promotion = ' ';
                        mv.white = true;

                        vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                        if (res[0] == 1) {
                            if (res[1] == 0) {
                                mv.check = 0;
                            } else {
                                mv.check = 1;
                            }
                            moves_list.push_back(mv);
                        }
                    }
                }

                while (moves) {
                    ll dest = BitManipulation::pop_LSB(&moves);

                    struct Move mv;
                    mv.piece = 'P';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[dest];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = true;

                    mv.captures = captures(b, dest);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    
                    if (res[0]) {
                        if (dest & EIGHT_RANK) {
                            char promotions[4] = {'q', 'r', 'b', 'n'};
                            for (int i = 0; i < 4; i++) {
                                mv.promotion = promotions[i];
                                moves_list.push_back(mv);
                            }
                        } else {
                            if (res[0] == 1) {
                                if (res[1] == 0) {
                                    mv.check = 0;
                                } else {
                                    mv.check = 1;
                                }
                                moves_list.push_back(mv);
                            }                            
                        }
                    }
                }
            }
            ll wr = b->WR;
            while (wr) {
                ll cur = BitManipulation::pop_LSB(&wr);
                moves |= moveGeneration::left_ray(cur, b->all);
                moves |= moveGeneration::right_ray(cur, b->all);
                moves |= moveGeneration::down_ray(cur, b->all);
                moves |= moveGeneration::up_ray(cur, b->all);
                moves &= ~b->allW;
                while (moves) {

                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'R';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = true;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;
                    
                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = false;
                        } else {
                            mv.check = true;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll wb = b->WB;
            while (wb) {
                ll cur = BitManipulation::pop_LSB(&wb);
                moves |= moveGeneration::up_left_ray(cur, b->all);
                moves |= moveGeneration::up_right_ray(cur, b->all);
                moves |= moveGeneration::down_left_ray(cur, b->all);
                moves |= moveGeneration::down_right_ray(cur, b->all);
                moves &= ~b->allW;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'B';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = true;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = false;
                        } else {
                            mv.check = true;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll wq = b->WQ;
            while (wq) {
                ll cur = BitManipulation::pop_LSB(&wq);
                moves |= moveGeneration::left_ray(cur, b->all);
                moves |= moveGeneration::right_ray(cur, b->all);
                moves |= moveGeneration::down_ray(cur, b->all);
                moves |= moveGeneration::up_ray(cur, b->all);
                moves |= moveGeneration::up_left_ray(cur, b->all);
                moves |= moveGeneration::up_right_ray(cur, b->all);
                moves |= moveGeneration::down_left_ray(cur, b->all);
                moves |= moveGeneration::down_right_ray(cur, b->all);
                moves &= ~b->allW;

                while (moves) {
                    struct Move mv;
                    ll destination = BitManipulation::pop_LSB(&moves);
                    mv.piece = 'Q';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = true;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = false;
                        } else {
                            mv.check = true;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll wn = b->WN;
            while (wn) {
                ll cur = BitManipulation::pop_LSB(&wn);
                moves |= moveGeneration::knight_moves(cur);
                moves &= ~b->allW;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'N';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = true;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = false;
                        } else {
                            mv.check = true;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll wk = b->WK;
            while (wk) {
                ll cur = BitManipulation::pop_LSB(&wk);
                moves |= moveGeneration::king_moves(cur);
                moves &= ~b->allW;
                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'K';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = true;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = false;
                        } else {
                            mv.check = true;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }

            // check if castling is legal
            ll in_attack = Board::black_attacks(b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->enpassant);

            if ((in_attack & (G_FILE & FIRST_RANK)) == 0 &&
                (in_attack & (F_FILE & FIRST_RANK)) == 0 &&
                (in_attack & b->WK) == 0) {
                if ((b->WK & b->nevermoved) != 0 &&
                    (b->WK & (FIRST_RANK & E_FILE)) &&
                    ((b->WR & (H_FILE & FIRST_RANK)) & b->nevermoved) != 0) {
                    if ((((G_FILE & FIRST_RANK) | (F_FILE & FIRST_RANK)) & b->all) == 0) {
                        struct Move mv;
                        mv.piece = 'K';
                        mv.location = b->squareMap[b->WK];
                        mv.destination = b->squareMap[(G_FILE & FIRST_RANK)];
                        mv.shortcastle = true;
                        mv.longcastle = false;
                        mv.promotion = ' ';
                        mv.white = true;

                        mv.captures = ' ';
                        mv.enpassant = 0;

                        vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                        
                    }
                }
            }

            if ((in_attack & (D_FILE & FIRST_RANK)) == 0 &&
                (in_attack & (C_FILE & FIRST_RANK)) == 0 &&
                (in_attack & b->WK)                 == 0) {
                if ((b->WK & b->nevermoved) != 0 &&
                    (b->WK & (FIRST_RANK & E_FILE)) &&
                    ((b->WR & (A_FILE & FIRST_RANK)) & b->nevermoved) != 0) {
                    if ((((D_FILE & FIRST_RANK) | (C_FILE & FIRST_RANK) | (B_FILE & FIRST_RANK)) & b->all) == 0) {
                        struct Move mv;
                        mv.piece = 'K';
                        mv.location = b->squareMap[b->WK];
                        mv.destination = b->squareMap[(C_FILE & FIRST_RANK)];
                        mv.shortcastle = false;
                        mv.longcastle = true;
                        mv.promotion = ' ';
                        mv.white = true;

                        mv.captures = ' ';
                        mv.enpassant = 0;

                        vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);

                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
        } else {
            ll bp = b->BP;
            ll moves = 0;
            while (bp) {
                ll cur = BitManipulation::pop_LSB(&bp);
                moves |= moveGeneration::black_pawn_moves(cur, b->all, b->allW, b->enpassant);

                if (b->enpassant && (cur & FOUR_RANK) != 0) {
                    if ((((b->enpassant >> 1) & ~A_FILE) & cur) != 0 ||
                        (((b->enpassant << 1) & ~H_FILE) & cur) != 0) {

                        struct Move mv;
                        mv.piece = 'p';
                        mv.location = b->squareMap[cur];
                        mv.destination = b->squareMap[b->enpassant >> 8];
                        mv.shortcastle = false;
                        mv.longcastle = false;
                        mv.promotion = ' ';
                        mv.white = false;

                        mv.captures = 'P';
                        mv.enpassant = b->enpassant;

                        vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                        if (res[0] == 1) {
                            if (res[1] == 0) {
                                mv.check = 0;
                            } else {
                                mv.check = 1;
                            }
                            moves_list.push_back(mv);
                        }
                    }
                }

                while (moves) {
                    ll dest = BitManipulation::pop_LSB(&moves);
                    struct Move mv;

                    mv.piece = 'p';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[dest];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = false;

                    mv.captures = captures(b, dest);
                    mv.enpassant = 0;


                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);

                    if (res[0]) {
                        if (dest & FIRST_RANK) {
                            char promotions[4] = {'q', 'r', 'b', 'n'};
                            for (int i = 0; i < 4; i++) {
                                mv.promotion = promotions[i];
                                moves_list.push_back(mv);
                            }
                        } else {
                            if (res[1] == 0) {
                                mv.check = 0;
                            } else {
                                mv.check = 1;
                            }
                            moves_list.push_back(mv);
                            }
                    }
                }
            }
            ll br = b->BR;
            while (br) {
                ll cur = BitManipulation::pop_LSB(&br);
                moves |= moveGeneration::left_ray(cur, b->all);
                moves |= moveGeneration::right_ray(cur, b->all);
                moves |= moveGeneration::down_ray(cur, b->all);
                moves |= moveGeneration::up_ray(cur, b->all);
                moves &= ~b->allB;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'r';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = false;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll bb = b->BB;
            while (bb) {
                ll cur = BitManipulation::pop_LSB(&bb);
                moves |= moveGeneration::up_left_ray(cur, b->all);
                moves |= moveGeneration::up_right_ray(cur, b->all);
                moves |= moveGeneration::down_left_ray(cur, b->all);
                moves |= moveGeneration::down_right_ray(cur, b->all);
                moves &= ~b->allB;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'b';

                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = false;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;


                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll bq = b->BQ;
            while (bq) {
                ll cur = BitManipulation::pop_LSB(&bq);
                moves |= moveGeneration::left_ray(cur, b->all);
                moves |= moveGeneration::right_ray(cur, b->all);
                moves |= moveGeneration::down_ray(cur, b->all);
                moves |= moveGeneration::up_ray(cur, b->all);
                moves |= moveGeneration::up_left_ray(cur, b->all);
                moves |= moveGeneration::up_right_ray(cur, b->all);
                moves |= moveGeneration::down_left_ray(cur, b->all);
                moves |= moveGeneration::down_right_ray(cur, b->all);
                moves &= ~b->allB;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'q';
                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = false;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll bn = b->BN;
            while (bn) {
                ll cur = BitManipulation::pop_LSB(&bn);
                moves |= moveGeneration::knight_moves(cur);
                moves &= ~b->allB;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'n';
                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = false;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }
            ll bk = b->BK;
            while (bk) {
                ll cur = BitManipulation::pop_LSB(&bk);
                moves |= moveGeneration::king_moves(cur);
                moves &= ~b->allB;

                while (moves) {
                    ll destination = BitManipulation::pop_LSB(&moves);
                    struct Move mv;
                    mv.piece = 'k';
                    mv.location = b->squareMap[cur];
                    mv.destination = b->squareMap[destination];
                    mv.shortcastle = false;
                    mv.longcastle = false;
                    mv.promotion = ' ';
                    mv.white = false;

                    mv.captures = captures(b, destination);
                    mv.enpassant = 0;

                    vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                    if (res[0] == 1) {
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);
                    }
                }
            }

            ll in_attack = Board::white_attacks(b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->all, b->allB, b->enpassant);

            if ((in_attack & (G_FILE & EIGHT_RANK)) == 0 &&
                (in_attack & (F_FILE & EIGHT_RANK)) == 0 &&
                (in_attack & b->BK)                 == 0 &&
                ((b->BK & (E_FILE & EIGHT_RANK))    != 0) &&
                (b->BR & (H_FILE & EIGHT_RANK))     != 0) {


                if ((b->BK & b->nevermoved)             != 0 &&
                    ((b->BR & (H_FILE & EIGHT_RANK)) & b->nevermoved)  != 0) {

                    if ((((G_FILE & EIGHT_RANK) | (F_FILE & EIGHT_RANK)) & b->all) == 0) {

                        struct Move mv;
                        mv.piece = 'k';
                        mv.location = b->squareMap[b->BK];
                        mv.destination = b->squareMap[(G_FILE & EIGHT_RANK)];
                        mv.shortcastle = true;
                        mv.longcastle = false;
                        mv.promotion = ' ';
                        mv.white = false;

                        mv.captures = ' ';
                        mv.enpassant = 0;
                        vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                        
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);

                    }
                }
            }

            if ((in_attack & (D_FILE & EIGHT_RANK)) == 0 &&
                (in_attack & (C_FILE & EIGHT_RANK)) == 0 &&
                (in_attack & b->BK)                 == 0 &&
                ((b->BK & (E_FILE & EIGHT_RANK))    != 0) &&
                (b->BR & (A_FILE & EIGHT_RANK))     != 0) {

                if ((b->BK & b->nevermoved)             != 0 &&
                    ((b->BR & (H_FILE & EIGHT_RANK)) & b->nevermoved)  != 0) {

                    if ((((D_FILE & EIGHT_RANK) | (C_FILE & EIGHT_RANK) | (B_FILE & EIGHT_RANK)) & b->all) == 0) {
                        
                        struct Move mv;
                        mv.piece = 'k';
                        mv.location = b->squareMap[b->BK];
                        mv.destination = b->squareMap[(C_FILE & EIGHT_RANK)];
                        mv.shortcastle = false;
                        mv.longcastle = true;
                        mv.promotion = ' ';
                        mv.white = false;

                        mv.captures = ' ';
                        mv.enpassant = 0;
                        
                        vector<int> res = legal_move(mv, b->WP, b->WK, b->WQ, b->WB, b->WN, b->WR, b->BP, b->BQ, b->BK, b->BN, b->BB, b->BR, b->all, b->allW, b->allB, b->enpassant);
                        
                        if (res[1] == 0) {
                            mv.check = 0;
                        } else {
                            mv.check = 1;
                        }
                        moves_list.push_back(mv);

                    }
                }
            }
        }

        Board::order_moves(moves_list);
        
        return moves_list;
    }

    static void order_moves(vector<Move> &moves_list) {
        
        int last_check = 0;
        int first_nothing = moves_list.size() - 1;

        int i = 0;
        while (i <= first_nothing) {
            if (moves_list[i].check == 1) {
                Move temp = moves_list[i];
                moves_list[i] = moves_list[last_check];
                moves_list[last_check] = temp;
                last_check++;
                if (last_check >= i) {
                    i++;
                }
            } else if (moves_list[i].captures == ' ' || piece_val[moves_list[i].piece] < piece_val[moves_list[i].captures]) {
                Move temp = moves_list[i];
                moves_list[i] = moves_list[first_nothing];
                moves_list[first_nothing] = temp;
                first_nothing--;
            } else {
                i++;
            }
        }
    }

    static vector<int> legal_move(Move move, ll WP, ll WK, ll WQ, ll WB, ll WN, ll WR, ll BP, ll BQ, ll BK, ll BN, ll BB, ll BR, ll all, ll allW, ll allB, ll enpassant) {
        vector<int> res;
        if (move.white) {
            ll dest = coordsBack[move.destination];

            if (dest & BN) {
                BN ^= coordsBack[move.destination];
                allB ^= coordsBack[move.destination];
            }
            if (dest & BB) {
                BB ^= coordsBack[move.destination];
                allB ^= coordsBack[move.destination];
            }
            if (dest & BQ) {
                BQ ^= coordsBack[move.destination];
                allB ^= coordsBack[move.destination];
            }
            if (dest & BP) {
                BP ^= coordsBack[move.destination];
                allB ^= coordsBack[move.destination];
            }
            if (dest & BR) {
                BR ^= coordsBack[move.destination];
                allB ^= coordsBack[move.destination];
            }

            allW ^= (coordsBack[move.location] | coordsBack[move.destination]);
            all ^= (coordsBack[move.location] | coordsBack[move.destination]);


            if (move.piece == 'K') {
                WK ^= (coordsBack[move.location] | coordsBack[move.destination]);
            } else if (move.piece == 'Q'){
                WQ ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'R'){
                WR ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'B'){
                WB ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'N'){
                WN ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'P'){
                WP ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }

            ll b_attacks = black_attacks(BP, BQ, BK, BN, BB, BR, all, allW, enpassant);
            ll w_attacks = white_attacks(WP, WK, WQ, WB, WN, WR, all, allB, enpassant);

            if ((WK & b_attacks) != 0) {
                res.push_back(0);
            } else {
                res.push_back(1);
            }

            if (BK & w_attacks) {
                res.push_back(1);
            } else {
                res.push_back(0);
            }

        } else {
            ll dest = coordsBack[move.destination];
            if ((dest & WN)) {
                WN ^= coordsBack[move.destination];
                allW ^= coordsBack[move.destination];
            }
            if (dest & WB) {
                WB ^= coordsBack[move.destination];
                allW ^= coordsBack[move.destination];
            }
            if (dest & WQ) {
                WQ ^= coordsBack[move.destination];
                allW ^= coordsBack[move.destination];
            }
            if (dest & WP) {
                WP ^= coordsBack[move.destination];
                allW ^= coordsBack[move.destination];
            }
            if (dest & WR) {
                WR ^= coordsBack[move.destination];
                allW ^= coordsBack[move.destination];
            }

            allB ^= (coordsBack[move.location] | coordsBack[move.destination]);
            all ^= (coordsBack[move.location] | coordsBack[move.destination]);

            if (move.piece == 'k') {
                BK ^= (coordsBack[move.location] | coordsBack[move.destination]);
            } else if (move.piece == 'q'){
                BQ ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'r'){
                BR ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'b'){
                BB ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'n'){
                BN ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }else if (move.piece == 'p'){
                BP ^= (coordsBack[move.location] | coordsBack[move.destination]);
            }

            ll b_attacks = black_attacks(BP, BQ, BK, BN, BB, BR, all, allW, enpassant);
            ll w_attacks = white_attacks(WP, WK, WQ, WB, WN, WR, all, allB, enpassant);

            if ((BK & w_attacks) != 0) {
                res.push_back(0);
            } else {
                res.push_back(1);
            }


            if (WK & b_attacks) {
                res.push_back(1);
            } else {
                res.push_back(0);
            }


        }
        return res;
    }

    static void make_move(Board *b, Move move) {

        if (b->turn) {
            if (move.shortcastle) {
                b->WK >>= 2;
                b->WR &= ~(FIRST_RANK & H_FILE);
                b->WR |= (FIRST_RANK & F_FILE);
                b->nevermoved &= ~(H_FILE & FIRST_RANK);
                b->nevermoved &= ~(F_FILE & FIRST_RANK);
            } else if (move.longcastle) {
                b->WK <<= 2;
                b->WR &= ~(FIRST_RANK & A_FILE);
                b->WR |= (FIRST_RANK & D_FILE);
                b->nevermoved &= ~(A_FILE & FIRST_RANK);
                b->nevermoved &= ~(D_FILE & FIRST_RANK);
            } else {
                ll dest = coordsBack[move.destination];
                ll loc = coordsBack[move.location];

                if (dest & b->allB) {
                    if (dest & b->BN) {
                        b->BN &= ~coordsBack[move.destination];
                    } else if (dest & b->BB) {
                        b->BB &= ~coordsBack[move.destination];
                    } else if (dest & b->BQ) {
                        b->BQ &= ~coordsBack[move.destination];
                    } else if (dest & b->BP) {
                        b->BP &= ~coordsBack[move.destination];
                    } else if (dest & b->BR) {
                        b->BR &= ~coordsBack[move.destination];
                    }
                }
                
                if (loc & b->WK) {
                    b->WK &= ~loc;
                    b->WK |= dest;
                } else if (loc & b->WP) {
                    b->WP &= ~loc;
                    if ((dest >> 16) == loc) {
                        b->enpassant = dest;
                        b->WP |= dest;
                    } else if (dest & EIGHT_RANK) {
                        b->enpassant = 0;
                        if (move.promotion == 'q') {
                            b->WQ |= dest;
                        } else if (move.promotion == 'r') {
                            b->WR |= dest;
                        } else if (move.promotion == 'b') {
                            b->WB |= dest;
                        } else if (move.promotion == 'n') {
                            b->WN |= dest;
                        }
                    } else {
                        if (b->enpassant) {

                            if (((dest & (loc << 7)) || (dest & (loc << 9))) && (dest & b->BP) == 0) {
                                if (loc & FIVE_RANK) {
                                    b->BP &= ~(dest >> 8);
                                }
                            }

                        }
                        
                        b->WP |= dest;
                        b->enpassant = 0;
                        
                    }
                } else if (loc & b->WR) {
                    b->WR &= ~loc;
                    b->WR |= dest;
                    b->enpassant = 0;
                } else if (loc & b->WN) {
                    b->WN &= ~loc;
                    b->WN |= dest;
                    b->enpassant = 0;
                } else if (loc & b->WB) {
                    b->WB &= ~loc;
                    b->WB |= dest;
                    b->enpassant = 0;
                } else if (loc & b->WQ) {
                    b->WQ &= ~loc;
                    b->WQ |= dest;
                    b->enpassant = 0;
                }
            }
        } else {
            if (move.shortcastle) {
                b->BK >>= 2;
                b->BR -= (EIGHT_RANK & H_FILE);
                b->BR += (EIGHT_RANK & F_FILE);
                b->nevermoved &= ~(H_FILE & EIGHT_RANK);
                b->nevermoved &= ~(F_FILE & EIGHT_RANK);
            } else if (move.longcastle) {
                b->BK <<= 2;
                b->BR &= ~(EIGHT_RANK & A_FILE);
                b->BR |= (EIGHT_RANK & D_FILE);
                b->nevermoved &= ~(D_FILE & EIGHT_RANK);
                b->nevermoved &= ~(A_FILE & EIGHT_RANK);
                
            } else {
                ll dest = coordsBack[move.destination];
                ll loc = coordsBack[move.location];

                if ((dest & b->allW)) {
                    if ((dest & b->WN)) {
                        b->WN &= ~coordsBack[move.destination];
                    } else if (dest & b->WB) {
                        b->WB &= ~coordsBack[move.destination];
                    } else if (dest & b->WQ) {
                        b->WQ &= ~coordsBack[move.destination];
                    } else if (dest & b->WP) {
                        b->WP &= ~coordsBack[move.destination];
                    } else if (dest & b->WR) {
                        b->WR &= ~coordsBack[move.destination];
                    }
                    b->enpassant = 0;
                }
                
                if (loc & b->BK) {
                    b->BK &= ~loc;
                    b->BK |= dest;
                } else if (loc & b->BP) {
                    b->BP &= ~loc;
                    if ((dest << 16) == loc) {
                        b->enpassant = dest;
                        b->BP |= dest;
                    } else if (dest & FIRST_RANK) {
                        b->enpassant = 0;
                        if (move.promotion == 'q') {
                            b->BQ |= dest;
                        } else if (move.promotion == 'r') {
                            b->BR |= dest;
                        } else if (move.promotion == 'b') {
                            b->BB |= dest;
                        } else if (move.promotion == 'n') {
                            b->BN |= dest;
                        }
                    } else {
                        if (b->enpassant) {
                            if (((dest & (loc >> 7)) || (dest & (loc >> 9))) && (dest & b->WP) == 0) {
                                if (loc & FOUR_RANK) {
                                    b->WP &= ~(dest << 8);
                                }
                            }
                        }
                        
                        b->BP |= dest;
                        b->enpassant = 0;
                    }
                } else if (loc & b->BR) {
                    b->BR &= ~loc;
                    b->BR |= dest;
                    b->enpassant = 0;
                } else if (loc & b->BN) {
                    b->BN &= ~loc;
                    b->BN |= dest;
                    b->enpassant = 0;
                } else if (loc & b->BB) {
                    b->BB &= ~loc;
                    b->BB |= dest;
                    b->enpassant = 0;
                } else if (loc & b->BQ) {
                    b->BQ &= ~loc;
                    b->BQ |= dest;
                    b->enpassant = 0;
                }
            }
        }

        b->allW = b->WK | b->WQ | b->WP | b->WN | b->WR | b->WB;
        b->allB = b->BK | b->BQ | b->BP | b->BN | b->BR | b->BB;
        b->all = b->allW | b->allB;
        b->turn = !b->turn;

        b->nevermoved = (b->nevermoved & ~(coordsBack[move.location] | coordsBack[move.destination]));
    }

    static void undo_move(Board *b, Move move) {
        ll location = b->backwardsSM[move.location];
        ll destination = b->backwardsSM[move.destination];

        if (move.white) {
            if (move.shortcastle) {
                b->WK <<= 2;
                b->WR ^= ((F_FILE & FIRST_RANK) | (H_FILE & FIRST_RANK));
                b->nevermoved |= (b->WK | (b->WR & (H_FILE & FIRST_RANK)));
            } else if (move.longcastle) {
                b->WK >>= 2;
                b->WR ^= ((D_FILE & FIRST_RANK) | (A_FILE & FIRST_RANK));
                b->nevermoved |= (b->WK | (b->WR & (A_FILE & FIRST_RANK)));
            } else if (move.enpassant) {
                // cout << "Looking at BP in undo move";
                // BitManipulation::printBitboard(b->BP);
                // BitManipulation::printBitboard(b->enpassant);
                b->BP ^= (destination >> 8);
                b->WP ^= (destination | location);
                b->enpassant = destination >> 8;
                // cout << "Looking at BP coming out of undo move";
                // BitManipulation::printBitboard(b->BP);
            } else {
                if (move.promotion == 'q') {
                    b->WQ ^= destination;
                    b->WP |= destination;
                } else if (move.promotion == 'r') {
                    b->WR ^= destination;
                    b->WP |= destination;
                } else if (move.promotion == 'b') {
                    b->WB ^= destination;
                    b->WP |= destination;
                } else if (move.promotion == 'n') {
                    b->WN ^= destination;
                    b->WP |= destination;
                }
                if (move.piece == 'P') {
                    b->WP ^= (destination | location);
                    if ((location << 16) == destination) {
                        b->enpassant = 0;
                    }
                } else if (move.piece == 'R') {
                    b->WR ^= (destination | location);
                } else if (move.piece == 'Q') {
                    b->WQ ^= (destination | location);
                } else if (move.piece == 'B') {
                    b->WB ^= (destination | location);
                } else if (move.piece == 'N') {
                    b->WN ^= (destination | location);
                } else if (move.piece == 'K') {
                    b->WK ^= (destination | location);
                }

                if (move.captures != ' ') {
                    if (move.captures == 'p') {
                        b->BP ^= (destination);
                    } else if (move.captures == 'r') {
                        b->BR ^= (destination);
                    } else if (move.captures == 'q') {
                        b->BQ ^= (destination);
                    } else if (move.captures == 'b') {
                        b->BB ^= (destination);
                    } else if (move.captures == 'n') {
                        b->BN ^= (destination);
                    }
                }
            }
        } else {
            if (move.shortcastle) {
                b->BK <<= 2;
                b->BR ^= ((F_FILE & EIGHT_RANK) | (H_FILE & EIGHT_RANK));
                b->nevermoved |= (b->BK | (b->BR & (H_FILE & EIGHT_RANK)));
            } else if (move.longcastle) {
                b->BK >>= 2;
                b->BR ^= ((D_FILE & EIGHT_RANK) | (A_FILE & EIGHT_RANK));
                b->nevermoved |= (b->BK | (b->BR & (A_FILE & EIGHT_RANK)));
            } else if (move.enpassant) {
                b->WP ^= (destination << 8);
                b->BP ^= (destination | location);
                b->enpassant = destination << 8;
            } else {
                if (move.promotion == 'q') {
                    b->BQ ^= destination;
                    b->BP |= destination;
                } else if (move.promotion == 'r') {
                    b->BR ^= destination;
                    b->BP |= destination;
                } else if (move.promotion == 'b') {
                    b->BB ^= destination;
                    b->BP |= destination;
                } else if (move.promotion == 'n') {
                    b->BN ^= destination;
                    b->BP |= destination;
                }
                if (move.piece == 'p') {
                    b->BP ^= (destination | location);
                    if ((location >> 16) == destination) {
                        b->enpassant = 0;
                    }
                } else if (move.piece == 'r') {
                    b->BR ^= (destination | location);
                } else if (move.piece == 'q') {
                    b->BQ ^= (destination | location);
                } else if (move.piece == 'b') {
                    b->BB ^= (destination | location);
                } else if (move.piece == 'n') {
                    b->BN ^= (destination | location);
                } else if (move.piece == 'k') {
                    b->BK ^= (destination | location);
                }

                if (move.captures != ' ') {
                    if (move.captures == 'P') {
                        b->WP ^= (destination);
                    } else if (move.captures == 'R') {
                        b->WR ^= (destination);
                    } else if (move.captures == 'Q') {
                        b->WQ ^= (destination);
                    } else if (move.captures == 'B') {
                        b->WB ^= (destination);
                    } else if (move.captures == 'N') {
                        b->WN ^= (destination);
                    }
                }
            }
        }
        b->nevermoved |= (destination | location);
        b->allW = b->WK | b->WQ | b->WP | b->WN | b->WR | b->WB;
        b->allB = b->BK | b->BQ | b->BP | b->BN | b->BR | b->BB;
        b->all = b->allW | b->allB;
        b->turn = !b->turn;
    }

    static Move read_in_move(Board *b, string move) {
        Move m;
        if ((move == "e1g1") || (move == "e8g8")) {
            if (b->turn                 && 
                (b->WK & b->nevermoved) && 
                (b->WR & b->nevermoved & H_FILE) &&
                ((move == "e1g1"))) {
                m.location = "e1";
                m.destination = "g1";
            } else if (b->turn          && 
                (b->BK & b->nevermoved) && 
                (b->BR & b->nevermoved & H_FILE) &&
                ((move == "e8g8"))) {
                m.location = "e8";
                m.destination = "g8";
            }
            m.shortcastle = true;
            m.longcastle = false;
            m.white = b->turn;
            Board::make_move(b, m);
        } else if ((move == "e1c1") || (move == "e8c8")) {
            if (b->turn &&
                (b->WK & b->nevermoved) && 
                (b->WR & b->nevermoved & A_FILE) &&
                (move == "e1c1")){
                m.location = "e1";
                m.destination = "c1";
            } else if (b->turn &&
                (b->BK & b->nevermoved) && 
                (b->BR & b->nevermoved & A_FILE) && 
                (move == "e8c8")) {
                m.location = "e8";
                m.destination = "c8";
            }
            m.shortcastle = false;
            m.longcastle = true;
            m.white = b->turn;
            Board::make_move(b, m);
        } else {
            m.location = move.substr(0, 2);
            m.destination = move.substr(2, 2);
            m.white = b->turn;
            m.shortcastle = false;
            m.longcastle = false;
            
            if (move.length() == 5) {
                m.promotion = move[4];
            }

            Board::make_move(b, m);
        }

        return m;
    }
    
};


// int main() {
//     char board[8][8] =
//         {
//             {'r', 'n', ' ', ' ', 'k', ' ', ' ', ' '},
//             {' ', ' ', ' ', ' ', 'p', 'p', ' ', ' '},
//             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
//             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
//             {' ', ' ', 'p', 'p', 'p', ' ', 'p', ' '},
//             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
//             {'P', 'P', ' ', ' ', ' ', 'P', ' ', 'P'},
//             {' ', ' ', ' ', ' ', 'K', ' ', ' ', ' '},
//         };

//     Board b = Board(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
    
//     b.read_in_char_board(board, 1);
//     Board::print_char_board(b);

//     vector<Move> moves = Board::move_gen1( &b );

//     string move;
//     cin>>move;

//     Board::read_in_move(&b, move);
    
//     Board::print_char_board(b);

//     moves = Board::move_gen1(&b);

//     for (int i = 0;i < moves.size(); i++) {
//         cout << moves[i].location << moves[i].destination << moves[i].enpassant << endl;
//     }

//     Board::make_move(&b, moves[0]);

//     Board::print_char_board(b);


//     Board::undo_move(&b, moves[0]);

//     Board::print_char_board(b);

// }
