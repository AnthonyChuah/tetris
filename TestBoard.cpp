#include "Board.h"

#include <iostream>
#include <cassert>

// Piece guide: 0 o, 1 l, 2 s, 3 z, 4 j, 5 7, 6 t
// Moves guide: 1 left, 2 right, 3 anti, 4 clock, 5 down, 6 drop
class BoardTester {

public:

  Board& board_;
  
  BoardTester(Board& _board) : board_(_board) {}
  long getTimesteps() const { return board_.timesteps_; }
  long getScore() const { return board_.score_; }
  int getTimeToNextTick() const { return board_.timeToNextTick_; }
  bool getPeriodBP() const { return board_.periodBetweenPieces_; }
  char getTypeOfCurrentPiece() const { return board_.currentPiece_->type(); }
  char getTypeOfNextPiece() const { return board_.nextPiece_->type(); }
  int getOrientation() const { return board_.currentPiece_->getOrient(); }
  void setCurrentPiece(int _i) {
    board_.currentPiece_ = &board_.pieces_[_i];
  }
  void setNextPiece(int _i) {
    board_.nextPiece_ = &board_.pieces_[_i];
  }
  void setGridAt(int _r, int _c, char _ch) {
    board_.board_.set(_r, _c, _ch);
  }
  char getGridAt(int _r, int _c) const { return board_.board_.get(_r, _c); }
  int getPieceCol() const { return board_.currentPiece_->getColPos(); }
  int getPieceRow() const { return board_.currentPiece_->getRowPos(); }
  char getBoardElement(int _r, int _c) const {
    return board_.board_.get(_r, _c);
  }
  void setBoardElement(int _r, int _c, char _dat) {
    board_.board_.set(_r, _c, _dat);
  }
  
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
  assert(tester.getTimeToNextTick() == 1);
  tester.board_.timestep(2);
  assert(tester.getTimeToNextTick() == 8);
  assert(tester.getPieceCol() == 7);
  assert(tester.getPieceRow() == 1);
  assert(tester.getTimesteps() == 8);
  assert(tester.getPeriodBP() == false);
  assert(tester.getTypeOfCurrentPiece() == 'o');
  std::cout << "Passed tests: Board::timestep (tickDown)\n";

  // Now, the Board should ignore commands out of range and do nothing
  tester.board_.timestep(-5);
  tester.board_.timestep(100);
  tester.board_.timestep(0);
  tester.board_.timestep(1); // shiftLeft
  tester.board_.timestep(1); // shiftLeft
  assert(tester.getTimeToNextTick() == 3);
  assert(tester.getPieceCol() == 5);
  assert(tester.getPieceRow() == 1);
  tester.board_.timestep(5); // down
  assert(tester.getTimeToNextTick() == 8);
  assert(tester.getPieceRow() == 2);
  std::cout << "Passed tests: Board::timestep (invalid commands)\n";
  
  // Drop to bottom!
  tester.board_.timestep(6);
  assert(tester.getTimeToNextTick() == 8);
  assert(tester.getPeriodBP() == true);
  assert(tester.getBoardElement(18, 5) == 'o');
  assert(tester.getBoardElement(18, 6) == 'o');
  assert(tester.getBoardElement(19, 5) == 'o');
  assert(tester.getBoardElement(19, 6) == 'o');
  tester.board_.timestep(0);
  tester.board_.timestep(0);
  tester.board_.timestep(0);
  tester.board_.timestep(0);
  tester.board_.timestep(0);
  tester.board_.timestep(0);
  tester.board_.timestep(0);
  assert(tester.getPeriodBP() == true);
  assert(tester.getTimeToNextTick() == 1);
  tester.board_.timestep(0);
  assert(tester.getPeriodBP() == false);
  assert(tester.getBoardElement(18, 5) == 'o');
  assert(tester.getTimeToNextTick() == 8);
  assert(tester.getTypeOfCurrentPiece() == 'l');
  assert(tester.getPieceCol() == 2);
  assert(tester.getPieceRow() == 0);
  tester.setNextPiece(6); // Next piece is T
  tester.board_.timestep(1);
  tester.board_.timestep(1);
  tester.board_.timestep(1);
  assert(tester.getPieceCol() == -1);
  assert(tester.getPieceRow() == 0);
  std::cout << "Passed tests: Board::timestep (dropToBottom)\n";
  
  // Test that rotates displace piece if out of bounds
  tester.board_.timestep(3); // anti-clockwise
  tester.board_.timestep(1); // do nothing, blocked by wall
  assert(tester.getPieceCol() == 0);
  assert(tester.getPieceRow() == 0);
  assert(tester.getOrientation() == 3);
  std::cout << "Passed tests: Board::timestep (rotateAnti)\n";
  tester.board_.timestep(4); // clock
  assert(tester.getPieceCol() == 0);
  assert(tester.getPieceRow() == 0);
  assert(tester.getOrientation() == 0);
  tester.board_.timestep(1); // left back to flush against wall
  assert(tester.getPieceCol() == -1);
  tester.board_.timestep(4); // clock back to horizontal, now dropped 1 row
  assert(tester.getPieceCol() == 0);
  assert(tester.getPieceRow() == 1);
  assert(tester.getOrientation() == 1);
  std::cout << "Passed tests: Board::timestep (rotateClock)\n";

