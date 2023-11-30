#include "chessboard.h"

using namespace std;

<<<<<<< HEAD
=======
bool ChessBoard::inrange(int r, int c) {
    if (r < 0 || r > 7) return false;
    if (c < 0 || c > 7) return false;
    return true;
}

>>>>>>> 73406f0 (More edits)
void ChessBoard::calculateAvailableMoves() {
    int r = 0;
    for (auto row: board) {
        int c = 0;
        for (auto &col: row) {
            if (col != nullptr) {
                std::vector<Vec> potentialmoves = col->getPotentialMoves();
                for (auto m: potentialmoves) {
                    if (inrange(r + m.y, c + m.x)) {
                        y + 5;
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