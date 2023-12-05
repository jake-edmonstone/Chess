#include <iostream>
#include <unistd.h>
#include "abstractplayer.h"
#include <cmath>
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

pair<string, string> Human::getMove(string config) const {
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

pair<string, string> Computer1::getMove(string config) const {
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
  if (config == "sleep") { sleep(2); }
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

// true means it's black's turn!
void Computer4::findPossibleMoves(State& state, int depth, bool turn) const {
  if (depth == 0) { return; }
  if (turn) {
    for (auto& piece : state.cb.blackPieces) {
      for (auto& move : piece->availableMoves) {
        shared_ptr<ChessBoard> temp = make_shared<ChessBoard>(state.cb);
        temp->movePiece(piece->getPosition(), move);
        shared_ptr<State> nextstate = make_shared<State>(*temp, pair(piece->getPosition(), move));
        findPossibleMoves(*nextstate.get(), depth - 1, !turn); // finds possible moves for the next state
        state.nextStates.emplace_back(nextstate);
      }
    }
  } else {
    for (auto& piece : state.cb.whitePieces) {
      for (auto& move : piece->availableMoves) {
        shared_ptr<ChessBoard> temp = make_shared<ChessBoard>(state.cb);
        temp->movePiece(piece->getPosition(), move);
        shared_ptr<State> nextstate = make_shared<State>(*temp, pair(piece->getPosition(), move));
        findPossibleMoves(*nextstate.get(), depth - 1, !turn); // finds possible moves for the next state
        state.nextStates.emplace_back(nextstate);
      }
    }
  }
}

int Computer4::getPeakValueDownPath(const State* state, string bound) const {
  int peak = state->cb.getValue();
  if (state->nextStates.empty()) { return peak; }
  else if (bound == "max") {
    for (auto& state : state->nextStates) {
      int val = getPeakValueDownPath(state.get(), bound);
      if (val > peak) {
        peak = val;
      }
    }
  } else if (bound == "min") {
    for (auto& state : state->nextStates) {
      int val = getPeakValueDownPath(state.get(), bound);
      if (val < peak) {
        peak = val;
      }
    }
  }
  return peak;
}

std::pair<std::string, std::string> Computer4::getOptimalMove(State& root, int peakValSoFar) const {
  pair<string, string> optimalMove;

  if (this->getColour() == "white") {
    for (auto nextstate : root.nextStates) {
      string bound = "max";
      if (getPeakValueDownPath(nextstate.get(), bound) > peakValSoFar) {
        optimalMove = nextstate->move;
      }
    }
  }

  if (this->getColour() == "black") {
    for (auto nextstate : root.nextStates) {
      string bound = "min";
      if (getPeakValueDownPath(nextstate.get(), bound) < peakValSoFar) {
        optimalMove = nextstate->move;
      }
    }
  }
  return optimalMove;
}


int Computer4::minimax(const ChessBoard* bp, int depth, bool isblack) const {
  if (depth == 0 || bp->isTerminalState()) { return bp->getValue(); }
  int value = 0;
  if (!isblack) {
    value = -1000000;
    for (auto piece : bp->whitePieces) {
      for (const auto &move : piece->availableMoves) {
        ChessBoard temp{*bp};
        temp.movePiece(piece->getPosition(), move);
        if (temp.pawnOnFirstOrLastRank()) {
          temp.placePiece('Q', move);
          temp.calculateAvailableMoves();
          temp.dontCheckYourself();
          temp.getOutOfCheck();
          temp.updatePieceLists();
          temp.updatePositions();
        }
        value = max(value, minimax(&temp, depth - 1, true));
      }
    }
  } else {
    value = 1000000;
    for (auto piece : bp->blackPieces) {
      for (const auto &move : piece->availableMoves) {
        ChessBoard temp{*bp};
        temp.movePiece(piece->getPosition(), move);
        if (temp.pawnOnFirstOrLastRank()) {
          temp.placePiece('q', move);
          temp.calculateAvailableMoves();
          temp.dontCheckYourself();
          temp.getOutOfCheck();
          temp.updatePieceLists();
          temp.updatePositions();
        }
        value = min(value, minimax(&temp, depth - 1, false));
      }
    }
  }
  return value;
} 



Computer4::Computer4(ChessBoard *cb, std::string colour): AbstractPlayer{cb, colour} {}

std::pair<std::string, std::string> Computer4::getMove(string config) const {
  pair<string, string> optimalMoveSoFar;
  int value;
  if (this->colour == "white") {
    value = -1000000;
    for (const auto piece : cb->whitePieces) {
      for (auto move : piece->availableMoves) {
        ChessBoard temp{*cb};
        temp.movePiece(piece->getPosition(), move);
        int newValue = minimax(&temp, 3, true);
        if (newValue > value) {
          value = newValue;
          optimalMoveSoFar = pair<string, string>(piece->getPosition(), move);
        }
      }
    }
  } else if (this->colour == "black") {
    value = 1000000;
    for (auto piece: cb->blackPieces) {
      for (auto move : piece->availableMoves) {
        ChessBoard temp{*cb};
        temp.movePiece(piece->getPosition(), move);
        int newValue = minimax(&temp, 3, false);
        if (newValue < value) {
          value = newValue;
          optimalMoveSoFar = pair<string, string>(piece->getPosition(), move);
        }
      }
    }
  }
  
  if (optimalMoveSoFar.first.empty() || optimalMoveSoFar.second.empty()) { 
    throw runtime_error("piece colour is incorrect"); 
  } 
  return optimalMoveSoFar;
}

char Computer4::getPromotionDecision() const {
  return 'Q';
}
