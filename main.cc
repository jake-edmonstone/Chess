#include <iostream>
#include "random.h"
#include "chessboard.h"

int main() {
  ChessBoard cb { "default" };
  std::string start;
  std::string end;
  while (std::cin >> start >> end) {
    cb.movePiece(start, end);
    std::cout << cb << std::endl;
  }
}