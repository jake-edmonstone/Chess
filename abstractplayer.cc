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

vector<std::pair<string, string>> AbstractPlayer::getMovesCheck() const {
  string start, end;
  vector<pair<string, string>> possibleChecks;

  if (this->getColour() == "white") {
    for (const auto &piece : cb->whitePieces) {
      start = piece->getPosition();
      for (const auto &move : piece->getAvailableMoves()) {
        ChessBoard temp{*cb};
        temp.movePiece(start, move);
        if (temp.isCheck("black") && !temp.isCheckMate("black")) {
          possibleChecks.emplace_back(pair<string, string>(start, move));
        }
      }
    }
  } else if (this->getColour() == "black") {
    for (const auto &piece: cb->blackPieces) {
      start = piece->getPosition();
      for (const auto &move : piece->getAvailableMoves()) {
        ChessBoard temp{*cb};
        temp.movePiece(start, move);
        if (temp.isCheck("white") && !temp.isCheckMate("white")) {
          possibleChecks.emplace_back(pair<string, string>(start, move));
        }
      }
    }
  }
  return possibleChecks;
}

vector<std::pair<string, string>> AbstractPlayer::getMovesCheckMate() const {
  string start, end;
  vector<pair<string, string>> possibleCheckMates;

  if (this->getColour() == "white") {
    for (const auto &piece : cb->whitePieces) {
      start = piece->getPosition();
      for (const auto &move : piece->getAvailableMoves()) {
        ChessBoard temp{*cb};
        temp.movePiece(start, move);
        if (temp.isCheckMate("black")) {
          possibleCheckMates.emplace_back(pair<string, string>(start, move));
        }
      }
    }
  } else if (this->getColour() == "black") {
    for (const auto &piece: cb->blackPieces) {
      start = piece->getPosition();
      for (const auto &move : piece->getAvailableMoves()) {
        ChessBoard temp{*cb};
        temp.movePiece(start, move);
        if (temp.isCheckMate("white")) {
          possibleCheckMates.emplace_back(pair<string, string>(start, move));
        }
      }
    }
  }
  return possibleCheckMates;
}

vector<std::pair<string, string>> AbstractPlayer::getMovesCapture() const {
  string start, end;
  vector<pair<string, string>> possibleCaptures;

  if (this->getColour() == "white") {
    for (const auto &piece : cb->whitePieces) {
      start = piece->getPosition();
      for (const auto &move : piece->targets) {
        possibleCaptures.emplace_back(pair<string, string>(start, move));
      }
    }
  } else if (this->getColour() == "black") {
    for (const auto &piece: cb->blackPieces) {
      start = piece->getPosition();
      for (const auto &move : piece->targets) {
        possibleCaptures.emplace_back(pair<string, string>(start, move));
      }
    }
  }
  return possibleCaptures;
}

std::vector<std::pair<std::string, std::string>> AbstractPlayer::getMovesAvoidThreat() const {
  string start, end;
  vector<pair<string, string>> threatAvoidantMoves;

  if (this->getColour() == "white") {
    for (const auto &piece : cb->whitePieces) {
      start = piece->getPosition();
      if (piece->threats.size() > 0) {
        for (auto& move : piece->availableMoves) {
          ChessBoard temp{*cb};
          temp.movePiece(start, move);
          pair<int, int> mv = temp.rankFileToIntPair(move);
          if (temp.board[mv.first][mv.second]->threats.empty()) {
            threatAvoidantMoves.emplace_back(start, move);
          }
        }
      }
    }
  } else if (this->getColour() == "black") {
    for (const auto &piece : cb->blackPieces) {
      start = piece->getPosition();
      if (piece->threats.size() > 0) {
        for (auto& move : piece->availableMoves) {
          ChessBoard temp{*cb};
          temp.movePiece(start, move);
          pair<int, int> mv = temp.rankFileToIntPair(move);
          if (temp.board[mv.first][mv.second]->threats.empty()) {
            threatAvoidantMoves.emplace_back(start, move);
          }
        }
      }
    }
  }
  cout << threatAvoidantMoves.size() << endl;
  return threatAvoidantMoves;
}

pair<string, string> AbstractPlayer::getRandomMove() const {
  vector<const AbstractPiece*> moves;

  if (this->getColour() == "white") {
    for (auto &p: cb->whitePieces) {
      if (!p->availableMoves.empty()) { moves.emplace_back(p); }
    }
  }

  if (this->getColour() == "black") {
    for (auto &p: cb->blackPieces) {
      if (!p->availableMoves.empty()) { moves.emplace_back(p); }
    }
  }

  const AbstractPiece* piece = getRandomElement<const AbstractPiece*>(moves);
  return pair<string, string>(piece->getPosition(), getRandomElement(piece->availableMoves));
}

Computer2::Computer2(ChessBoard *cb, std::string colour): AbstractPlayer{cb, colour} {}

std::pair<std::string, std::string> Computer2::getMove(string config) const {
  sleep(2);
  string start, end;
  
  vector<pair<string, string>>checkMates = getMovesCheckMate();
  if (!checkMates.empty()) { return getRandomElement(checkMates); }

  vector<pair<string, string>>checks = getMovesCheck();
  if (!checks.empty()) { return getRandomElement(checks); }

  vector<pair<string, string>>captures = getMovesCapture();
  if (!captures.empty()) { return getRandomElement(captures);}

  return getRandomMove();
}

char Computer2::getPromotionDecision() const {
  vector<char> pieces {'Q', 'N'};
  return getRandomElement<char>(pieces);
}

Computer3::Computer3(ChessBoard *cb, std::string colour): AbstractPlayer{cb, colour} {}

std::pair<std::string, std::string> Computer3::getMove(string config) const {
  sleep(2);
  string start, end;

  vector<pair<string, string>> checkMates = getMovesCheckMate();
  if (!checkMates.empty()) { return getRandomElement(checkMates); }

  vector<pair<string, string>> movesAvoidThreat = getMovesAvoidThreat();
  if (!movesAvoidThreat.empty()) { return getRandomElement(movesAvoidThreat); }

  vector<pair<string, string>> checks = getMovesCheck();
  if (!checks.empty()) { return getRandomElement(checks); }

  vector<pair<string, string>> captures = getMovesCapture();
  if (!captures.empty()) { return getRandomElement(captures);}

  return getRandomMove();
}

char Computer3::getPromotionDecision() const {
  return 'Q';
}
