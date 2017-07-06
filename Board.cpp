#include "Board.h"

#include <exception>

Board::Board() {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < HEIGHT; ++j) {
      board_[i][j] = ' ';
    }
  }
}

void Board::timestep(int _command) {
  ++timesteps_;
  /* Logic of commands:
     Case 5: speeding up descent, simply make the next tick happen immediately,
     by setting the time to next tick to 1.
     Case 6: pushing current piece to bottom immediately requires some calculation to 
     figure out how far the current piece should go, then make the next tick happen,
     in order to lock the piece in place
     The tick() function will check if current piece is "flush" against either the floor or 
     an already-laid piece
   */
  switch(_command) {
  case 1: currentPiece_->shiftLeft();
    // Let the Piece take care of whether it exceeds Board boundaries,
    // or if it encroaches on an already-laid area
  case 2: currentPiece_->shiftRight();
  case 3: currentPiece_->rotateAnti();
  case 4: currentPiece_->rotateClock();
  case 5: timeToNextTick_ = 1;
  case 6: dropToBottom(); timeToNextTick_ = 1;
  default: // Do nothing
  }
  --timeToNextTick_;
  if (!timeToNextTick) {
    timeToNextTick_ = timestepsPerTick_;
    this->tick();
  }
}

void Board::dropToBottom() {
  /* First, retrieve the y-displacements of the lowest points of currentPiece
     Check the distance to fall for each of the lowest points
     Find the shortest, then displace the piece downwards by that distance
   */
}
