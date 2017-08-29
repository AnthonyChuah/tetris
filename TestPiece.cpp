#include "Piece.h"

#include <cassert>
#include <iostream>

int main() {
  char board[20][10];
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      board[i][j] = ' ';
    }
  }
  Piece testO('o', board);
  Piece testL('l', board);
  Piece testS('s', board);
  Piece testZ('z', board);
  Piece testJ('j', board);
  Piece test7('7', board);
  Piece testT('t', board);
  // type
  assert(testO.type() == 'o');
  std::cout << "Passed tests: Piece::type\n";
  // getRowPos
  assert(testO.getRowPos() == 0);
  assert(testL.getRowPos() == 0);
  std::cout << "Passed tests: Piece::getRowPos\n";
  // getColPos
  assert(testO.getColPos() == 3);
  assert(testL.getColPos() == 2);
  std::cout << "Passed tests: Piece::getColPos\n";
  // rotateFrameWidth
  assert(testO.rotateFrameWidth() == 2);
  assert(testL.rotateFrameWidth() == 4);
  assert(testT.rotateFrameWidth() == 3);
  std::cout << "Passed tests: Piece::rotateFrameWidth\n";
  // checkIfRowColOccupied
  assert(testL.checkIfRowColOccupied(0, 2) == false);
  assert(testL.checkIfRowColOccupied(3, 3) == true);
  assert(testL.checkIfRowColOccupied(3, 2) == true);
  assert(testL.checkIfRowColOccupied(3, 1) == true);
  assert(testL.checkIfRowColOccupied(3, 0) == true);
  assert(testO.checkIfRowColOccupied(19, 9) == false);
  std::cout << "Passed tests: Piece::checkIfRowColOccupied\n";
  // shiftLeft
  testO.shiftLeft();
  assert(testO.getColPos() == 2);
  testO.shiftLeft();
  testO.shiftLeft(); // After this, colPos is 0
  testO.shiftLeft(); // It should not be allowed to go under 0
  assert(testO.getColPos() == 0);
  std::cout << "Passed tests: Piece::shiftLeft\n";
  // resetPiece
  testO.resetPiece();
  assert(testO.getColPos() == 3);
  std::cout << "Passed tests: Piece::resetPiece\n";
  // shiftRight
  testS.shiftRight();
  assert(testS.getColPos() == 4);
  testS.shiftRight(); // 5
  testS.shiftRight(); // 6
  testS.shiftRight(); // 7
  testS.shiftRight(); // Should not be able to go above 7
  assert(testS.getColPos() == 7);
  std::cout << "Passed tests: Piece::shiftRight\n";
  // tickDown
  testS.tickDown(); // Row: 1
  assert(testS.getRowPos() == 1);
  std::cout << "Passed tests: Piece::tickDown\n";
  // dropToBottom
  board[4][7] = 'o';
  testS.dropToBottom();
  assert(testS.getColPos() == 7);
  assert(testS.getRowPos() == 1);
  // checkCollideBelow
  assert(testS.checkCollideBelow() == true);
  board[4][7] = ' ';
  assert(testS.checkCollideBelow() == false);
  testS.dropToBottom();
  assert(testS.getColPos() == 7);
  assert(testS.getRowPos() == 17);
  assert(testS.checkCollideBelow() == true);
  testS.resetPiece();
  assert(testS.checkCollideBelow() == false);
  std::cout << "Passed tests: dropToBottom and checkCollideBelow\n";
  
  return 0;
}
