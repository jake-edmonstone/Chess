#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include "chessboard.h"

struct State {
  ChessBoard cb;
  std::pair<std::string, std::string> move;
  std::vector<std::shared_ptr<State>> nextStates;
};

class AbstractPlayer {
 protected:
  const ChessBoard *cb; // pointer to const ChessBoard. Each player can "observe" the board but not change it
  std::string colour;   // colour ("black" or "white") 
 public:
  AbstractPlayer(ChessBoard *cb, std::string colour); // ctor
  virtual ~AbstractPlayer() = 0; // virtual destructor
   // this is the main functionality of the players, returns a pair of strings that describes the move.
   // this is what allows for the polymorphism of players (computer vs human, computer vs computer etc.)
  virtual std::pair<std::string, std::string>getMove(std::string config="sleep") const = 0;
  std::string getColour() const; // getter
  virtual char getPromotionDecision() const = 0; // for when a player can promote a pawn
  // gets all moves that lead to check, but not checkmate for the player
  std::vector<std::pair<std::string, std::string>> getMovesCheck() const;
  // gets all moves that lead to checkmate for the player
  std::vector<std::pair<std::string, std::string>> getMovesCheckMate() const;
  // gets all moves that lead to a capture
  std::vector<std::pair<std::string, std::string>> getMovesCapture() const;
  // gets all moves that avoid a threat
  std::vector<std::pair<std::string, std::string>> getMovesAvoidThreat() const;
  // fetches a random move
  std::pair<std::string, std::string> getRandomMove() const;


};

class Human: public AbstractPlayer {
 public:
  Human(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove(std::string config="nosleep") const override;
  char getPromotionDecision() const override;
};

class Computer1: public AbstractPlayer {
 public:
  Computer1(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove(std::string config="sleep") const override;
  char getPromotionDecision() const override;
};

class Computer2: public AbstractPlayer {
 public:
  Computer2(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove(std::string config="sleep") const override;
  char getPromotionDecision() const override;
};

class Computer3: public AbstractPlayer {
 public:
  Computer3(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove(std::string config="sleep") const override;
  char getPromotionDecision() const override;
};

class Computer4: public AbstractPlayer {
 public:
  Computer4(ChessBoard *cb, std::string colour);
  std::pair<std::string, std::string>getMove(std::string config="sleep") const override;
  void findPossibleMoves(State& state, int depth, bool turn) const;
  std::pair<std::string, std::string> getOptimalMove(State& root, int peakValSoFar) const;
  int getPeakValueDownPath(const State* state, std::string bound) const;
  int minimax(const ChessBoard* bp, int depth, bool isblack) const;
  char getPromotionDecision() const override;
};

#endif
