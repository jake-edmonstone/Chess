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
  virtual ~AbstractPiece();
  virtual std::vector<Vec> getPotentialMoves() = 0;
};

#endif
