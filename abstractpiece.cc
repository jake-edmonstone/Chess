#include "abstractpiece.h"
#include <memory>
using namespace std;

template<typename T> bool in(const vector<T>& vec, const T& element) {
  return find(vec.begin(), vec.end(), element) != vec.end();
}

AbstractPiece::AbstractPiece(string colour, string position): colour{colour}, position{position} {}

AbstractPiece::~AbstractPiece() {}

string AbstractPiece::getColour() const { return colour; }

string AbstractPiece::getPosition() const { return position; };

const vector<string> &AbstractPiece::getAvailableMoves() const { return availableMoves; }

void AbstractPiece::addAvailableMove(string move) { availableMoves.emplace_back(move); }

void AbstractPiece::addTarget(string target) { targets.emplace_back(target); }

void AbstractPiece::addThreat(string threat) { threats.emplace_back(threat); }

const std::vector<std::string> &AbstractPiece::getTargets() const { return targets; }

bool threatens(const AbstractPiece* threat, const AbstractPiece* victim) {
  if (threat != nullptr && victim != nullptr) {
    if (in(threat->getTargets(), victim->getName())) { return true; }
  }
  return false;
}

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

King::King(string colour, string position): AbstractPiece{colour, position} {}
Queen::Queen(string colour, string position): AbstractPiece{colour, position} {}
Rook::Rook(string colour, string position): AbstractPiece{colour, position} {}
Knight::Knight(string colour, string position): AbstractPiece{colour, position} {}
Bishop::Bishop(string colour, string position): AbstractPiece{colour, position} {}
BlackPawn::BlackPawn(string colour, string position): AbstractPiece{colour, position} {}
WhitePawn::WhitePawn(string colour, string position): AbstractPiece{colour, position} {}

unique_ptr<AbstractPiece> King::clone() const {
  return std::make_unique<King>(*this);
}
unique_ptr<AbstractPiece> Queen::clone() const {
  return std::make_unique<Queen>(*this);
}
unique_ptr<AbstractPiece> Rook::clone() const {
  return std::make_unique<Rook>(*this);
}
unique_ptr<AbstractPiece> Knight::clone() const {
  return std::make_unique<Knight>(*this);
}
unique_ptr<AbstractPiece> Bishop::clone() const {
  return std::make_unique<Bishop>(*this);
}
unique_ptr<AbstractPiece> BlackPawn::clone() const {
  return std::make_unique<BlackPawn>(*this);
}
unique_ptr<AbstractPiece> WhitePawn::clone() const {
  return std::make_unique<WhitePawn>(*this);
}

int King::getRank() const { return KING_VALUE; }

int Queen::getRank() const { return QUEEN_VALUE; }

int Rook::getRank() const { return ROOK_VALUE; }

int Knight::getRank() const { return KNIGHT_VALUE; }

int Bishop::getRank() const { return BISHOP_VALUE; }

int WhitePawn::getRank() const { return PAWN_VALUE; }

int BlackPawn::getRank() const { return PAWN_VALUE; }


bool AbstractPiece::isCastleable() const { return false; }

void AbstractPiece::setCastleable(bool value) { ; }

bool King::isCastleable() const { return Castleable; }

void King::setCastleable(bool value) { Castleable = value; }

bool Rook::isCastleable() const { return Castleable; }

void Rook::setCastleable(bool value) { Castleable = value; }

bool AbstractPiece::isEnPassantable() const { return false; }

void AbstractPiece::setEnPassantable(bool value) { ; }

bool WhitePawn::isEnPassantable() const { return enPassantable; }

void WhitePawn::setEnPassantable(bool value) { enPassantable = value; } 

bool BlackPawn::isEnPassantable() const { return enPassantable; }

void BlackPawn::setEnPassantable(bool value) { enPassantable = value; }
