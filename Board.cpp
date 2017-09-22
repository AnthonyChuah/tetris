#include "Board.h"

#include <exception>
#include <random>

Board::Board(int _stepsPerTick = 8) : timestepsPerTick_(_stepsPerTick) {
  timeToNextTick_ = timestepsPerTick_;
  for (int i = 0; i < Board::HEIGHT; ++i) {
    for (int j = 0; j < Board::WIDTH; ++j) {
      board_.set(i, j, ' ');
    }
  }
  Piece oPiece('o', &board_);
  Piece lPiece('l', &board_);
  Piece sPiece('s', &board_);
  Piece zPiece('z', &board_);
  Piece jPiece('j', &board_);
  Piece sevenPiece('7', &board_);
  Piece tPiece('t', &board_);
  pieces_[0] = oPiece;
  pieces_[1] = lPiece;
  pieces_[2] = sPiece;
  pieces_[3] = zPiece;
  pieces_[4] = jPiece;
  pieces_[5] = sevenPiece;
  pieces_[6] = tPiece;
  this->generateNextPiece();
  this->bringNextPieceUp();
  this->generateNextPiece();
}

bool Board::timestep(int _command) {
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
  if (this->gameLost_) {
    std::cout << "Board::timestep found that game has been lost.\n";
    return false;
  }
  if (this->periodBetweenPieces_) {
    --timeToNextTick_;
    if (!timeToNextTick_) {
      timeToNextTick_ = timestepsPerTick_;
      bringNextPieceUp();
      generateNextPiece();
      this->periodBetweenPieces_ = false;
    }
    return true;
  }
  switch(_command) {
  case 1: currentPiece_->shiftLeft();
    break;
  case 2: currentPiece_->shiftRight();
    break;
  case 3: currentPiece_->rotateAnti();
    break;
  case 4: currentPiece_->rotateClock();
    break;
  case 5: timeToNextTick_ = 1;
    break;
  case 6: currentPiece_->dropToBottom(); timeToNextTick_ = 1;
    break;
  default:
    break;
  }
  --timeToNextTick_;
  if (!timeToNextTick_) {
    timeToNextTick_ = timestepsPerTick_;
    this->tick();
  }
  return true;
}

void Board::tick() {
  if (currentPiece_->checkCollideBelow()) {
    layCurrentPiece();
  } else {
    currentPiece_->tickDown();
  }
}

void Board::layCurrentPiece() {
  int pieceRowPos = currentPiece_->getRowPos();
  int pieceColPos = currentPiece_->getColPos();
  int width = currentPiece_->rotateFrameWidth();
  int thisRow, thisCol;
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
    if (i >= Board::HEIGHT || i < 0) continue;
    score_ += tryCollapseRow(i);
  }
  this->periodBetweenPieces_ = true;
}

int Board::tryCollapseRow(int _row) {
  // First, check if the collapse fails: if so, return
  for (int j = 0; j < Board::WIDTH; ++j) {
    if (board_.get(_row, j) == ' ') {
      return 0;
    }
  }
  // This annoying nested loop moves all the squares downwards
  int upperRow;
  for (int j = 0; j < Board::WIDTH; ++j) {
    for (int i = _row; i > 0; --i) {
      upperRow = i - 1;
      board_.set(i, j, board_.get(upperRow, j));
    }
  }
  // Make sure top row is now empty.
  for (int j = 0; j < Board::WIDTH; ++j) board_.set(0, j, ' ');
  return 1;
}

long Board::getFinalScore() const { return score_; }

// Private member functions

void Board::bringNextPieceUp() {
  currentPiece_ = nextPiece_;
  if (!currentPiece_->resetPiece()) {
    std::cout << "Board::bringNextPieceUp found that game is lost.\n";
    this->gameLost_ = true;
  }
}

void Board::generateNextPiece() {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<int> distribution(0, Board::NTYPES - 1);
  int randomInt = distribution(generator);
  nextPiece_ = pieces_ + randomInt;
}
