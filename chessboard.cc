#include <cctype>
#include <iostream>
#include "chessboard.h"

using namespace std;

bool ChessBoard::inrange(int r, int c) {
    if (r < 0 || r > 7) return false;
    if (c < 0 || c > 7) return false;
    return true;
}

string ChessBoard::intPairToRankFile(int row, int col) {
    col += 65;
    char ch = col;
    string c = to_string(ch);
    c += to_string(row);
    return c;
}

int ChessBoard::rankFileToIntPair(string rf) {
    ;
}

void ChessBoard::calculateAvailableMoves() {
    int r = 0;
    for (auto &row: board) {
        int c = 0;
        for (auto &col: row) {
            if (col != nullptr) {
                std::vector<Vec> potentialmoves = col->getPotentialMoves();
                for (auto m: potentialmoves) {
                    if (inrange(r + m.y, c + m.x)) {
                        if (board[r][c]->colour != board[r + m.y][c + m.x]->colour || board[r + m.y][c + m.x] == nullptr) {
                            // either there is an opposing piece, or no piece on the position r + m.y, c + m.x
                            if (m.x == 0 || m.y == 0 || m.x == m.y) {

                            } else {
                                board[r][c]->availableMoves.emplace_back(intPairToRankFile(r + m.y, c + m.x));
                                // target??
                            }
                        }
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
    board[0][0] = make_unique<Rook>("white");
    board[0][1] = make_unique<Knight>("white");
    board[0][2] = make_unique<Bishop>("white");
    board[0][3] = make_unique<Queen>("white");
    board[0][4] = make_unique<King>("white");
    board[0][5] = make_unique<Bishop>("white");
    board[0][6] = make_unique<Knight>("white");
    board[0][7] = make_unique<Rook>("white");
    for (int i = 0; i < 8; ++i) board[1][i] = make_unique<WhitePawn>("white");
    board[7][0] = make_unique<Rook>("black");
    board[7][1] = make_unique<Knight>("black");
    board[7][2] = make_unique<Bishop>("black");
    board[7][3] = make_unique<Queen>("black");
    board[7][4] = make_unique<King>("black");
    board[7][5] = make_unique<Bishop>("black");
    board[7][6] = make_unique<Knight>("black");
    board[7][7] = make_unique<Rook>("black");
    for (int i = 0; i < 8; ++i) board[6][i] = make_unique<BlackPawn>("black");
  }
}

std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard) {
  char sym;
  for (const auto &i: chessboard.board) {
    for (const auto &j: i) {
      if (j->getName() == "rook") sym = 'r';
      if (j->getName() == "knight") sym = 'n';
      if (j->getName() == "bishop") sym = 'b';
      if (j->getName() == "king") sym = 'k';
      if (j->getName() == "queen") sym = 'q';
      if (j->getName() == "blackpawn" || j->getName() == "whitepawn") sym = 'p';
      if (j->colour == "white") 
    }
    cout << endl;
  }
}
