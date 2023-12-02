#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include "chessboard.h"

class AbstractPlayer {
  const ChessBoard *cb;
  std::string colour;
 public:
  AbstractPlayer(ChessBoard *cb, std::string colour);
  virtual ~AbstractPlayer() = 0;
  virtual std::pair<std::string, std::string>getMove() const = 0;
  std::string getColour() const;
};

class Human: public AbstractPlayer {
 public:
  Human(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove() const override;
};

class Computer1: public AbstractPlayer {
 public:
  Computer1(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove() const override;
};

#endif
