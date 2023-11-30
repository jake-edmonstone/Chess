#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <memory>
#include <string>
#include "abstractpiece.h"

class ChessBoard {
  // A 2D vector of unique_ptrs to pieces
  std::vector<std::vector<std::unique_ptr<AbstractPiece>>> board;
  // Checks if a pair (r,c) is within board limits
  bool inrange(int r, int c);
  int rankFileToIntPair(std::string rf);
  std::string intPairToRankFile(int row, int col);
 public:
  ChessBoard(std::string config);
  void calculateAvailableMoves();
  bool isCheck(std::string colour);
  bool isCheckMate(std::string colour);
  void move(std::string start, std::string end);
  friend std::ostream &operator<<(std::ostream &out, const ChessBoard &board);
};

std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard);

#endif