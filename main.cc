#include <iostream>
#include "random.h"
#include "chessboard.h"

using namespace std;

int main() {
  ChessBoard cb {"default"};
  std::cout << cb;
  
  while (true) { 
    std::string start, end;
    std::cin >> start >> end; 
    cb.movePiece(start, end);
    cb.dontCheckYourself();
    cb.getOutOfCheck();
    std::cout << cb;
  } 
}