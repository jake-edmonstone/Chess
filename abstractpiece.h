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
  std::string position;
  virtual std::unique_ptr<AbstractPiece> clone() const = 0;
 public:
  AbstractPiece(std::string colour, std::string position);
  virtual ~AbstractPiece() = 0;

  // returns the set of all potential moves a piece can make by virtue of its type
  // each potential move is specified by a vector in R^2 indicating the number of squares it can move
  // in the x-direction followed by the number of squares it can move in the y-direction
  virtual std::vector<Vec> getPotentialMoves() const = 0;
  // returns the name of the Chess piece. For example, "king"
  virtual std::string getName() const = 0;
  // add elements to the corresponding vectors
  void addAvailableMove(std::string move);
  void addTarget(std::string target);
  void addThreat(std::string threat);
  // obtains the colour of the piece
  std::string getColour() const; 
  std::string getPosition() const;
  const std::vector<std::string> &getAvailableMoves() const;
  friend class ChessBoard;
};

class King: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  King(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Queen: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Queen(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Rook: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Rook(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Knight: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Knight(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class Bishop: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Bishop(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class WhitePawn: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  WhitePawn(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};
class BlackPawn: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  BlackPawn(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
};

#endif
