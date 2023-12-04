#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "window.h"
#include <vector>


class GraphicsDisplay {
  Xwindow xw; // the window, only one made for one GraphicsDisplay object
  std::vector<std::vector<char>> charBoard; // current board state

 public:
  GraphicsDisplay(std::vector<std::vector<char>> charBoard); // Constructor
  void notify(std::vector<std::vector<char>> newCharBoard); // Called when the board is changed
};

#endif
