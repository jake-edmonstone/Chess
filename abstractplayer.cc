#include <iostream>
#include <unistd.h>
#include "abstractplayer.h"
#include "random.h"

using namespace std;

AbstractPlayer::AbstractPlayer(ChessBoard *cb, string colour): cb{cb}, colour{colour} {}
AbstractPlayer::~AbstractPlayer() {}
string AbstractPlayer::getColour() const {
  return colour;
}

Human::Human(ChessBoard *cb, string colour): AbstractPlayer{cb, colour} {}
pair<string, string> Human::getMove() const {
  string start, end;
  cin >> start >> end;
  return {start, end};
}

Computer1::Computer1(ChessBoard *cb, string colour): AbstractPlayer{cb, colour} {}
pair<string, string> Computer1::getMove() const {
  sleep(2);
  string start, end;
  const AbstractPiece* piece;
  if (this->getColour() == "white") {
    piece = getRandomElement<const AbstractPiece*>(cb->whitePieces);
  } else if (this->getColour() == "black") {
    piece = getRandomElement<const AbstractPiece*>(cb->blackPieces);
  }
  start = piece->getPosition();
  end = getRandomElement<string>(piece->getAvailableMoves());
  return {start, end};
}

