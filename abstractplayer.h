#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include "chessboard.h"

class AbstractPlayer {
 protected:
  const ChessBoard *cb; // pointer to const ChessBoard. Each player can "observe" the board but not change it
  std::string colour;   // colour ("black" or "white") 
 public:
  AbstractPlayer(ChessBoard *cb, std::string colour); // ctor
  virtual ~AbstractPlayer() = 0; // virtual destructor
   // this is the main functionality of the players, returns a pair of strings that describes the move.
   // this is what allows for the polymorphism of players (computer vs human, computer vs computer etc.)
  virtual std::pair<std::string, std::string>getMove() const = 0;
  std::string getColour() const; // getter
  virtual char getPromotionDecision() const = 0; // for when a player can promote a pawn
};

class Human: public AbstractPlayer {
 public:
  Human(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove() const override;
  char getPromotionDecision() const override;
};

class Computer1: public AbstractPlayer {
 public:
  Computer1(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove() const override;
  char getPromotionDecision() const override;
};

#endif
