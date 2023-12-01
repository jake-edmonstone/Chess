#include <iostream>
#include "random.h"
#include "chessboard.h"

int main() {
  ChessBoard cb {"default"};
  std::cout<<cb;
  cb.calculateAvailableMoves();

  ChessBoard cb2 {cb};
  std::cout<<cb2;
}