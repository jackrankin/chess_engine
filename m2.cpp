#include "mg2.cpp"

using namespace std;

class Heuristic {
   public:
    static int material(Board *board) {
        
        Board fake = *board;

        int material = 0;
        while (fake.WP) {
            BitManipulation::pop_LSB(&fake.WP);
            material++;
        }
        while (fake.WR) {
            BitManipulation::pop_LSB(&fake.WR);
            material += 9;
        }
        while (fake.WN) {
            BitManipulation::pop_LSB(&fake.WN);
            material += 4;
        }
        while (fake.WB) {
            BitManipulation::pop_LSB(&fake.WB);
            material += 6;
        }
        while (fake.WQ) {
            BitManipulation::pop_LSB(&fake.WQ);
            material += 20;
        }
        while (fake.BP) {
            BitManipulation::pop_LSB(&fake.BP);
            material--;
        }
        while (fake.BR) {
            BitManipulation::pop_LSB(&fake.BR);
            material -= 9;
        }
        while (fake.BN) {
            BitManipulation::pop_LSB(&fake.BN);
            material -= 5;
        }
        while (fake.BB) {
            BitManipulation::pop_LSB(&fake.BB);
            material -= 6;
        }
        while (fake.BQ) {
            BitManipulation::pop_LSB(&fake.BQ);
            material -= 20;
        }
        return 3*material;
    }
    static int checkmate(Board *board) {
        vector<Move> moos = Board::move_gen1(board);
        if (moos.size() == 0) {
            if (board->turn == 1) {
                ll attacks = Board::black_attacks(board->BP, board->BK, board->BQ, board->BB, board->BN, board->BR, board->all, board->allW, board->enpassant);
                if (board->WK & attacks) {
                    return INT_MAX;
                } else {
                    return 1;
                }
            } else {
                ll attacks = Board::white_attacks(board->WP, board->WK, board->WQ, board->WB, board->WN, board->WR, board->all, board->allB, board->enpassant);
                if (board->BK & attacks) {
                    return -INT_MAX;
                } else {
                    return -1;
                }
            }
        } else {
            return 0;
        }
    }
    static int control(Board *board) {
        ll b_attacks = Board::black_attacks(board->BP, board->BQ, board->BK, board->BN, board->BB, board->BR, board->all, board->allW, board->enpassant);
        ll w_attacks = Board::white_attacks(board->WP, board->WK, board->WQ, board->WB, board->WN, board->WR, board->all, board->allB, board->enpassant);
        
        int w = 0;
        int b = 0;

        while(b_attacks) {
            BitManipulation::pop_LSB(&b_attacks);
            b -= 10;
        }
        while(w_attacks) {
            BitManipulation::pop_LSB(&w_attacks);
            w += 10;
        }
        return (w + b);
    }
};

class Engine {
   public:

    static Move find_move(Board *board) {
        
        int maxi = -INT_MAX;
        int mini = INT_MAX;
        Move smol;
        Move swol;
        vector<Move> moos = Board::move_gen1(board);

        for (int i = 0; i < moos.size(); i++) {
            Board::make_move(board, moos[i]);
            int score = minimax(board);
            Board::undo_move(board, moos[i]);
            if (maxi < score) {
                maxi = score;
                swol = moos[i];
            }
            if (mini > score) {
                mini = score;
                smol = moos[i];
            }

            // killer moves
            if (board->turn && maxi > 30) {
                break;
            } else if (!board->turn && mini < -30) {
                break;
            }
        }
        if (board->turn) {
            return swol;
        } else {
            return smol;
        }
    }

    static int minimax(Board *board, int alpha = -INT_MAX, int beta = INT_MAX, int depth = 0, int big_depth = 0) {
        node_count++;
        if (depth == MAX_ENGINE_DEPTH || big_depth == MAX_ENGINE_DEPTH_FINAL) {
            return evaluate_board(board);
        }

        if (board->turn) {
            int maximum = -INT_MAX;
            vector<Move> moos = Board::move_gen1(board);
            
            for (int i = 0; i < moos.size(); i++) {
                Board::make_move(board, moos[i]);
                
                int score = 0;
                
                if (!moos[i].check && moos[i].captures == ' ') {
                    score = minimax(board, alpha, beta, depth + 1, big_depth + 1);
                } else {
                    score = minimax(board, alpha, beta, depth, big_depth + 1);
                }

                Board::undo_move(board, moos[i]);

                maximum = max(score, maximum);

                if (maximum > beta || score > 5) {
                    break;
                }

            }
            alpha = max(maximum, alpha);
            return maximum;
        } else {
            int minimum = INT_MAX;
            vector<Move> moos = Board::move_gen1(board);

            for (int i = 0; i < moos.size(); i++) {
                Board::make_move(board, moos[i]);
                
                int score = 0;
                
                if (!moos[i].check && moos[i].captures == ' ') {
                    score = minimax(board, alpha, beta, depth + 1, big_depth + 1);
                } else {
                    score = minimax(board, alpha, beta, depth, big_depth + 1);
                }
                
                Board::undo_move(board, moos[i]);

                minimum = min(score, minimum);
                if (minimum < alpha || score < -5) {
                    break;
                }
            }
            beta = min(beta, minimum);
            return minimum;
        }
    }

    static double evaluate_board(Board *board) {
        return Heuristic::checkmate(board) + Heuristic::control(board) + Heuristic::material(board);
    }
};




int main() {
    char board[8][8] =
        {
            {' ', ' ', ' ', ' ', 'k', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'K', ' ', ' '},
        };

    Board b = Board(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    b.read_in_char_board(board, 0);
    Board::print_char_board(b);

    Move best = Engine::find_move(&b);

    Board::make_move(&b, best);

    Board::print_char_board(b);

    vector<Move> ms = Board::move_gen1(&b);

    Board::undo_move(&b, best);
    
    Board::print_char_board(b);

    
}
