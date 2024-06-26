#include <cctype>
#include <iostream>
#include <algorithm>
#include "chessboard.h"
#include <cmath>

using namespace std;

// prints a vector, for debugging purposes
void print(vector<string>v) {
  for (auto &i : v) cout << i << " ";
  cout << endl;
}
// returns true if element is in vec
template<typename T> bool in(const vector<T>& vec, const T& element) {
  return find(vec.begin(), vec.end(), element) != vec.end();
}
// removes element from vec
template<typename T> bool myRemove(std::vector<T>& vec, const T& element) {
  bool removed = false;
  for (auto it = vec.begin(); it != vec.end(); ) {
    if (*it == element) { vec.erase(it); removed = true; } else ++it;
  }
  return removed;
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

void ChessBoard::addThreats() {
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
}
// determines if ap is a valid EnPassantable piece
bool ChessBoard::enPassantable(const AbstractPiece * ap, string name) {
  return ap != nullptr && ap->getName() == name && ap->isEnPassantable();
}

void ChessBoard::addEnPassantMoves() {
  int k = 0;
  for (auto &row: board) {
    int l = 0; 
    for (auto &col: row) {
      AbstractPiece* left = inrange(k, l - 1) ? board[k][l - 1].get() : nullptr;
      AbstractPiece* right = inrange(k, l + 1) ? board[k][l + 1].get(): nullptr;
      if (enPassantable(col.get(), "whitepawn")) { // Add targets to the Pieces next to it, and add threats to itself
        string target = intPairToRankFile(k - 1, l);
        if (left && left->getName() == "blackpawn") {
            left->addAvailableMove(target);
            left->addTarget(target);
            col->addThreat(intPairToRankFile(k, l - 1));
        }
        if (right && right->getName() == "blackpawn") {
          right->addAvailableMove(target);
          right->addTarget(target);
          col->addThreat(intPairToRankFile(k, l + 1));
        }
        col->setEnPassantable(false);
      } else if (col.get() != nullptr && col->getName() == "whitepawn") {
        if (left != nullptr && threatens(dynamic_cast<AbstractPiece*>(left), col.get()) && left->getName() == "blackpawn") {
          myRemove(col->threats, intPairToRankFile(k, l - 1));
          myRemove(left->availableMoves, intPairToRankFile(k - 1, l));
          myRemove(left->targets, intPairToRankFile(k - 1, l));
        } else if (right != nullptr && threatens(dynamic_cast<AbstractPiece*>(right), col.get()) && right->getName() == "blackpawn") {
          myRemove(col->threats, intPairToRankFile(k, l + 1));
          myRemove(right->availableMoves, intPairToRankFile(k - 1, l));
          myRemove(right->targets, intPairToRankFile(k - 1, l));
        }
      }

      if (enPassantable(col.get(), "blackpawn")) { // Add targets to the Pieces next to it, and add threats to itself
        string target = intPairToRankFile(k + 1, l);
        if (left && left->getName() == "whitepawn") {
            left->addAvailableMove(target);
            left->addTarget(target);
            col->addThreat(intPairToRankFile(k, l - 1));
        }
        if (right && right->getName() == "whitepawn") {
          right->addAvailableMove(target);
          right->addTarget(target);
          col->addThreat(intPairToRankFile(k, l + 1));
        }
        col->setEnPassantable(false);
      } else if (col.get() != nullptr && col->getName() == "blackpawn") {
        if (left != nullptr && threatens(dynamic_cast<AbstractPiece*>(left), col.get()) && left->getName() == "whitepawn") {
          myRemove(col->threats, intPairToRankFile(k, l - 1));
          myRemove(left->availableMoves, intPairToRankFile(k + 1, l));
          myRemove(left->targets, intPairToRankFile(k + 1, l));
        } else if (right != nullptr && threatens(dynamic_cast<AbstractPiece*>(right), col.get()) && right->getName() == "whitepawn") {
          myRemove(col->threats, intPairToRankFile(k, l + 1));
          myRemove(right->availableMoves, intPairToRankFile(k + 1, l));
          myRemove(right->targets, intPairToRankFile(k + 1, l));
        }
      }
      l++;
    }
    k++;
  }
}

bool ChessBoard::isTarget(string piece) const {
  for (auto &n: board) {
    for (auto &m: n) {
      if (m.get() != nullptr && (in(m->targets, piece))) {
        return true;
      }
    }
  }
  return false;
}

bool ChessBoard::whiteRightCastleOpen() const {
  AbstractPiece* king = board[0][4].get();
  AbstractPiece* rook = board[0][7].get();
  if (!king || !rook) return false;
  if (king->getName() == "king" && rook->getName() == "rook" && board[0][5] == nullptr && board[0][6] == nullptr) {
    if (!isTarget(intPairToRankFile(0, 4)) && !isTarget(intPairToRankFile(0, 5)) && !isTarget(intPairToRankFile(0, 6))) {
      if (king->isCastleable() && rook->isCastleable()) {
        return true;
      }
    }
  }
  return false;
}

bool ChessBoard::whiteLeftCastleOpen() const {
  AbstractPiece* king = board[0][4].get();
  AbstractPiece* rook = board[0][0].get();
  if (!king || !rook) return false;
  if (king->getName() == "king" && rook->getName() == "rook" && board[0][1] == nullptr && board[0][2] == nullptr && board[0][3] == nullptr) {
    if (!isTarget(intPairToRankFile(0, 2)) && !isTarget(intPairToRankFile(0, 3)) && !isTarget(intPairToRankFile(0, 4))) {
      if (king->isCastleable() && rook->isCastleable()) {
        return true;
      }
    }
  }
  return false;
}

bool ChessBoard::blackRightCastleOpen() const {
  AbstractPiece* king = board[7][4].get();
  AbstractPiece* rook = board[7][7].get();
  if (!king || !rook) return false;
  if (king->getName() == "king" && rook->getName() == "rook" && board[7][5] == nullptr && board[7][6] == nullptr) {
    if (!isTarget(intPairToRankFile(7, 4)) && !isTarget(intPairToRankFile(7, 5)) && !isTarget(intPairToRankFile(7, 6))) {
      if (king->isCastleable() && rook->isCastleable()) {
        return true;
      }
    }
  }
  return false;
}

bool ChessBoard::blackLeftCastleOpen() const {
  AbstractPiece* king = board[7][4].get();
  AbstractPiece* rook = board[7][0].get();
  if (!king || !rook) return false;
  if (king->getName() == "king" && rook->getName() == "rook" && board[7][1] == nullptr && board[7][2] == nullptr && board[7][3] == nullptr) {
    if (!isTarget(intPairToRankFile(7, 2)) && !isTarget(intPairToRankFile(7, 3)) && !isTarget(intPairToRankFile(7, 4))) {
      if (king->isCastleable() && rook->isCastleable()) {
        return true;
      }
    }
  }
  return false;
}

void ChessBoard::addCastlingMoves() {
  if (whiteRightCastleOpen()) {
    board[0][4]->addAvailableMove(intPairToRankFile(0, 6));
    board[0][7]->addAvailableMove(intPairToRankFile(0, 5));
  }

  if (whiteLeftCastleOpen()) {
    board[0][0]->addAvailableMove(intPairToRankFile(0, 3));
    board[0][4]->addAvailableMove(intPairToRankFile(0, 2));
  }

  if (blackRightCastleOpen()) {
    board[7][4]->addAvailableMove(intPairToRankFile(7, 6));
    board[7][7]->addAvailableMove(intPairToRankFile(7, 5));
  }

  if (blackLeftCastleOpen()) {
    board[7][0]->addAvailableMove(intPairToRankFile(7, 3));
    board[7][4]->addAvailableMove(intPairToRankFile(7, 2));
  }
}

void ChessBoard::calculateAvailableMoves() {
  basicAddAvailableMoves();
  addThreats();
  addEnPassantMoves();
  addCastlingMoves();
}

ChessBoard::ChessBoard() {
  gd = make_unique<GraphicsDisplay>(this->getBoard()); 
  board.resize(8); // makes 8 rows
  for (auto &i: board) i.resize(8); // makes 8 columns, each entry is nullptr
  updatePieceLists(); // updates the lists of black and white pieces
}
ChessBoard::ChessBoard(const ChessBoard &other) {
  board.resize(8); // makes 8 rows
  for (auto &i: board) i.resize(8); // makes 8 columns, each entry is nullptr
  blackPieces = other.blackPieces;
  whitePieces = other.whitePieces;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (other.board[i][j]) {
        board[i][j] = other.board[i][j]->clone(); // makes a copy of each piece
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
  // iterates over the board, piece is white => add to whitePieces, same for black
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
  // checks if the king has any threats
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
  // check if the king is in check and has no legal moves
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
  // check if the king is not in check and has no legal moves
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
  // checks if both sides have insufficient material to checkmate
  // ie has a lone king, a king and a knight, or a king and a bishop
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

void ChessBoard::doUpdates() {
  dontCheckYourself();
  getOutOfCheck();
  updatePieceLists();
  updatePositions();
}

void printElements(vector<int> vec) {
  for (auto m: vec) {
    cout << m << endl;
  }
}

bool ChessBoard::isTerminalState() const {
  return (isCheckMate("white") || isCheckMate("black") || isStaleMate("white") || isStaleMate("black") || isInsufficientMaterial());
}

void ChessBoard::updatePositions() {
  // sets the position fields of each piece to be where they are on the board
  int r = 0;
  for (auto& n: board) {
    int c = 0;
    for (auto& m: n) {
      if (m.get()) { m->position = intPairToRankFile(r, c); }
      c++;
    }
    r++;
  }
}

bool ChessBoard::movePiece(string start, string end) {
  auto startCoords = rankFileToIntPair(start);
  auto endCoords = rankFileToIntPair(end);
  if (in(board[startCoords.first][startCoords.second]->availableMoves, end)) { // if end is in the available moves of the piece at start
    bool capturedDirectly = board[endCoords.first][endCoords.second].get() == nullptr ? false: true;
    AbstractPiece* startpiece = board[startCoords.first][startCoords.second].get();
    if (startpiece != nullptr && startpiece->getName() == "whitepawn") {
      if (endCoords.first - startCoords.first == 2) { // if the WhitePawn moved two times forward
        startpiece->setEnPassantable(true);
      } else if (endCoords.second != startCoords.second && !capturedDirectly) { // if the WhitePawn captured En Passant
        board[endCoords.first - 1][endCoords.second] = nullptr; // capture
      }
    } else if (startpiece != nullptr && startpiece->getName() == "blackpawn") {
      if (startCoords.first - endCoords.first == 2) { // if the BlackPawn moved two times forward
        startpiece->setEnPassantable(true);
      } else if (endCoords.second != startCoords.second && !capturedDirectly) { // if the BlackPawn captured En Passant
        board[endCoords.first + 1][endCoords.second] = nullptr; // capture
      }
    }
    board[endCoords.first][endCoords.second] = move(board[startCoords.first][startCoords.second]);
    board[startCoords.first][startCoords.second] = nullptr;

    if (startpiece->getName() == "king" && endCoords.second - startCoords.second ==  2) { // right Castle
      board[startCoords.first][startCoords.second + 1] = move(board[endCoords.first][endCoords.second + 1]);
      board[endCoords.first][endCoords.second + 1] = nullptr;
      board[startCoords.first][startCoords.second + 1]->setCastleable(false);
    }
    if (startpiece->getName() == "king" && startCoords.second - endCoords.second ==  2) { // left Castle
      board[startCoords.first][startCoords.second - 1] = move(board[endCoords.first][endCoords.second - 2]);
      board[endCoords.first][endCoords.second - 2] = nullptr;
      board[startCoords.first][startCoords.second - 1]->setCastleable(false);
    }
    if (startpiece->getName() == "king" || startpiece->getName() == "rook") { startpiece->setCastleable(false); }
    calculateAvailableMoves();
    updatePieceLists();
    updatePositions();
    return true;
  } else { return false; }
}

void ChessBoard::dontCheckYourself() {
  // iterates through every piece's available moves. A copy of the board is
  // made for each move, the move is then made on the copy. if the copy board is in check,
  // then remove the original move as it would result in putting yourself in check.
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
  // same logic as dontCheckYourself() but it first checks if the colour is in check.
  // If it is, then restrict each pieces moves to be only the moves that gets it
  // out of check
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
  throw runtime_error("trying to get pieces of neither black or white"); // should never happen, just for ease of debugging
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

int ChessBoard::getValue() const {
  int sum = 0;
  for (auto& wp: whitePieces) {
    sum += wp->getRank();
  }
  for (auto& bp: blackPieces) {
    sum -= bp->getRank();
  }
  return sum;
}
