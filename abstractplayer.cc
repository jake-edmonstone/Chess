#include <iostream>
#include <unistd.h>
#include "abstractplayer.h"
#include "random.h"

using namespace std;

template<typename T> bool in(const vector<T>& vec, const T& element) {
  return find(vec.begin(), vec.end(), element) != vec.end();
}

template<typename T> void print(const vector<T>& vec) {
  for (auto n: vec) { cout << n << " "; } cout << endl;
}

AbstractPlayer::AbstractPlayer(ChessBoard *cb, string colour): cb{cb}, colour{colour} {}
AbstractPlayer::~AbstractPlayer() {}
string AbstractPlayer::getColour() const {
  return colour;
}

Human::Human(ChessBoard *cb, string colour): AbstractPlayer{cb, colour} {}

pair<string, string> Human::getMove() const {
  string command;
  vector<string> whitePiecePositions;
  vector<string> blackPiecePositions;
  while (cin) {
    cin >> command;
    if (command == "move") {
      whitePiecePositions.clear();
      blackPiecePositions.clear();
      for (const auto &i : cb->whitePieces) {
        whitePiecePositions.emplace_back(i->getPosition());
      }
      for (const auto &i : cb->blackPieces) {
        blackPiecePositions.emplace_back(i->getPosition());
      }
      string start, end;
      cin >> start >> end;
      if (!in<string>(whitePiecePositions, start) && colour == "white") {
        cout << "That is not a white piece, but it's white's turn" << endl;
        continue;
      }
      else if (!in<string>(blackPiecePositions, start) && colour == "black") {
        cout << "That is not a black piece, but it's black's turn" << endl;
        continue;
      }
      else if (!in<string>(cb->getPiece(start)->getAvailableMoves(), end)) {
        cout << "That is not a legal move" << endl;
        continue;
      }
      return {start, end};
    }
    else if (command == "resign") {
      if (colour == "white") {
        cout << "white has resigned, black wins!" << endl;
        return {"resign",""};
      }
      else if (colour == "black") {
        cout << "black has resigned, white wins!" << endl;
        return {"resign",""};
      }
    }
  }
  throw runtime_error("not a legal move");
  return {"",""};
}
char Human::getPromotionDecision() const {
  char decision;
  while (cin) {
    cout << "What would you like to promote to?: ";
    cin >> decision;
    if (toupper(decision) == 'Q' || toupper(decision) == 'R' || toupper(decision) == 'N' || toupper(decision) == 'B') {
      return toupper(decision);
    }
    else {
      cout << "Please enter a valid piece" << endl;
      continue;
    }
  }
  throw runtime_error("bad promotion");
  return 1;
}
Computer1::Computer1(ChessBoard *cb, string colour): AbstractPlayer{cb, colour} {}

pair<string, string> Computer1::getMove() const {
  sleep(2);
  string start, end;
  vector<const AbstractPiece*> validPieces;
  if (this->getColour() == "white") {
    for (const auto &i : cb->whitePieces) {
      if (!i->getAvailableMoves().empty()) {
        validPieces.emplace_back(i);
      }
    }    
  } else if (this->getColour() == "black") {
    for (const auto &i : cb->blackPieces) {
      if (!i->getAvailableMoves().empty()) {
        validPieces.emplace_back(i);
      }
    }  
  }
  if (validPieces.empty()) throw runtime_error("no valid moves");

  const AbstractPiece* piece = getRandomElement<const AbstractPiece*>(validPieces);
  start = piece->getPosition();
  end = getRandomElement<string>(piece->getAvailableMoves());
  return {start, end};
}
char Computer1::getPromotionDecision() const {
  vector<char> pieces {'Q', 'R', 'N', 'B'};
  return getRandomElement<char>(pieces);
}

