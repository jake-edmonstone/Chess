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
  string getColour(); 
};

class King: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Queen: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Rook: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Knight: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Bishop: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class WhitePawn: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class BlackPawn: public AbstractPiece {
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};

#endif
