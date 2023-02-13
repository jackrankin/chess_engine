#include "m2.cpp"

class Play {
    public:

    static void play_on_console() {
        char board[8][8] =
        {
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
        };

        Board b = Board(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
        
        b.read_in_char_board(board, 1);

        cout << "Enter moves like this: e2e4, e1g1, e7e8Q" << endl;
        cout << "Hit enter to continue..." << endl;
        cin.get();
        
        for(int i = 0;; i++) {
            
            fputs("\x1b[2J", stdout);
            fputs("\x1b[2H", stdout);
            Board::print_char_board(b);
            cout << "Positions Searched: " << node_count << endl;
            if (i%2 == 0) {
                string move = "";
                cin >> move;
                Board::read_in_move(&b, move);
                vector<Move> ms = Board::move_gen1(&b);
                
                if (ms.size() == 0) {
                    Board::print_char_board(b);
                    cout << "u won!! or maybe u stalemated, i'm honestly not sure i'm pretty stupid";
                    break;
                }
            } else {
                Move m = Engine::find_move(&b);
                Board::make_move(&b, m);
                vector<Move> ms = Board::move_gen1(&b);
                if (ms.size() == 0) {
                    Board::print_char_board(b);
                    cout << "u lost, :(. or maybe u stalemated, i'm honestly not sure i'm pretty stupid";
                    break;
                }
            }
        }
    }
};



int main() {
    
    Play::play_on_console();
    
    return 0;
}

