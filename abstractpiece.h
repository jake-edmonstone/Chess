#ifndef ABSTRACTPIECE_H
#define ABSTRACTPIECE_H

#include <string>
#include <vector>
#include "vec.h"

class AbstractPiece {
  std::vector<std::string> availableMoves;
  std::vector<std::string> targets;
  std::vector<std::string> threats;
  std::string colour;
 public:
  AbstractPiece(std::string colour);
  virtual ~AbstractPiece() = 0;
  virtual std::vector<Vec> getPotentialMoves() const = 0;
  virtual std::string getName() const = 0;
  friend class ChessBoard;
  std::string getColour(); 
};

class King: public AbstractPiece {
 public:
  King(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Queen: public AbstractPiece {
 public:
  Queen(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Rook: public AbstractPiece {
 public:
  Rook(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Knight: public AbstractPiece {
 public:
  Knight(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Bishop: public AbstractPiece {
 public:
  Bishop(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class WhitePawn: public AbstractPiece {
 public:
  WhitePawn(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class BlackPawn: public AbstractPiece {
 public:
  BlackPawn(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};

#endif
