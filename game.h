#ifndef GAME_H
#define GAME_H

#include "abstractplayer.h"

class Game {
  // Game has 2 players, p1 is white, p2 is black
  std::unique_ptr<AbstractPlayer> p1;
  std::unique_ptr<AbstractPlayer> p2;
  // Game has 1 ChessBoard
  std::unique_ptr<ChessBoard> cb;
  // returns a pointer to a player of type playerType, colour colour. (used for game command)
  std::unique_ptr<AbstractPlayer> makePlayer(std::string playerType, std::string colour);
  // starts the board (called by play)
  void initialize(float &whiteScore, float &blackScore);
  // setup phase (called by initialize)
  void setup();
  // plays one game of potentially many
  void play1Game(float &whiteScore, float &blackScore);
  // for determining whether to have default board configuration or custom setup
  bool isSetup = false;
  // for turn taking
  bool whitesTurn = true;
 public:
  Game();
  // plays the Game!
  void play();
};

#endif
