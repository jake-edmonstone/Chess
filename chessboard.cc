#include <cctype>
#include <iostream>
#include <algorithm>
#include "chessboard.h"
#include <cmath>

using namespace std;

void print(vector<string>v) {
  for (auto &i : v) cout << i << " ";
  cout << endl;
}

template<typename T> bool in(const vector<T>& vec, const T& element) {
  return find(vec.begin(), vec.end(), element) != vec.end();
}

bool ChessBoard::inrange(int r, int c) const {
  if (r < 0 || r > 7) return false;
  if (c < 0 || c > 7) return false;
  return true;
}

string ChessBoard::intPairToRankFile(int row, int col) const {
  col += 'a';
  char ch = col;
  string c = string(1, ch);
  c += to_string(row + 1);
  return c;
}

pair<int,int> ChessBoard::rankFileToIntPair(string rf) const {
  char first = rf[0];
  int col = first - 'a';
  int row = rf[1] - '0';
  return pair<int, int>(row - 1, col);
}

bool ChessBoard::isBlocked(int firstpiecey, int firstpiecex, int yshift, int xshift) const {
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

void ChessBoard::basicAddAvailableMoves() {
  int r = 0;
  for (auto &row: board) {
    int c = 0;
    for (auto &col: row) {
      if (col != nullptr) {
        col->availableMoves.clear();
        col->targets.clear();
        col->threats.clear();
        std::vector<Vec> potentialmoves = col->getPotentialMoves();
        for (auto m: potentialmoves) {
          if (!inrange(r + m.y, c + m.x)) continue;
          if (board[r + m.y][c + m.x].get() != nullptr && board[r][c]->getColour() == board[r + m.y][c + m.x]->getColour()) continue;
          if (isBlocked(r, c, m.y, m.x)) continue;
          if (col->getName() == "whitepawn" || col->getName() == "blackpawn") {
            if (m.y == 2 || m.y == -2) {
              if (col->getName() == "whitepawn" && r != 1) continue;
              if (col->getName() == "blackpawn" && r != 6) continue;
            }
            if (m.x == 0) {
              if (board[r + m.y][c + m.x] != nullptr) continue;
            } else {
              if (board[r + m.y][c + m.x] == nullptr) continue;
            }
          }
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
}

void ChessBoard::calculateAvailableMoves() {
  basicAddAvailableMoves();

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

  for (auto &row: board) {
    int k = 0;
    for (auto &col: row) {
      int l = 0; 
      if (col != nullptr) {
        if (col->getName() == "whitepawn") { // handles EnPassant for WhitePawns being captured
          string pos = intPairToRankFile(k - 1, l);
          if (col->isEnPassantable()) { // add EnPassant threats to the piece and add it as a target to surrounding ones
            if (inrange(k, l - 1) && board[k][l - 1] != nullptr && board[k][l - 1]->getName() == "blackpawn") {
              board[k][l - 1]->addAvailableMove(pos);
              board[k][l - 1]->addTarget(pos);
              col->addThreat(intPairToRankFile(k, l - 1));
            }
            if (inrange(k, l + 1) && board[k][l + 1] != nullptr && board[k][l + 1]->getName() == "blackpawn") {
              board[k][l + 1]->addAvailableMove(pos);
              board[k][l + 1]->addTarget(pos);
              col->addThreat(intPairToRankFile(k, l + 1));
            }
            col->setEnPassantable(false);
          } else { // remove EnPassant threats from the piece and remove the piece as a target for EnPassant
            if (inrange(k, l-1) && board[k][l - 1] != nullptr && board[k][l - 1]->getName() == "blackpawn" && in(col->threats, intPairToRankFile(k, l - 1))) { // if there exists threat next to it
              myRemove(col->threats, intPairToRankFile(k, l - 1));
              myRemove(board[k][l-1]->availableMoves, intPairToRankFile(k - 1, l));
              myRemove(board[k][l-1]->targets, intPairToRankFile(k - 1, l));
            }
            if (inrange(k, l+1) && board[k][l + 1] != nullptr && board[k][l + 1]->getName() == "blackpawn" && in(col->threats, intPairToRankFile(k, l + 1))) { // if there exist threat next to it
              myRemove(col->threats, intPairToRankFile(k, l + 1));
              myRemove(board[k][l+1]->availableMoves, intPairToRankFile(k - 1, l));
              myRemove(board[k][l+1]->targets, intPairToRankFile(k - 1, l));
            }
          }
        } else if (col->getName() == "blackpawn") { // handles EnPassant for BlackPawns being captured
          if (col->isEnPassantable()) {
            string pos = intPairToRankFile(k + 1, l);
            if (inrange(k, l - 1) && board[k][l - 1] != nullptr && board[k][l - 1]->getName() == "whitepawn") {
              board[k][l - 1]->addAvailableMove(pos);
              board[k][l - 1]->addTarget(pos);
              col->addThreat(intPairToRankFile(k, l - 1));
            }
            if (inrange(k, l + 1) && board[k][l + 1] != nullptr && board[k][l + 1]->getName() == "whitepawn") {
              board[k][l + 1]->addAvailableMove(pos);
              board[k][l + 1]->addTarget(pos);
              col->addThreat(intPairToRankFile(k, l + 1));
            }
            col->setEnPassantable(false);
          } else {
            if (inrange(k, l-1) && board[k][l - 1] != nullptr && board[k][l - 1]->getName() == "whitepawn" && in(col->threats, intPairToRankFile(k, l - 1))) { 
              myRemove(col->threats, intPairToRankFile(k, l - 1));
              myRemove(board[k][l-1]->availableMoves, intPairToRankFile(k + 1, l));
              myRemove(board[k][l-1]->targets, intPairToRankFile(k + 1, l));
            }
            if (inrange(k, l+1) && board[k][l + 1] != nullptr && board[k][l + 1]->getName() == "whitepawn" && in(col->threats, intPairToRankFile(k, l + 1))) {
              myRemove(col->threats, intPairToRankFile(k, l + 1));
              myRemove(board[k][l+1]->availableMoves, intPairToRankFile(k + 1, l));
              myRemove(board[k][l+1]->targets, intPairToRankFile(k + 1, l));
            }
          }
        }
      }
      l++;
    }
    k++;
  }

  // handles Castling
  bool whiteCastling = true;
  if (board[0][4].get() != nullptr && board[0][4]->getName() == "king" && board[0][4]->getColour() == "white") {
    if (board[0][7].get() != nullptr && board[0][7]->getName() == "rook" && board[0][7]->getColour() == "white") {
      if (board[0][5].get() == nullptr && board[0][6].get() == nullptr) {
        if (board[0][4]->isCastleable() && board[0][7]->isCastleable()) {
          if (board[0][5].get() == nullptr && board[0][6].get() == nullptr) {
            for (auto &r: board) {
              for (auto &c: r) {
                if (c.get() != nullptr && c->getColour() == "black" && (in(c->availableMoves, intPairToRankFile(0, 5)) || in(c->availableMoves, intPairToRankFile(0, 6)))) {
                  whiteCastling = false;
                }
                if (c.get() != nullptr && c->getColour() == "black" && (in(c->availableMoves, intPairToRankFile(0, 7)))) {
                  whiteCastling = false;
                }
              }
            }
            if (whiteCastling) {
              board[0][4]->addAvailableMove(intPairToRankFile(0, 7));
              board[0][7]->addAvailableMove(intPairToRankFile(0, 4));
            }
          }
        }
      }
    }
  }

  bool blackCastling = true;
  if (board[7][4].get() != nullptr && board[7][4]->getName() == "king" && board[7][4]->getColour() == "black") {
    if (board[7][7].get() != nullptr && board[7][7]->getName() == "rook" && board[7][7]->getColour() == "black") {
      if (board[7][5].get() == nullptr && board[7][6].get() == nullptr) {
        if (board[7][4]->isCastleable() && board[7][7]->isCastleable()) {
          if (board[7][5].get() == nullptr && board[7][6].get() == nullptr) {
            for (auto &r: board) {
              for (auto &c: r) {
                if (c.get() != nullptr && c->getColour() == "white" && (in(c->availableMoves, intPairToRankFile(7, 5)) || in(c->availableMoves, intPairToRankFile(7, 6)))) {
                  blackCastling = false;
                }
                if (c.get() != nullptr && c->getColour() == "white" && (in(c->availableMoves, intPairToRankFile(7, 7)))) { // checks if king is threatened after castle
                  blackCastling = false;
                }
              }
            }
            if (blackCastling) {
              board[7][4]->addAvailableMove(intPairToRankFile(7, 7));
              board[7][7]->addAvailableMove(intPairToRankFile(7, 4));
            }
          }
        }
      }
    }
  }

}

ChessBoard::ChessBoard() {
  gd = make_unique<GraphicsDisplay>(this->getBoard()); 
  board.resize(8); // makes 8 rows
  for (auto &i: board) i.resize(8); // makes 8 columns, each entry is nullptr
}
ChessBoard::ChessBoard(const ChessBoard &other) {
  board.resize(8); // makes 8 rows
  for (auto &i: board) i.resize(8); // makes 8 columns, each entry is nullptr
  blackPieces = other.blackPieces;
  whitePieces = other.whitePieces;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (other.board[i][j]) {
        board[i][j] = other.board[i][j]->clone();
      }
    }
  }
}
void ChessBoard::clear() {
  board.resize(0);
  board.resize(8); // makes 8 rows
  for (auto &i: board) i.resize(8); // makes 8 columns, each entry is nullptr
}
void ChessBoard::defaultSetup() {
  board[0][0] = make_unique<Rook>("white", intPairToRankFile(0,0));
  board[0][1] = make_unique<Knight>("white", intPairToRankFile(0,1));
  board[0][2] = make_unique<Bishop>("white", intPairToRankFile(0,2));
  board[0][3] = make_unique<Queen>("white", intPairToRankFile(0,3));
  board[0][4] = make_unique<King>("white", intPairToRankFile(0,4));
  board[0][5] = make_unique<Bishop>("white", intPairToRankFile(0,5));
  board[0][6] = make_unique<Knight>("white", intPairToRankFile(0,6));
  board[0][7] = make_unique<Rook>("white", intPairToRankFile(0,7));
  for (int i = 0; i < 8; ++i) board[1][i] = make_unique<WhitePawn>("white", intPairToRankFile(1,i));
  board[7][0] = make_unique<Rook>("black", intPairToRankFile(7,0));
  board[7][1] = make_unique<Knight>("black", intPairToRankFile(7,1));
  board[7][2] = make_unique<Bishop>("black", intPairToRankFile(7,2));
  board[7][3] = make_unique<Queen>("black", intPairToRankFile(7,3));
  board[7][4] = make_unique<King>("black", intPairToRankFile(7,4));
  board[7][5] = make_unique<Bishop>("black", intPairToRankFile(7,5));
  board[7][6] = make_unique<Knight>("black", intPairToRankFile(7,6));
  board[7][7] = make_unique<Rook>("black", intPairToRankFile(7,7));
  for (int i = 0; i < 8; ++i) board[6][i] = make_unique<BlackPawn>("black", intPairToRankFile(6,i));
  updatePieceLists();
  calculateAvailableMoves();
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
bool ChessBoard::isCheck(string colour) const {
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
bool ChessBoard::isCheckMate(string colour) const {
  if (!isCheck(colour)) return false;
  for (const auto &i : board) {
    for (const auto &j : i) {
      if (j) {
        if (j->getColour() == colour && !j->availableMoves.empty()) {
          return false;
        }
      }
    }
  }
  return true;
}
bool ChessBoard::isStaleMate(string colour) const {
  if (isCheck(colour)) return false;
  for (const auto &i : board) {
    for (const auto &j : i) {
      if (j) {
        if (j->getColour() == colour && !j->availableMoves.empty()) {
          return false;
        }
      }
    }
  }
  return true;
}
bool ChessBoard::isInsufficientMaterial() const {
  bool whiteInsufficient = false;
  bool blackInsufficient = false;
  if (whitePieces.size() == 1) whiteInsufficient = true;
  if (whitePieces.size() == 2) {
    if (whitePieces[0]->getName() == "bishop" || whitePieces[1]->getName() == "bishop") whiteInsufficient = true;
    if (whitePieces[0]->getName() == "knight" || whitePieces[1]->getName() == "knight") whiteInsufficient = true;
  }
  if (blackPieces.size() == 1) blackInsufficient = true;
  if (blackPieces.size() == 2) {
    if (blackPieces[0]->getName() == "bishop" || blackPieces[1]->getName() == "bishop") blackInsufficient = true;
    if (blackPieces[0]->getName() == "knight" || blackPieces[1]->getName() == "knight") blackInsufficient = true;
  }
  return whiteInsufficient && blackInsufficient;
}
bool ChessBoard::movePiece(string start, string end) {
  auto startCoords = rankFileToIntPair(start);
  auto endCoords = rankFileToIntPair(end);
  if (in(board[startCoords.first][startCoords.second]->availableMoves, end)) { // if end is in the available moves of the piece at start
    bool capturedDirectly = board[endCoords.first][endCoords.second] == nullptr ? false: true;
    bool moveMade = false;
    if (board[endCoords.first][endCoords.second].get() != nullptr && in(board[endCoords.first][endCoords.second]->availableMoves, start)) {
      if (board[startCoords.first][startCoords.second]->getColour() == board[endCoords.first][endCoords.second]->getColour()) {
        swap(board[startCoords.first][startCoords.second], board[endCoords.first][endCoords.second]);
        board[startCoords.first][startCoords.second]->setCastleable(false);
        board[endCoords.first][endCoords.second]->setCastleable(false);
        moveMade = true;
      }
    }
    if (!moveMade) {
      board[endCoords.first][endCoords.second] = move(board[startCoords.first][startCoords.second]);
      board[startCoords.first][startCoords.second] = nullptr;
    }
    if (board[endCoords.first][endCoords.second]->getName() == "whitepawn") {
      if (endCoords.first - startCoords.first == 2) { // if the WhitePawn moved two times forward
        board[endCoords.first][endCoords.second]->setEnPassantable(true);
      } else if (endCoords.second != startCoords.second && !capturedDirectly) { // if the WhitePawn captured En Passant
        board[endCoords.first - 1][endCoords.second] = nullptr; // capture
      }
    } else if (board[endCoords.first][endCoords.second]->getName() == "blackpawn") {
      if (startCoords.first - endCoords.first == 2) { // if the BlackPawn moved two times forward
        board[endCoords.first][endCoords.second]->setEnPassantable(true);
      } else if (endCoords.second != startCoords.second && !capturedDirectly) { // if the BlackPawn captured En Passant
        board[endCoords.first + 1][endCoords.second] = nullptr; // capture
      }
    }
    calculateAvailableMoves();
    updatePieceLists();
    return true;
  } else { return false; }
}

void ChessBoard::dontCheckYourself() {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      auto &piece = board[r][c];
      if (piece) {
        auto originalMoves = piece->availableMoves;
        vector<string> tempMoves;
        for (const auto &move: originalMoves) {
          ChessBoard tempBoard(*this);
          string start = intPairToRankFile(r, c);
          tempBoard.movePiece(start, move);
          if (!tempBoard.isCheck(piece->getColour())) {
            tempMoves.emplace_back(move);
          }
        }
        piece->availableMoves = tempMoves;
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
          vector<string> tmp;
          for (const auto &move : originalMoves) {
            ChessBoard tempBoard(*this);
            string start = intPairToRankFile(r, c); 
            tempBoard.movePiece(start, move);
            if (!tempBoard.isCheck(pieceColour)) {
              tmp.emplace_back(move);
            }
          }
          piece->availableMoves = tmp;
        }
      }
    }
  }
}
const vector<const AbstractPiece*> &ChessBoard::getPieces(string colour) const {
  if (colour == "white") return whitePieces;
  else if (colour == "black") return blackPieces;
  throw runtime_error("trying to get pieces of neither black or white");
  return whitePieces;
}

const AbstractPiece* ChessBoard::getPiece(string square) const {
  pair<int, int> pos = rankFileToIntPair(square);
  return board[pos.first][pos.second].get();
}

void ChessBoard::placePiece(char type, string square) {
  pair<int, int> pos = rankFileToIntPair(square);
  if (type == 'R') board[pos.first][pos.second] = make_unique<Rook>("white", square);
  else if (type == 'N') board[pos.first][pos.second] = make_unique<Knight>("white", square);
  else if (type == 'B') board[pos.first][pos.second] = make_unique<Bishop>("white", square);
  else if (type == 'Q') board[pos.first][pos.second] = make_unique<Queen>("white", square);
  else if (type == 'K') board[pos.first][pos.second] = make_unique<King>("white", square);
  else if (type == 'P') board[pos.first][pos.second] = make_unique<WhitePawn>("white", square);
  else if (type == 'r') board[pos.first][pos.second] = make_unique<Rook>("black", square);
  else if (type == 'n') board[pos.first][pos.second] = make_unique<Knight>("black", square);
  else if (type == 'b') board[pos.first][pos.second] = make_unique<Bishop>("black", square);
  else if (type == 'q') board[pos.first][pos.second] = make_unique<Queen>("black", square);
  else if (type == 'k') board[pos.first][pos.second] = make_unique<King>("black", square);
  else if (type == 'p') board[pos.first][pos.second] = make_unique<BlackPawn>("black", square);
}

void ChessBoard::removePiece(string square) {
  pair<int, int> pos = rankFileToIntPair(square);
  board[pos.first][pos.second].reset();
}

bool ChessBoard::pawnOnFirstOrLastRank() const {
  for (int i = 0; i < 8; ++i) {
    if (board[0][i]) {
      if (board[0][i]->getName() == "whitepawn" || board[0][i]->getName() == "blackpawn") {
        return true;
      }
    }
    if (board[7][i]) {
      if (board[7][i]->getName() == "whitepawn" || board[7][i]->getName() == "blackpawn") {
        return true;
      }
    }
  }
  return false;
}

const vector<vector<char>> ChessBoard::getBoard() const {
  static vector<vector<char>> charBoard(8, vector<char>(8, ' ')); // 8x8 chess board
  int row = 0;
  for (const auto &i : board) {
    int col = 0;
    for (const auto &j : i) {
      char sym = ((row + col) % 2 == 0) ? '_' : ' '; // default to empty square representation
      if (j) {
        if (j->getName() == "rook") sym = 'r';
        else if (j->getName() == "knight") sym = 'n';
        else if (j->getName() == "bishop") sym = 'b';
        else if (j->getName() == "king") sym = 'k';
        else if (j->getName() == "queen") sym = 'q';
        else if (j->getName() == "blackpawn" || j->getName() == "whitepawn") sym = 'p';
        if (j->getColour() == "white") sym = std::toupper(sym); // White pieces are uppercase
      }
      charBoard[row][col] = sym;
      ++col;
    }
    ++row;
  }
  return charBoard;
}

std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard) { // viewing the board
  int m = 0;
  int n = 0;
  char sym;
  char col = 'a';
  int row = 1;
  out << "  ";
  for (; col <= 'h'; ++col) {
    out << col << " ";
  }
  out << endl;
  for (const auto &i: chessboard.board) {
    ++m;
    out << row++ << " ";
    for (const auto &j: i) {
      ++n;
      if (!j) {
        if ((n + m) % 2 == 0) {
          out << '_';
        } else {
          out << ' ';
        }
        out << " ";
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
      out << " ";
    }
    out << endl;
  }
  return out;
}
