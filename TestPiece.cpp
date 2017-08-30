#include "Grid.h"
#include "Piece.h"

#include <cassert>
#include <iostream>

int main() {
  Piece::populateLookupMaps();
  Grid<20, 10> board;
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      board.set(i, j, ' ');
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
  std::cout << "Passed 1 tests: Piece::type\n";
  // getRowPos
  assert(testO.getRowPos() == 0);
  assert(testS.getRowPos() == 0);
  std::cout << "Passed 2 tests: Piece::getRowPos\n";
  // getColPos
  assert(testO.getColPos() == 3);
  assert(testL.getColPos() == 2);
  std::cout << "Passed 2 tests: Piece::getColPos\n";
  // rotateFrameWidth
  assert(testO.rotateFrameWidth() == 2);
  assert(testL.rotateFrameWidth() == 4);
  assert(testT.rotateFrameWidth() == 3);
  std::cout << "Passed 3 tests: Piece::rotateFrameWidth\n";
  // checkIfRowColOccupied
  assert(testL.checkIfRowColOccupied(0, 2) == false);
  assert(testL.checkIfRowColOccupied(3, 3) == true);
  assert(testL.checkIfRowColOccupied(2, 3) == true);
  assert(testL.checkIfRowColOccupied(1, 3) == true);
  assert(testL.checkIfRowColOccupied(0, 3) == true);
  assert(testO.checkIfRowColOccupied(19, 9) == false);
  std::cout << "Passed 6 tests: Piece::checkIfRowColOccupied\n";
  // shiftLeft
  testO.shiftLeft();
  assert(testO.getColPos() == 2);
  testO.shiftLeft();
  testO.shiftLeft(); // After this, colPos is 0
  testO.shiftLeft(); // It should not be allowed to go under 0
  assert(testO.getColPos() == 0);
  std::cout << "Passed 2 tests: Piece::shiftLeft\n";
  // resetPiece
  bool resetResult = testO.resetPiece();
  assert(testO.getColPos() == 3);
  assert(resetResult == true);
  board.set(0, 3, 'o');
  resetResult = testO.resetPiece();
  assert(resetResult == false);
  board.set(0, 3, ' ');
  std::cout << "Passed 3 tests: Piece::resetPiece\n";
  // shiftRight
  testS.shiftRight();
  assert(testS.getColPos() == 4);
  testS.shiftRight(); // 5
  testS.shiftRight(); // 6
  testS.shiftRight(); // 7
  testS.shiftRight(); // Should not be able to go above 7
  assert(testS.getColPos() == 7);
  std::cout << "Passed 2 tests: Piece::shiftRight\n";
  // tickDown
  testS.tickDown(); // Row: 1
  assert(testS.getRowPos() == 1);
  std::cout << "Passed 1 tests: Piece::tickDown\n";
  // dropToBottom
  board.set(4, 7, 'o');
  testS.dropToBottom();
  assert(testS.getColPos() == 7);
  assert(testS.getRowPos() == 1);
  // checkCollideBelow
  assert(testS.checkCollideBelow() == true);
  board.set(4, 7, ' ');
  assert(testS.checkCollideBelow() == false);
  testS.dropToBottom();
  assert(testS.getColPos() == 7);
  assert(testS.getRowPos() == 17);
  assert(testS.checkCollideBelow() == true);
  testS.resetPiece();
  assert(testS.checkCollideBelow() == false);
  std::cout << "Passed 8 tests: dropToBottom and checkCollideBelow\n";
  // rotateAnti rotateClock
  // 1st case: simple unobstructed rotation
  testL.resetPiece();
  assert(testL.checkIfRowColOccupied(0, 3) == true);
  assert(testL.checkIfRowColOccupied(3, 3) == true);
  testL.rotateAnti();
  assert(testL.checkIfRowColOccupied(2, 2) == true);
  assert(testL.checkIfRowColOccupied(2, 5) == true);
  assert(testL.checkIfRowColOccupied(1, 3) == false);
  std::cout << "Passed 5 tests: rotateAnti (unobstructed)\n";
  testL.resetPiece();
  testL.rotateClock();
  assert(testL.checkIfRowColOccupied(1, 2) == true);
  assert(testL.checkIfRowColOccupied(1, 5) == true);
  assert(testL.checkIfRowColOccupied(0, 3) == false);
  std::cout << "Passed 3 tests: rotateClock (unobstructed)\n";
  // 2nd case: obstructed and impossible rotation
  testL.resetPiece();
  board.set(2, 2, 'o');
  bool rotateResult = testL.rotateAnti();
  assert(rotateResult == false);
  assert(testL.checkIfRowColOccupied(2, 5) == false);
  board.set(2, 2, ' ');
  rotateResult = testL.rotateAnti();
  assert(rotateResult == true);
  assert(testL.checkIfRowColOccupied(2, 5) == true);
  std::cout << "Passed 4 tests: rotateAnti (obstructed)\n";
  board.set(0, 3, 'o');
  rotateResult = testL.rotateClock();
  assert(rotateResult == false);
  assert(testL.checkIfRowColOccupied(2, 5) == true);
  board.set(0, 3, ' ');
  rotateResult = testL.rotateClock();
  assert(rotateResult == true);
  assert(testL.checkIfRowColOccupied(2, 5) == false);
  std::cout << "Passed 4 tests: rotateClock (obstructed)\n";
  // 3rd case: displaced rotation
  testL.resetPiece();
  testL.shiftLeft();
  testL.shiftLeft();
  testL.shiftLeft();
  assert(testL.getColPos() == -1);
  board.set(2, 3, 'o');
  // Should block the rotate-displace sequence, thus piece should fail to rotate
  rotateResult = testL.rotateAnti();
  assert(rotateResult == false);
  assert(testL.getColPos() == -1);
  board.set(2, 3, ' ');
  rotateResult = testL.rotateAnti();
  assert(rotateResult == true);
  assert(testL.getColPos() == 0);
  std::cout << "Passed 4 tests: rotateAnti (displaced)\n";
  testL.rotateAnti();
  testL.shiftLeft();
  testL.shiftLeft();
  assert(testL.getColPos() == -2);
  board.set(2, 3, 'o');
  rotateResult = testL.rotateClock();
  assert(rotateResult == false);
  assert(testL.getColPos() == -2);
  board.set(2, 3, ' ');
  rotateResult = testL.rotateClock();
  assert(rotateResult == true);
  assert(testL.getColPos() == 0);
  std::cout << "Passed 5 tests: rotateClock (displaced)\n";
  // All tests passed
  return 0;
}
