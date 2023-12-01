#include <iostream>
#include "random.h"
#include "chessboard.h"

int main() {
  ChessBoard cb {"default"};
  std::cout << cb;
  cb.calculateAvailableMoves();
  
  while (true) { 
    std::string start, end;
    std::cin >> start >> end; 
    cb.movePiece(start, end); 
    std::cout << cb;
  } 
}