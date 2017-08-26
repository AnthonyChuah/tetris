#include "Board.h"

#include <exception>
#include <random>

Board::Board() {
  for (int i = 0; i < Board::HEIGHT; ++i) {
    for (int j = 0; j < Board::WIDTH; ++j) {
      board_[i][j] = ' ';
    }
  }
  pieces_[0] = Piece('o', board_);
  pieces_[1] = Piece('l', board_);
  pieces_[2] = Piece('s', board_);
  pieces_[3] = Piece('z', board_);
  pieces_[4] = Piece('j', board_);
  pieces_[5] = Piece('7', board_);
  pieces_[6] = Piece('t', board_);
  this->generateNextPiece();
  this->bringNextPieceUp();
  this->generateNextPiece();
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
     ACTUALLY, CONSIDER MOVING THIS TO PIECE::
   */
}

void Board::bringNextPieceUp() {
  currentPiece_ = nextPiece_;
}

void Board::generateNextPiece() {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice);
  std::uniform_int_distribution<int> distribution(0, Board::NTYPES - 1);
  int randomInt = distribution(generator);
  nextPiece_ = pieces_ + randomInt;
}