  // Now I drop the long piece to bottom
  tester.setNextPiece(5); // 7 piece
  tester.board_.timestep(6); // drop
  // Now the current piece upcoming is 7
  assert(tester.getBoardElement(19, 0) == 'l');
  assert(tester.getBoardElement(19, 1) == 'l');
  assert(tester.getBoardElement(19, 2) == 'l');
  assert(tester.getBoardElement(19, 3) == 'l');
  // Make 8 steps to wait for new sevenPiece to be current
  tester.board_.timestep(0); tester.board_.timestep(0); tester.board_.timestep(0);
  tester.board_.timestep(0); tester.board_.timestep(0); tester.board_.timestep(0);
  tester.board_.timestep(0); tester.board_.timestep(0);
  assert(tester.getTypeOfCurrentPiece() == '7');
  tester.board_.timestep(1); // left, now 7 piece should have tip aligned above hole
  assert(tester.getPeriodBP() == false);
  tester.board_.timestep(6); // drop
  assert(tester.getPeriodBP() == true);
  assert(tester.getBoardElement(19, 4) == '7');
  assert(tester.getBoardElement(18, 4) == '7');
  assert(tester.getBoardElement(17, 4) == '7');
  assert(tester.getBoardElement(17, 3) == '7');
  std::cout << "Passed tests: Board::tryCollapseRows (no collapse)\n";
  
  // 4. Test the collapsing of a row
  tester.setNextPiece(6); // t piece
  // Now trigger the end of period between pieces by making 8 commands (anything)
  tester.board_.timestep(0); tester.board_.timestep(0); tester.board_.timestep(0);
  tester.board_.timestep(0); tester.board_.timestep(0); tester.board_.timestep(0);
  tester.board_.timestep(0); tester.board_.timestep(0);
  // Now t piece is the current piece
  assert(tester.getTypeOfCurrentPiece() == 't');
  assert(tester.getOrientation() == 0);
  tester.board_.timestep(3);
  assert(tester.getOrientation() == 3);
  tester.board_.timestep(3);
  assert(tester.getOrientation() == 2);
  // Shift t piece rightwards 4 times
  tester.board_.timestep(2); tester.board_.timestep(2); // right
  tester.board_.timestep(2); tester.board_.timestep(2);
  // Verify that the two squares on the rightmost have gone down one step
  assert(tester.getPeriodBP() == false);
  assert(tester.getScore() == 0);
  tester.board_.timestep(6); // drop
  assert(tester.getPeriodBP() == true);
  assert(tester.getScore() == 1); // score should increment
  // Verify that bottom of board looks like:
  //     7oo t
  assert(tester.getBoardElement(19, 4) == '7');
  assert(tester.getBoardElement(19, 5) == 'o');
  assert(tester.getBoardElement(19, 6) == 'o');
  assert(tester.getBoardElement(19, 8) == 't');
  // Second row from bottom looks like:
  //    77
  assert(tester.getBoardElement(18, 4) == '7');
  assert(tester.getBoardElement(18, 3) == '7');
  // End of unit tests for Board
  std::cout << "Passed tests: Board::tryCollapseRows (collapse)\n";

  // Check loss condition
  tester.setNextPiece(1); // l piece
  tester.setBoardElement(4, 3, 'o');
  // Now trigger the end of period between pieces by making 8 commands (anything)  
  tester.board_.timestep(0); tester.board_.timestep(0); tester.board_.timestep(0);
  tester.board_.timestep(0); tester.board_.timestep(0); tester.board_.timestep(0);
  tester.board_.timestep(0); tester.board_.timestep(0);
  assert(tester.getPeriodBP() == false);
  tester.board_.timestep(5); // forces next tick, laying the piece
  assert(tester.getBoardElement(0, 3) == 'l');
  assert(tester.getBoardElement(1, 3) == 'l');
  assert(tester.getBoardElement(2, 3) == 'l');
  assert(tester.getBoardElement(3, 3) == 'l');
  assert(tester.getBoardElement(4, 3) == 'o');
  tester.setNextPiece(0);
  assert(tester.getPeriodBP() == true);
  bool gameNotLost = true;
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(0);
  assert(gameNotLost == true);
  assert(tester.getPeriodBP() == true);
  gameNotLost = tester.board_.timestep(0);
  gameNotLost = tester.board_.timestep(6);
  assert(tester.getPeriodBP() == false);
  assert(gameNotLost == false); // Failed this test
  std::cout << "Passed tests: Board loss conditions\n";
  
  return 0;
}
