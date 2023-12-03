#ifndef GAME_H
#define GAME_H

#include "abstractplayer.h"

class Game {
  std::unique_ptr<AbstractPlayer> p1;
  std::unique_ptr<AbstractPlayer> p2;
  std::unique_ptr<ChessBoard> cb;
  std::unique_ptr<AbstractPlayer> makePlayer(std::string playerType, std::string colour);
  void initialize(float &whiteScore, float &blackScore);
  void setup();
  void play1Game(float &whiteScore, float &blackScore);
  bool isSetup = false;
  bool whitesTurn = true;
 public:
  Game();
  void play();
};

#endif