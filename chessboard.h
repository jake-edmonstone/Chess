#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <memory>
#include <string>
#include "abstractpiece.h"

class ChessBoard {
  // A 2D vector of unique_ptrs to pieces
  std::vector<std::vector<std::unique_ptr<AbstractPiece>>> board;
  // Vectors of raw pointers to pieces
  std::vector<AbstractPiece*> whitePieces;
  std::vector<AbstractPiece*> blackPieces;
  // Checks if a pair (r,c) is within board limits
  bool inrange(int r, int c);
  // swicthes between eg. e4 -> 3,4
  std::pair<int,int> rankFileToIntPair(std::string rf);
  // inverse of previous
  std::string intPairToRankFile(int row, int col);
  // update list of owned pieces
  void updatePieceLists();
  // called by calculateAvailableMoves to remove the possibility of putting yourself in check
  void dontCheckYourself();
  // called by calculate moves to make sure you get out of check if you are in it
  void getOutOfCheck();
 public:
  // ChessBoard ctor, takes a string to signify how it should be set up: "default" means official chess "empty" means empty
  ChessBoard(std::string config);
  // determines whether or not a Piece is blocked by another Piece from moving by (xshift, yshift)
  bool isBlocked(int firstpiecey, int firstpiecex, int yshift, int xshift);
  // ChessBoard copy ctor (deep copy)
  ChessBoard(const ChessBoard &other);
  // updates each pieces fields (threats, targets, availaible moves)
  void calculateAvailableMoves();
  bool isCheck(std::string colour);
  bool isCheckMate(std::string colour);
  void movePiece(std::string start, std::string end);
  friend std::ostream &operator<<(std::ostream &out, const ChessBoard &board);
};

std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard);

#endif