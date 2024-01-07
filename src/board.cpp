#include "../include/board.h"

using namespace std;

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

    Board(ll WP, ll WK, ll WQ, ll WB, ll WN, ll WR, ll BP, ll BQ, ll BK, ll BN, ll BB, ll BR, bool turn, ll nevermoved) {
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
    }

    static ll white_attacks(Board *b) {
        ll attacks = 0;
        
        // pawn moves
        attacks |= moveGeneration::white_pawn_captures(b->WP, b->all, 0xFFFFFFFFFFFFFFFF);
        
        // rook moves
        attacks |= moveGeneration::left_ray(b->WR, b->all);
        attacks |= moveGeneration::right_ray(b->WR, b->all);
        attacks |= moveGeneration::down_ray(b->WR, b->all);
        attacks |= moveGeneration::up_ray(b->WR, b->all);
        
        // bishop moves
        attacks |= moveGeneration::up_left_ray(b->WB, b->all);
        attacks |= moveGeneration::up_right_ray(b->WB, b->all);
        attacks |= moveGeneration::down_left_ray(b->WB, b->all);
        attacks |= moveGeneration::down_right_ray(b->WB, b->all);

        // queen moves
        attacks |= moveGeneration::left_ray(b->WQ, b->all);
        attacks |= moveGeneration::right_ray(b->WQ, b->all);
        attacks |= moveGeneration::down_ray(b->WQ, b->all);
        attacks |= moveGeneration::up_ray(b->WQ, b->all);
        attacks |= moveGeneration::up_left_ray(b->WQ, b->all);
        attacks |= moveGeneration::up_right_ray(b->WQ, b->all);
        attacks |= moveGeneration::down_left_ray(b->WQ, b->all);
        attacks |= moveGeneration::down_right_ray(b->WQ, b->all);

        // knight moves
        attacks |= moveGeneration::knight_moves(b->WN);
        
        // king moves
        attacks |= moveGeneration::king_moves(b->WK);
        
        return attacks;
    }

    static ll black_attacks(Board *b) {
        ll attacks = 0;
        
        // pawn moves
        attacks |= moveGeneration::black_pawn_captures(b->BP, b->all, 0xFFFFFFFFFFFFFFFF);
        
        // rook moves
        attacks |= moveGeneration::left_ray(b->BR, b->all);
        attacks |= moveGeneration::right_ray(b->BR, b->all);
        attacks |= moveGeneration::down_ray(b->BR, b->all);
        attacks |= moveGeneration::up_ray(b->BR, b->all);
        
        // bishop moves
        attacks |= moveGeneration::up_left_ray(b->BB, b->all);
        attacks |= moveGeneration::up_right_ray(b->BB, b->all);
        attacks |= moveGeneration::down_left_ray(b->BB, b->all);
        attacks |= moveGeneration::down_right_ray(b->BB, b->all);

        // queen moves
        attacks |= moveGeneration::left_ray(b->BQ, b->all);
        attacks |= moveGeneration::right_ray(b->BQ, b->all);
        attacks |= moveGeneration::down_ray(b->BQ, b->all);
        attacks |= moveGeneration::up_ray(b->BQ, b->all);
        attacks |= moveGeneration::up_left_ray(b->BQ, b->all);
        attacks |= moveGeneration::up_right_ray(b->BQ, b->all);
        attacks |= moveGeneration::down_left_ray(b->BQ, b->all);
        attacks |= moveGeneration::down_right_ray(b->BQ, b->all);

        // knight moves
        attacks |= moveGeneration::knight_moves(b->BN);
        
        // king moves
        attacks |= moveGeneration::king_moves(b->BK);
        
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
                moves |= moveGeneration::white_pawn_moves(cur, b->all, b->allB);
                
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

                    if (dest & EIGHT_RANK) {
                        char promotions[4] = {'q', 'r', 'b', 'n'};
                        for (int i = 0; i < 4; i++) {
                            mv.promotion = promotions[i];
                            vector<int> res = legal_move(mv, b);
                            if (res[0]) {
                                if (res[1] ? mv.check = 1 : mv.check = 0)
                                moves_list.push_back(mv);
                            }
                        }
                    } else {
                        vector<int> res = legal_move(mv, b);
                        
                        if (res[0]) {
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
                    
                    
                    vector<int> res = legal_move(mv, b);
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
                    

                    vector<int> res = legal_move(mv, b);
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
                    

                    vector<int> res = legal_move(mv, b);
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
                    

                    vector<int> res = legal_move(mv, b);
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
                    

                    vector<int> res = legal_move(mv, b);
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
            ll in_attack = Board::black_attacks(b);

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

                        vector<int> res = legal_move(mv, b);
                    
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

                        vector<int> res = legal_move(mv, b);

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
                moves |= moveGeneration::black_pawn_moves(cur, b->all, b->allW);

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
                    
                    if (dest & FIRST_RANK) {
                        char promotions[4] = {'q', 'r', 'b', 'n'};
                        for (int i = 0; i < 4; i++) {
                            mv.promotion = promotions[i];
                            vector<int> res = legal_move(mv, b);
                            if (res[0]) {
                                if (res[1] ? mv.check = 1 : mv.check = 0)
                                moves_list.push_back(mv);
                            }
                        }
                    } else {
                        vector<int> res = legal_move(mv, b);
                        
                        if (res[0]) {
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
                    

                    vector<int> res = legal_move(mv, b);

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
                    
                    vector<int> res = legal_move(mv, b);
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

                    vector<int> res = legal_move(mv, b);
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
                    
                    vector<int> res = legal_move(mv, b);
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

                    vector<int> res = legal_move(mv, b);
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

            ll in_attack = Board::white_attacks(b);

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
                        
                        vector<int> res = legal_move(mv, b);
                        
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
                        
                        vector<int> res = legal_move(mv, b);
                        
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
                        b->BN ^= dest;
                    } else if (dest & b->BB) {
                        b->BB ^= dest;
                    } else if (dest & b->BQ) {
                        b->BQ ^= dest;
                    } else if (dest & b->BP) {
                        b->BP ^= dest;
                    } else if (dest & b->BR) {
                        b->BR ^= dest;
                    }
                }
                
                if (loc & b->WK) {
                    b->WK ^= (loc | dest);
                } else if (loc & b->WP) {
                    b-> WP ^= loc;
                    if (dest & EIGHT_RANK) {
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
                        b->WP |= dest;
                    }
                } else if (loc & b->WR) {
                    b->WR ^= (loc | dest);
                } else if (loc & b->WN) {
                    b->WN ^= (loc | dest);
                } else if (loc & b->WB) {
                    b->WB ^= (loc | dest);
                } else if (loc & b->WQ) {
                    b->WQ ^= (loc | dest);
                }
            }
        } else {
            if (move.shortcastle) {
                b->BK >>= 2;
                b->BR ^= (EIGHT_RANK & H_FILE) | (EIGHT_RANK & F_FILE);
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

                if (dest & b->allW) {
                    if ((dest & b->WN)) {
                        b->WN &= ~dest;
                    } else if (dest & b->WB) {
                        b->WB ^= dest;
                    } else if (dest & b->WQ) {
                        b->WQ ^= dest;
                    } else if (dest & b->WP) {
                        b->WP ^= dest;
                    } else if (dest & b->WR) {
                        b->WR ^= dest;
                    }
                }
                
                if (loc & b->BK) {
                    b->BK ^= (loc | dest);
                } else if (loc & b->BP) {
                    b->BP ^= loc;
                    if (dest & FIRST_RANK) {
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
                        b->BP |= dest;
                    }
                } else if (loc & b->BR) {
                    b->BR ^= (loc | dest);
                } else if (loc & b->BN) {
                    b->BN ^= (loc | dest);
                } else if (loc & b->BB) {
                    b->BB ^= (loc | dest);
                } else if (loc & b->BQ) {
                    b->BQ ^= (loc | dest);
                }
            }
        }

        b->allW = b->WK | b->WQ | b->WP | b->WN | b->WR | b->WB;
        b->allB = b->BK | b->BQ | b->BP | b->BN | b->BR | b->BB;
        b->all  = b->allW | b->allB;
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
            } else {
                if (move.promotion == 'q') {
                    b->WQ ^= destination;
                    b->WP |= location;
                } else if (move.promotion == 'r') {
                    b->WR ^= destination;
                    b->WP |= location;
                } else if (move.promotion == 'b') {
                    b->WB ^= destination;
                    b->WP |= location;
                } else if (move.promotion == 'n') {
                    b->WN ^= destination;
                    b->WP |= location;
                }

                if (move.piece == 'P' && move.promotion == ' ') {
                    b->WP ^= (destination | location);
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
                        b->BP ^= destination;
                    } else if (move.captures == 'r') {
                        b->BR ^= destination;
                    } else if (move.captures == 'q') {
                        b->BQ ^= destination;
                    } else if (move.captures == 'b') {
                        b->BB ^= destination;
                    } else if (move.captures == 'n') {
                        b->BN ^= destination;
                    } else if (move.captures == 'k') {
                        b->BK ^= destination;
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
            } else {
                if (move.promotion == 'q') {
                    b->BQ ^= destination;
                    b->BP |= location;
                } else if (move.promotion == 'r') {
                    b->BR ^= destination;
                    b->BP |= location;
                } else if (move.promotion == 'b') {
                    b->BB ^= destination;
                    b->BP |= location;
                } else if (move.promotion == 'n') {
                    b->BN ^= destination;
                    b->BP |= location;
                }

                if (move.piece == 'p' && move.promotion == ' ') {
                    b->BP ^= (destination | location);
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
                    } else if (move.captures == 'K') {
                        b->WK ^= destination;
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

    static vector<int> legal_move(Move move, Board *b) {
        vector<int> res;

        Board::make_move(b, move);

        ll b_attacks = black_attacks(b);
        ll w_attacks = white_attacks(b);

        if (((b->WK & b_attacks) && !b->turn) || ((b->BK &w_attacks) && b->turn)) {
            res.push_back(0);
        } else {
            res.push_back(1);
        }

        if (((b->WK & b_attacks) && b->turn) || ((b->BK & w_attacks) && !b->turn)) {
            res.push_back(1);
        } else {
            res.push_back(0);
        }
        
        Board::undo_move(b, move);
        return res;
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
