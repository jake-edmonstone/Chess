#include <iostream>
#include "game.h"
#include "abstractplayer.h"

using namespace std;


Game::Game() { // ctor
  cb = make_unique<ChessBoard>();
}

unique_ptr<AbstractPlayer> Game::makePlayer(string playerType, string colour) {
  if (playerType == "human") {
    return make_unique<Human>(cb.get(), colour);
  } else if (playerType == "computer1") {
    return make_unique<Computer1>(cb.get(), colour);
  } else if (playerType == "computer2") {
    return make_unique<Computer2>(cb.get(), colour);
  } else if (playerType == "computer3") {
    return make_unique<Computer3>(cb.get(), colour);
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
      cb->clear(); // when game is done, board gets cleard
      isSetup = false; // is not setup anymore, needs a call to setup() if custom is desired
    }
  }
}
void Game::setup() {
  cb->clear();
  cout << *cb; // print text display
  cb->gd->notify(cb->getBoard()); // notify graphics display something has changed
  string command;
  while (cin) {
    cin >> command;
    if (!cin) break;
    if (command == "+") {
      char piece;
      string position;
      cin >> piece >> position;
      if (piece == 'p' || piece == 'P') {
        if (position[1] == '1' || position[1] == '8') { // if first or last rank
          cout << "You cannot have pawns on the first or last rank" << endl;
          continue;
        }
      }
      cb->placePiece(piece, position); // puts the valid piece in the desired spot
    } else if (command == "-") {
      string position;
      cin >> position;
      cb->removePiece(position); // removes piece at position position
    } else if (command == "=") {
      string colour;
      cin >> colour;
      // asserts whos turn it is
      if (colour == "black") whitesTurn = false;
      if (colour == "white") whitesTurn = true;
    } else if (command == "done") {
      // make sure there is exactly 1 white king and 1 black king
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
      // update ChessBoard internals
      cb->calculateAvailableMoves();
      cb->dontCheckYourself();
      cb->getOutOfCheck();

      if (cb->isCheck("white") || cb->isCheck("black")) { // is either side in check?
        cout << "Neither colour can be in check" << endl;
        continue;
      }
      isSetup = true; // setup is done
      break;
    }
    cout << *cb;
    cb->gd->notify(cb->getBoard());
  }
}

void Game::play1Game(float &whiteScore, float &blackScore) {
  if (!isSetup) { // assumes they want default board setup (setup wasn't called)
    cb->clear();
    cb->defaultSetup();
  }
  // update ChessBoard internals
  cb->dontCheckYourself();
  cb->getOutOfCheck();
  cb->updatePieceLists();
  cout << *cb;
  cb->gd->notify(cb->getBoard());
  while (cin) {
    string start, end;
    if (whitesTurn) {
      while (true) {
        pair<string, string> move = p1->getMove(); // polymorphic getMove
        start = move.first;
        if (start == "resign") { // check if they resigned
          ++blackScore;
          return;
        }
        end = move.second;
        if (cb->movePiece(start, end)) break;
      }
    }
    else if (!whitesTurn) {
      while (true) {
        // same thing but for white
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
    if (cb->pawnOnFirstOrLastRank()) {
      // promotion handling
      if (whitesTurn) {
        cb->placePiece(toupper(p1->getPromotionDecision()), end);
      } else if (!whitesTurn) {
        cb->placePiece(tolower(p2->getPromotionDecision()), end);
      }
      cb->calculateAvailableMoves(); // explicit call since pieces change but movePiece is not called which normally handles this
    }
    cb->dontCheckYourself();
    cb->getOutOfCheck();


    whitesTurn = !whitesTurn; // change turns
    cout << *cb;
    cb->gd->notify(cb->getBoard());

    string turn = whitesTurn ? "white" : "black";
    string notTurn = whitesTurn ? "black" : "white";

    // Checkmate, Stalemate, insufficient material and chek handling

    if (cb->isCheckMate(turn)) {
      cout << "Checkmate! " << notTurn << " Wins!" << endl;
      if (notTurn == "white") ++whiteScore;
      else ++blackScore;
      break;
    } else if (cb->isStaleMate(turn)) {
      cout << "Stalemate!" << endl;
      whiteScore += 0.5;
      blackScore += 0.5;
      break;
    } else if (cb->isInsufficientMaterial()) {
      cout << "There is insufficent material for either colour to win" << endl;
      whiteScore += 0.5;
      blackScore += 0.5;
      break;
    } else if (cb->isCheck(turn)) {
      cout << turn << " is in check" << endl;
    }
  }

}

void Game::play() {
  // starting scores
  float whiteScore = 0;
  float blackScore = 0;
  while (cin) {  // will continue to play games until ctrl+D
    initialize(whiteScore, blackScore);
  }
  // Show the final scores
  cout << "Final Score:" << endl;
  cout << "White: " << whiteScore << endl;
  cout << "Black: " << blackScore << endl;
}
