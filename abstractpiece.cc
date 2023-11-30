#include "abstractpiece.h"
using namespace std;

AbstractPiece::AbstractPiece(string colour): colour{colour} {}

AbstractPiece::~AbstractPiece() {}

string AbstractPiece::getColour() {return colour;}

std::vector<Vec> King::getPotentialMoves() const {
  return {{1,0}, {1,1}, {0,1}, {-1, 1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};
}
std::vector<Vec> Queen::getPotentialMoves() const {
  return {{ 1, 0}, { 2, 0}, { 3, 0}, { 4, 0}, { 5, 0}, { 6, 0}, { 7, 0},
          {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
          { 0, 1}, { 0, 2}, { 0, 3}, { 0, 4}, { 0, 5}, { 0, 6}, { 0, 7},
          { 0,-1}, { 0,-2}, { 0,-3}, { 0,-4}, { 0,-5}, { 0,-6}, { 0,-7},
          { 1, 1}, { 2, 2}, { 3, 3}, { 4, 4}, { 5, 5}, { 6, 6}, { 7, 7},
          {-1,-1}, {-2,-2}, {-3,-3}, {-4,-4}, {-5,-5}, {-6,-6}, {-7,-7},
          { 1,-1}, { 2,-2}, { 3,-3}, { 4,-4}, { 5,-5}, { 6,-6}, { 7,-7},
          {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}};
}
std::vector<Vec> Rook::getPotentialMoves() const {
  return {{ 1, 0}, { 2, 0}, { 3, 0}, { 4, 0}, { 5, 0}, { 6, 0}, { 7, 0},
          {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
          { 0, 1}, { 0, 2}, { 0, 3}, { 0, 4}, { 0, 5}, { 0, 6}, { 0, 7},
          { 0,-1}, { 0,-2}, { 0,-3}, { 0,-4}, { 0,-5}, { 0,-6}, { 0,-7}};
}
std::vector<Vec> Knight::getPotentialMoves() const {
  return {{2,1}, {2,-1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {1,-2}, {-1,-2}};
}
std::vector<Vec> Bishop::getPotentialMoves() const {
  return {{ 1, 1}, { 2, 2}, { 3, 3}, { 4, 4}, { 5, 5}, { 6, 6}, { 7, 7},
          {-1,-1}, {-2,-2}, {-3,-3}, {-4,-4}, {-5,-5}, {-6,-6}, {-7,-7},
          { 1,-1}, { 2,-2}, { 3,-3}, { 4,-4}, { 5,-5}, { 6,-6}, { 7,-7},
          {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}};
}
std::vector<Vec> WhitePawn::getPotentialMoves() const {
  return {{0,1}, {0,2}, {1,1}, {-1,1}};
}
std::vector<Vec> BlackPawn::getPotentialMoves() const {
  return {{0,-1}, {0,-2}, {1,-1}, {-1,-1}};
}
string King::getName() const {
  return "king";
}
string Queen::getName() const {
  return "queen";
}
string Rook::getName() const {
  return "rook";
}
string Knight::getName() const {
  return "knight";
}
string Bishop::getName() const {
  return "bishop";
}
string WhitePawn::getName() const {
  return "whitepawn";
}
string BlackPawn::getName() const {
  return "blackpawn";
}

King::King(string colour): AbstractPiece{colour} {}
Queen::Queen(string colour): AbstractPiece{colour} {}
Rook::Rook(string colour): AbstractPiece{colour} {}
Knight::Knight(string colour): AbstractPiece{colour} {}
Bishop::Bishop(string colour): AbstractPiece{colour} {}
BlackPawn::BlackPawn(string colour): AbstractPiece{colour} {}
WhitePawn::WhitePawn(string colour): AbstractPiece{colour} {}