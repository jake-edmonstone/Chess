#include "graphicsdisplay.h"

using namespace std;

GraphicsDisplay::GraphicsDisplay(vector<vector<char>> charBoard): charBoard{charBoard} {
  int squareSize = 500 / 8;
  int offset = 40; // offset for the rank and file labels
  for (int i = 0; i < 8; ++i) { // draws the the rank and file labels
    xw.drawString(offset + i * squareSize + squareSize / 2 - 5, 25, std::string(1, 'a' + i)); 
    xw.drawString(10, offset + i * squareSize + squareSize / 2 + 5, std::to_string(i + 1));
  }
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      int x = offset + col * squareSize;
      int y = offset + row * squareSize;
      auto colour = (row + col) % 2 == 0 ? Xwindow::Blue : Xwindow::White; // makes checkerboard pattern
      xw.fillRectangle(x, y, squareSize, squareSize, colour);  // draws square
      if (charBoard[row][col] != ' ' && charBoard[row][col] != '_') {
        xw.drawString(x + 20, y + 40, string(1, charBoard[row][col])); // if there's a piece, draw it
      }
    }
  }
}

void GraphicsDisplay::notify(vector<vector<char>> newCharBoard) { // almost same as cotor
  int squareSize = 500 / 8;
  int offset = 40;
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      // checks to see where the new board is different from the old board. This way
      // only the different parts are redrawn
      if (newCharBoard[row][col] != charBoard[row][col]) {
        int x = offset + col * squareSize;
        int y = offset + row * squareSize;
        auto colour = (row + col) % 2 == 0 ? Xwindow::Blue : Xwindow::White;
        xw.fillRectangle(x, y, squareSize, squareSize, colour);
        if (newCharBoard[row][col] != ' ' && newCharBoard[row][col] != '_') {
          xw.drawString(x + 20, y + 40, string(1, newCharBoard[row][col]));
        }
      }
    }
  }
  charBoard = newCharBoard; // updates our "copy" of board
}
