#ifndef ABSTRACTPIECE_H
#define ABSTRACTPIECE_H

#  define PAWN_VALUE 100
#  define KNIGHT_VALUE 325
#  define BISHOP_VALUE 325
#  define ROOK_VALUE 500
#  define QUEEN_VALUE 1050
#  define KING_VALUE  40000

#include <string>
#include <vector>
#include <memory>
#include "vec.h"

class AbstractPiece {
  // vectors of strings storing a pieces available moves, which squares it is targeting, and which squares are threatening it
  std::vector<std::string> availableMoves;
  std::vector<std::string> targets;
  std::vector<std::string> threats;
  std::string colour;
  std::string position;
  // used for the board copy constructor
  virtual std::unique_ptr<AbstractPiece> clone() const = 0;
  // add elements to the corresponding vectors
  void addAvailableMove(std::string move);
  void addTarget(std::string target);
  void addThreat(std::string threat);
  // Checkers for special move validity
  virtual bool isCastleable() const;
  virtual void setCastleable(bool value);
  virtual bool isEnPassantable() const;
  virtual void setEnPassantable(bool value);
 public:
  AbstractPiece(std::string colour, std::string position);
  virtual ~AbstractPiece() = 0;
  // returns the set of all potential moves a piece can make by virtue of its type
  // each potential move is specified by a vector in R^2 indicating the number of squares it can move
  // in the x-direction followed by the number of squares it can move in the y-direction
  virtual std::vector<Vec> getPotentialMoves() const = 0;
  // returns the name of the Chess piece. For example, "king"
  virtual std::string getName() const = 0;
  // getters
  std::string getColour() const; 
  std::string getPosition() const;
  const std::vector<std::string> &getTargets() const;
  const std::vector<std::string> &getThreats() const;
  const std::vector<std::string> &getAvailableMoves() const;
  virtual int getRank() const = 0;
  friend class ChessBoard;
};

class King: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
  bool Castleable = true;
  bool isCastleable() const override;
  void setCastleable(bool value) override;
 public:
  King(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};
class Queen: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Queen(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};
class Rook: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
  bool Castleable = true;
  bool isCastleable() const override;
  void setCastleable(bool value) override;
 public:
  Rook(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};
class Knight: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Knight(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};
class Bishop: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
 public:
  Bishop(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};
class WhitePawn: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
  bool enPassantable = false;
  bool isEnPassantable() const override;
  void setEnPassantable(bool value) override;
 public:
  WhitePawn(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};
class BlackPawn: public AbstractPiece {
  std::unique_ptr<AbstractPiece> clone() const override;
  bool enPassantable = false;
  bool isEnPassantable() const override;
  void setEnPassantable(bool value) override;
 public:
  BlackPawn(std::string colour, std::string position);
  std::vector<Vec> getPotentialMoves() const override;
  std::string getName() const override;
  int getRank() const override;
};

bool threatens(const AbstractPiece* threat, const AbstractPiece* victim);

#endif
