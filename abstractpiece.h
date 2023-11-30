#ifndef ABSTRACTPIECE_H
#define ABSTRACTPIECE_H

#include <string>
#include <vector>
#include <memory>
#include "vec.h"

class AbstractPiece {
  std::vector<std::string> availableMoves;
  std::vector<std::string> targets;
  std::vector<std::string> threats;
  std::string colour;
  virtual std::unique_ptr<AbstractPiece> clone() const = 0;
 public:
  AbstractPiece(std::string colour);
  virtual ~AbstractPiece() = 0;
  virtual std::vector<Vec> getPotentialMoves() const = 0;
  virtual std::string getName() const = 0;
  friend class ChessBoard;
  std::string getColour(); 
};

class King: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  King(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Queen: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Queen(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Rook: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Rook(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Knight: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Knight(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Bishop: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Bishop(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class WhitePawn: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  WhitePawn(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class BlackPawn: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  BlackPawn(std::string colour);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};

#endif
