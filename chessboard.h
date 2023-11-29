#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <memory>
#include <string>
#include "abstractpiece.h"

class ChessBoard {
  std::vector<std::vector<std::unique_ptr<AbstractPiece>>> board;
 public:
  void calculateAvailableMoves();
  bool isCheck(std::string colour);
  bool isCheckMate(std::string colour);
  void move(std::pair<int,int> start, std::pair<int,int> end);
  friend std::ostream &operator<<(std::ostream &out, const ChessBoard &board);
};

std::ostream &operator<<(std::ostream &out, const ChessBoard &board);

#endif