#include "chessboard.h"

using namespace std;

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

ChessBoard::ChessBoard(string config) {
  board.resize(8);
  for (auto &i: board) i.resize(8);
  if (config == "empty") return;
  if (config == "default") {
    board[0][0] = make_unique<Rook>();
    board[0][0] = make_unique<Rook>();
  }
}