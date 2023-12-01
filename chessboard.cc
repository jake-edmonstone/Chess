#include <cctype>
#include <iostream>
#include <algorithm>
#include "chessboard.h"

using namespace std;

template<typename T> bool in(const vector<T>& vec, const T& element) {
  return find(vec.begin(), vec.end(), element) != vec.end();
}

bool ChessBoard::inrange(int r, int c) {
  if (r < 0 || r > 7) return false;
  if (c < 0 || c > 7) return false;
  return true;
}

string ChessBoard::intPairToRankFile(int row, int col) {
  col += 'a';
  char ch = col;
  string c = string(1, ch);
  c += to_string(row + 1);
  return c;
}

pair<int,int> ChessBoard::rankFileToIntPair(string rf) {
  char first = rf[0];
  int col = first - 'a';
  int row = rf[1] - '0';
  return pair<int, int>(row - 1, col);
}

bool ChessBoard::isBlocked(int firstpiecey, int firstpiecex, int yshift, int xshift) {
  if (xshift == 0) {
    if (yshift > 0) {
      for (int y = 1; y < yshift; y++) {
        if (board[firstpiecey + y][firstpiecex].get() != nullptr) return true;
      }
    } else {
        for (int y = -1; y > yshift; y--) {
          if (board[firstpiecey + y][firstpiecex].get() != nullptr) return true;
        }
      }
  } else if (yshift == 0) {
      if (xshift > 0) {
        for (int x = 1; x < xshift; x++) {
            if (board[firstpiecey][firstpiecex + x].get() != nullptr) return true;
        }
      } else {
          for (int x = -1; x > xshift; x--) {
            if (board[firstpiecey][firstpiecex + x].get() != nullptr) return true;
          }
        }
  } else if (xshift == yshift) {
      if (xshift > 0) {
        for (int i = 1; i < xshift; i++) {
          if (board[firstpiecey + i][firstpiecex + i] != nullptr) return true;
        }
      } else  {
          for (int i = -1; i > xshift; i--) {
            if (board[firstpiecey + i][firstpiecex + i] != nullptr) return true;
          }
        }
  } else if (xshift == -yshift) {
      if (xshift > 0) {
        for (int i = 1; i < xshift; i++) {
            if (board[firstpiecey - i][firstpiecex + i] != nullptr) return true;
        }
      } else {
          for (int i = -1; i > xshift; i--) {
            if (board[firstpiecey - i][firstpiecex + i] != nullptr) return true;
          }
        }
    }
  return false;
}

void ChessBoard::calculateAvailableMoves() {
  static int count = 0;
  ++count;
  cout << count << endl;
  int r = 0;
  for (auto &row: board) {
    int c = 0;
    for (auto &col: row) {
      if (col != nullptr) {
        std::vector<Vec> potentialmoves = col->getPotentialMoves();
        for (auto m: potentialmoves) {
          if (!inrange(r + m.y, c + m.x)) continue;
          if (board[r + m.y][c + m.x].get() != nullptr && board[r][c]->getColour() == board[r + m.y][c + m.x]->getColour()) continue;
          if (isBlocked(r, c, m.y, m.x)) continue;
          board[r][c]->addAvailableMove(intPairToRankFile(r + m.y, c + m.x));
          if (board[r + m.y][c + m.x].get() != nullptr) {
            board[r][c]->addTarget(intPairToRankFile(r + m.y, c + m.x));
          }
        }
      }
          ++c;
    }
      ++r;
  }
  int i = 0;
  for (auto &row: board) {
    int j = 0;
    for (auto &col: row) {
      if (col != nullptr) {
        for (auto &availabletarget: col->targets) {
          pair<int, int> target = rankFileToIntPair(availabletarget);
          board[target.first][target.second]->addThreat(intPairToRankFile(i, j));
        }
      }
      j++;
    }
    i++;
  }
  dontCheckYourself();
  getOutOfCheck();
}

ChessBoard::ChessBoard(string config) {
  board.resize(8); // makes 8 rows
  for (auto &i: board) i.resize(8); // makes 8 columns, each entry is nullptr
  if (config == "empty") return;
  if (config == "default") { // official board setup
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
  updatePieceLists();
  calculateAvailableMoves();
}
ChessBoard::ChessBoard(const ChessBoard &other): ChessBoard{"empty"} {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (other.board[i][j]) {
        board[i][j] = other.board[i][j]->clone();
      }
    }
  }
  updatePieceLists();
}
void ChessBoard::updatePieceLists() {
  blackPieces.resize(0);
  whitePieces.resize(0);
  for (const auto &i: board) {
    for (const auto &j: i) {
      if (j) {
        if (j->getColour() == "white") whitePieces.emplace_back(j.get());
        else if (j->getColour() == "black") blackPieces.emplace_back(j.get());
      }
    }
  }
}
bool ChessBoard::isCheck(string colour) {
  for (const auto &i: board) {
    for (const auto &j : i) {
      if (j) {
        if (j->getName() == "king" && j->getColour() == colour) {
          if (j->threats.empty()) return false;
          else return true;
        }
      }
    }
  }
  return false;
}
bool ChessBoard::movePiece(string start, string end) {
  auto startCoords = rankFileToIntPair(start);
  auto endCoords = rankFileToIntPair(end);
  if (in(board[startCoords.first][startCoords.second]->availableMoves, end)) { // if end is in the available moves of the piece at start
    board[endCoords.first][endCoords.second] = move(board[startCoords.first][startCoords.second]);
    board[startCoords.first][startCoords.second] = nullptr;
    calculateAvailableMoves();
    return true;
  } else { return false; }
}

void ChessBoard::dontCheckYourself() {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto &piece = board[r][c];
      if (piece) {
        auto originalMoves = piece->availableMoves;
        piece->availableMoves.clear();
        for (const auto &move: originalMoves) {
          ChessBoard tempBoard(*this);
          string start = intPairToRankFile(r, c);
          tempBoard.movePiece(start, move);
          if (!tempBoard.isCheck(piece->getColour())) {
            piece->availableMoves.emplace_back(move);
          }
        }
      }
    }
  }
}
void ChessBoard::getOutOfCheck() {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto &piece = board[r][c];
      if (piece) {
        string pieceColour = piece->getColour();
        if (isCheck(pieceColour)) {
          auto originalMoves = piece->availableMoves;
          piece->availableMoves.clear();
          for (const auto &move : originalMoves) {
            ChessBoard tempBoard(*this);
            string start = intPairToRankFile(r, c);
            tempBoard.movePiece(start, move);
            if (!tempBoard.isCheck(pieceColour)) {
              piece->availableMoves.emplace_back(move);
            }
          }
        }
      }
    }
  }
}
std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard) { // viewing the board
  char sym;
  for (const auto &i: chessboard.board) {
    for (const auto &j: i) {
      if (!j) {
        out << '_';
        continue;
      }
      if (j->getName() == "rook") sym = 'r';
      else if (j->getName() == "knight") sym = 'n';
      else if (j->getName() == "bishop") sym = 'b';
      else if (j->getName() == "king") sym = 'k';
      else if (j->getName() == "queen") sym = 'q';
      else if (j->getName() == "blackpawn" || j->getName() == "whitepawn") sym = 'p';
      if (j->getColour() == "white") out << static_cast<char>(toupper(sym)); // White pieces are uppercase
      else out << sym;
    }
    cout << endl;
  }
  return out;
}