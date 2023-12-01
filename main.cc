#include <iostream>
#include "random.h"
#include "chessboard.h"

int main() {
  ChessBoard cb {"default"};
  std::cout<<cb;
  std::cout << cb.rankFileToIntPair("e4").first << cb.rankFileToIntPair("e4").second << std::endl;
  cb.calculateAvailableMoves();
  cb.movePiece("c2", "c4");
  std::cout<<cb;
}