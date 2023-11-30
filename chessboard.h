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
  // swicthes between eg. e4 -> 3,4
  int rankFileToIntPair(std::string rf);
  // inverse of previous
  std::string intPairToRankFile(int row, int col);
 public:
  // ChessBoard ctor, takes a string to signify how it should be set up: "default" means official chess "empty" means empty
  ChessBoard(std::string config);
  // ChessBoard copy ctor (deep copy)
  ChessBoard(const ChessBoard &other);
  // updates each pieces fields (threats, targets, availaible moves)
  void calculateAvailableMoves();
  bool isCheck(std::string colour);
  bool isCheckMate(std::string colour);
  void move(std::string start, std::string end);
  friend std::ostream &operator<<(std::ostream &out, const ChessBoard &board);
};

std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard);

#endif