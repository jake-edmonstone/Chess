#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <memory>
#include <string>
#include "abstractpiece.h"

class ChessBoard {
  std::vector<std::vector<std::unique_ptr<AbstractPiece>>> board;
  bool inrange(int r, int c);
 public:
  ChessBoard(std::string config);
  void calculateAvailableMoves();
  bool isCheck(std::string colour);
  bool isCheckMate(std::string colour);
  void move(std::string start, std::string end);
  friend std::ostream &operator<<(std::ostream &out, const ChessBoard &board);
};

std::ostream &operator<<(std::ostream &out, const ChessBoard &board);

#endif