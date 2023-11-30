#include "chessboard.h"

using namespace std;

ChessBoard::ChessBoard() {
  
}

void ChessBoard::calculateAvailableMoves() {
    int r = 0;
    for (auto row: board) {
        int c = 0;
        for (auto &col: row) {
            if (col != nullptr) {
                std::vector<Vec> potentialmoves = col->getPotentialMoves();
                for (auto m: potentialmoves) {
                    if (board[r][c] == ) {
                        
                    }
                }
            }
            ++c;
        }
        ++r;
    }
}