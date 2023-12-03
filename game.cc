#include <iostream>
#include "game.h"
#include "abstractplayer.h"

using namespace std;


Game::Game() {
  cb = make_unique<ChessBoard>();
}

unique_ptr<AbstractPlayer> Game::makePlayer(string playerType, string colour) {
  if (playerType == "human") {
    return make_unique<Human>(cb.get(), colour);
  } else if (playerType == "computer1") {
    return make_unique<Computer1>(cb.get(), colour);
  } 
  // more computers
  else {
    throw invalid_argument("not a recognized player type");
  }
}

void Game::initialize(float &whiteScore, float &blackScore) {
  string command;
  while (cin) {
    cin >> command;
    if (command == "setup") {
      isSetup = true;
      setup();
    }
    if (command == "game") {
      string whitePlayer, blackPlayer;
      if (!(cin >> whitePlayer >> blackPlayer)) {
        break;
      }
      p1 = makePlayer(whitePlayer, "white");
      p2 = makePlayer(blackPlayer, "black");
      play1Game(whiteScore, blackScore);
    }
  }
}
void Game::setup() {
  cb->clear();
  cout << *cb;
  string command;
  while (cin) {
    cin >> command;
    if (!cin) break;
    if (command == "+") {
      char piece;
      string position;
      cin >> piece >> position;
      if (piece == 'p' || piece == 'P') {
        if (position[1] == '1' || position[1] == '8') {
          cout << "You cannot have pawns on the first or last rank" << endl;
          continue;
        }
      }
      cb->placePiece(piece, position);
    } else if (command == "-") {
      string position;
      cin >> position;
      cb->removePiece(position);
    } else if (command == "=") {
      string colour;
      cin >> colour;
      if (colour == "black") whitesTurn = false;
      if (colour == "white") whitesTurn = true;
    } else if (command == "done") {
      int blackKingCount = 0;
      int whiteKingCount = 0;
      for (const auto &i : cb->board) {
        for (const auto &j : i) {
          if (j) {
            if (j->getName() == "king") {
              if (j->getColour() == "white") ++whiteKingCount;
              else if (j->getColour() == "black") ++blackKingCount;
            }
          }
        }
      }
      if (blackKingCount != 1 || whiteKingCount != 1) {
        cout << "You need exactly one white king and one black king" << endl;
        continue;
      }
      cb->calculateAvailableMoves();
      cb->dontCheckYourself();
      cb->getOutOfCheck();
      if (cb->isCheck("white") || cb->isCheck("black")) {
        cout << "Neither colour can be in check" << endl;
        continue;
      }
      isSetup = true;
      break;
    }
    cout << *cb;
  }
}

void Game::play1Game(float &whiteScore, float &blackScore) {
  if (!isSetup) {
    cb->clear();
    cb->defaultSetup();
  }
  cb->dontCheckYourself();
  cb->getOutOfCheck();
  cb->updatePieceLists();
  cout << *cb;
  while (cin) {
    string start, end;
    if (whitesTurn) {
      while (true) {
        pair<string, string> move = p1->getMove();
        start = move.first;
        if (start == "resign") {
          ++blackScore;
          return;
        }
        end = move.second;
        if (cb->movePiece(start, end)) break;
      }
    }
    else if (!whitesTurn) {
      while (true) {
        pair<string, string> move = p2->getMove();
        start = move.first;
        if (start == "resign") {
          ++whiteScore;
          return;
        }
        end = move.second;
        if (cb->movePiece(start, end)) break;
      }
    }
    cb->dontCheckYourself();
    cb->getOutOfCheck();
    whitesTurn = !whitesTurn;
    cout << *cb;


    string turn = whitesTurn ? "white" : "black";
    string notTurn = whitesTurn ? "black" : "white";
    if (cb->isCheckMate(turn)) {
      cout << "Checkmate! " << notTurn << " Wins!" << endl;
      if (notTurn == "white") ++whiteScore;
      else ++blackScore;
    }
    else if (cb->isStaleMate(turn)) {
      cout << "Stalemate!" << endl;
      whiteScore += 0.5;
      blackScore += 0.5;
    }
    else if (cb->isCheck(turn)) {
      cout << turn << " is in check" << endl;
    }
  }

}


void Game::play() {
  float whiteScore = 0;
  float blackScore = 0;
  while (cin) {
    initialize(whiteScore, blackScore);
    cb->clear();
  }
  cout << "Final Score:" << endl;
  cout << "White: " << whiteScore << endl;
  cout << "Black: " << blackScore << endl;
}

