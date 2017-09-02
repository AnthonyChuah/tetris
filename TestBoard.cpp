#include "Board.h"

#include <iostream>
#include <cassert>

class BoardTester {

public:

  Board& board_;
  
  BoardTester(Board& _board) : board_(_board) {}
  long getTimesteps() const { return board_.timesteps_; }
  long getScore() const { return board_.score_; }
  int getTimeToNextTick() const { return board_.timeToNextTick_; }
  bool getPeriodBP() const { return board_.periodBetweenPieces; }
  char getTypeOfCurrentPiece() const { return board_.currentPiece_->type(); }
  char getTypeOfNextPiece() const { return board_.nextPiece_->type(); }
  int getOrientation() const { return board_.currentPiece_->getOrient(); }
  void setCurrentPiece(int _i) {
    board_.currentPiece_ = board_.pieces_[_i];
  }
  void setNextPiece(int _i) {
    board_.nextPiece_ = board_.pieces_[_i];
  }
  void setGridAt(int _r, int _c, char _ch) {
    board_.board_.set(_r, _c, _ch);
  }
  char getGridAt(int _r, int _c) const { return board_.board_.get(_r, _c); }
  int getPieceCol() const { return board_.currentPiece->getColPos(); }
  int getPieceRow() const { return board_.currentPiece->getRowPos(); }
  
};

int main() {
  // Using BoardTester class, run unit tests for Board
  // 1. Construct the Board.
  // Check that the current and next pieces are correctly initialized
  // Now for our further tests, set current to 'o' piece, next to 'l' piece
  Piece::populateLookupMaps();
  Board board(8);
  BoardTester tester(board);
  tester.setCurrentPiece(0);
  tester.setNextPiece(1);

  // 2. Give 4 commands to rotate clockwise, then 4 commands to shiftRight
  // Since it is 8 timesteps, the tick should happen automatically. Check that piece
  // has stepped down one step
  // 1. shiftLeft 2. shiftRight 3. rotateAnti 4. rotateClock 5. stepDown 6. dropBottom
  assert(tester.getTypeOfCurrentPiece() == 'o');
  assert(tester.getTypeOfNextPiece() == 'l');
  assert(tester.getPeriodBP() == false);
  assert(tester.getPieceCol() == 3);
  assert(tester.getPieceRow() == 0);
  tester.board_.timestep(4);
  assert(tester.getOrientation() == 1);
  tester.board_.timestep(4);
  assert(tester.getOrientation() == 2);
  tester.board_.timestep(4);
  assert(tester.getOrientation() == 3);
  tester.board_.timestep(4);
  assert(tester.getOrientation() == 0);
  tester.board_.timestep(2);
  tester.board_.timestep(2);
  tester.board_.timestep(2);
  assert(tester.getPieceCol() == 6);
  assert(tester.getPieceRow() == 0);
  tester.board_.timestep(2);
  assert(tester.getPieceCol() == 7);
  assert(tester.getPeriodBP() == false);
  assert(tester.getPieceRow() == 1);
  assert(tester.getPeriodBP() == true);
  assert(tester.getTypeOfCurrentPiece() == 'l');
  std::cout << "Passed 16 tests: Board::timestep (tickDown))\n";
  std::cout << "Next piece was randomly generated to be of type: "
	    << tester.getTypeOfNextPiece() << "\n";
  std::cout << "Only after initial 8 timesteps, does Board change to PeriodBetweenPieces\n";

  // Now, the Board should ignore 8 commands and do nothing
  tester.board_.timestep(-5);
  tester.board_.timestep(100);
  tester.board_.timestep(0);
  tester.board_.timestep(1);
  tester.board_.timestep(1);
  assert(tester.getPieceCol() == 7);
  assert(tester.getPieceRow() == 1);
  tester.board_.timestep(5);
  tester.board_.timestep(6);
  assert(tester.getPeriodBP() == true);
  assert(tester.getPieceCol() == 7);
  assert(tester.getPieceRow() == 1);
  tester.board_.timestep(3);
  assert(tester.getOrientation() == 0);
  assert(tester.getPeriodBP() == false);
  std::cout << "Passed 7 tests: Board::timestep (during period between pieces)\n";
  
  // 3. Give 1 command to dropToBottom
  // Verify that timeToNextTick is 8. Verify that the piece has gone to the bottom
  // Verify that the total timesteps is 9
  // Verify that the piece has been "laid", such that the Grid now contains the piece
  // Verify that periodBetweenPieces is true
  // Verify that with 8 more commands (any), nothing happens to piece (piece still at top)
  // Verify that now periodBetweenPieces is false
  // Verify that the piece now moves, by shifting left once and checking col position

  // 4. Test the collapsing of a row
  // Fill up the bottom row completely
  // Trigger tryCollapseRow on bottom row
  // Verify that the two squares on the rightmost have gone down one step

  // 5. Test the in-game laying of a piece which collapses a row
  // Fill up bottom row again, except for the leftmost spot
  // Shift left 3 more times, then dropToBottom
  // Verify that score is now 1
  // Verify that bottom row is now empty (collapsed) except for the leftmost square
  
  // End of unit tests for Board
  return 0;
}
