#include "Board.h"

#include <exception>
#include <random>

Board::Board(int _stepsPerTick) : timestepsPerTick_(_stepsPerTick) {
  timeToNextTick_ = timestepsPerTick_;
  for (int i = 0; i < Board::HEIGHT; ++i) {
    for (int j = 0; j < Board::WIDTH; ++j) {
      board_.set(i, j, ' ');
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
  if (this->periodBetweenPieces_) {
    --timeToNextTick_;
    if (!timeToNextTick) {
      bringNextPieceUp();
      generateNextPiece();
      this->periodBetweenPieces_ = false;
    }
    return;
  }
  switch(_command) {
  case 1: currentPiece_->shiftLeft();
    // Let the Piece take care of whether it exceeds Board boundaries,
    // or if it encroaches on an already-laid area
  case 2: currentPiece_->shiftRight();
  case 3: currentPiece_->rotateAnti();
  case 4: currentPiece_->rotateClock();
  case 5: timeToNextTick_ = 1;
  case 6: currentPiece->dropToBottom(); timeToNextTick_ = 1;
  default: // Do nothing
  }
  --timeToNextTick_;
  if (!timeToNextTick) {
    timeToNextTick_ = timestepsPerTick_;
    this->tick();
  }
}

void Board::tick() {
  // First, check if the currentPiece is flush-against something below it.
  if (currentPiece_->checkCollideBelow()) {
    // If so, lay the current piece.
    layCurrentPiece();
  } else {
    // Else, displace the current piece down one step Piece::tickDown().
    currentPiece_->tickDown();
  }
}

void Board::layCurrentPiece() {
  // Get each row number of the current Piece's rotation frame
  int pieceRowPos = currentPiece_->getRowPos();
  int pieceColPos = currentPiece_->getColPos();
  int width = currentPiece_->rotateFrameWidth();
  int thisRow, thisCol;
  // This "lays" the current piece into the board 2D array permanently
  for (int i = 0; i < width; ++i) {
    thisRow = pieceRowPos + i;
    for (int j = 0; j < width; ++j) {
      thisCol = pieceColPos + j;
      if (currentPiece_->checkIfRowColOccupied(thisRow, thisCol)) {
	board_.set(thisRow, thisCol, currentPiece_->type());
      }
    }
  }
  for (int i = pieceRowPos; i < pieceRowPos + width; ++i) {
    score_ += tryCollapseRow(i);
  }
  this->periodBetweenPieces_ = true;
}

int Board::tryCollapseRows(int _row) {
  // Check if this row is all filled up. If so, collapse this row
  for (int j = 0; j < Board::WIDTH; ++j) {
    if (board_.get(_row, j) == ' ') {
      return 0;
    }
  }
  // Should be OK to do the slightly-inefficient row-by-row collapse
  // Do the collapse, then return 1 (to increment score by 1)
  int upperRow;
  for (int j = 0; j < Board::WIDTH; ++j) {
    for (int i = _row; i > 0; --i) {
      upperRow = _row - 1;
      board_[_row][j] = board_[upperRow][j];
      board_.set(_row, j, board_.get(upperRow, j));
    }
  }
  // Make sure top row is now empty.
  for (int j = 0; j < Board::WIDTH; ++j) board_.set(0, j, ' ');
  return 1;
}

// Private member functions

void Board::bringNextPieceUp() {
  currentPiece_ = nextPiece_;
  currentPiece_->resetPiece();
}

void Board::generateNextPiece() {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice);
  std::uniform_int_distribution<int> distribution(0, Board::NTYPES - 1);
  int randomInt = distribution(generator);
  nextPiece_ = pieces_ + randomInt;
}
