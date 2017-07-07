#include "Piece.h"

// All these below may belong to Piece.cpp instead!

void Piece::shiftLeft() {
  // First, find leftmost blocks
  std::vector<int> leftMostCols = leftMostSquares();
  // Check if it will clash or go out of bounds if shifted one step left
  // If clash or out of bounds, do nothing (return)
  for (int i = 0; i < rotateFrameSize_; ++i) {
    // Now I am checking each row in the rotationFrame, and finding the location of that row's
    // leftmost point. After I get it, check if it either hits the edge, or clashes with a
    // laid piece.
    if (leftMostCols[i] < 0) continue;
    int rownum = topLeftRowPos_ + i;
    int colToCheckForCollide = topLeftColPos_ + leftMostCols[i];
  }
  // If not clash, move all 4 blocks left by decrementing topLeftColPos
  --topLeftColPos;
}

void Piece::shiftRight() {
  // Similar reasoning to shiftLeft
  ++topLeftColPos;
}

void Piece::tickDown() {
  // Piece is NOT RESPONSIBLE for checking if it should be laid
  // the Board should know if it is time to permanently lay this piece
  // So tickDown is called only when you know that there is space to move down
  // Think of shiftRight/Left as player-initiated, tickDown as tick-initiated
  // Since at each tick you should always check for permanent-layings, the tickDown
  // is merely a setter function to alter the position of the Piece
  --topLeftRowPos_;
}

void Piece::rotateAnti() {
  --orientation_;
  if (orientation_ < 0) orientation_ = 3;
}

void Piece::rotateClock() {
  ++orientation_;
  if (orientation_ > 3) orientation_ = 0;
}

bool Piece::checkForRotateCollision() {
}

int Piece::shiftIfRotatePastLeftEdge() {
  // Gives the number of columns to shift by if your rotation puts piece past the edge
  // If no shift necessary, returns 0
  // Worst-case: 'l' piece rotates anti-wise and its leftmost point is 2 steps leftwards
  // Therefore, the only place you need to start checking is when the topLeftColPos_ is
  // either 0 or 1. This function should only ever be called when that is the case.
}

int Piece::shiftIfRotatePastRightEdge() {
  // Same as the function above, just reduces checking burden to separate the functions
  // Worst-case: 'l' piece rotates clockwise and its rightmost point becomes 2 steps rightwards
  // Rotation frame of 'l' piece is also the biggest possible: 4
  // Therefore, the only place you need to start checking is when the topLeftColPos_ is
  // WIDTH - 2 (topLeftColPos_ can never be greater than this!)
}

bool Piece::checkIfHitBottom() {
  // Get the lowest squares for this piece
  std::vector lowests = this->lowestSquares();
  // Check if the lowest squares are already adjacent to any already-laid pieces,
  // OR if adjacent to the floor itself
  // mutate the vector so that it gives the rownum of each lowest square, UNLESS -1
  for (int i = 0; i < lowests.size(); ++i) {
    int colnum = topLeftColPos_ + i;
    if (lowests[i] < 0) continue;
    // Get the rownum of the lowest point for this col
    int rowToCheckForCollide = lowests[i] + topLeftRowPos_ + 1;
    if (board_[rowToCheckForCollide][colnum] > ' ') return true;
    if (rowToCheckForCollide == BOARDHEIGHT) return true;
  }
  return false;
}

std::vector<int> leftMostSquares() {
  std::vector<int> leftMosts;
  for (int i = 0; i < rotateFrameSize; ++i) {
    // Initialize leftMosts to -1, necessary for some shapes where certain rows are empty
    // when oriented in certain ways
    leftMosts.push_back(-1);
  }
  int rFrameVolume = rotateFrameSize_ * rotateFrameSize_;
  int rownum; int colnum;
  for (int i = rFrameVolume-1; i >= 0; --i) {
    rownum = i / rotateFrameSize;
    colnum = i % rotateFrameSize;
    if (orientMap_[orientation_][i] > ' ') {
      // That means that square is non-empty
      leftMosts[rownum] = colnum;
      // Since iterating backwards, the final one is always leftmost
    }
  }
  return leftMosts;
}

std::vector<int> rightMostSquares() {
  std::vector<int> rightMosts;
  for (int i = 0; i < rotateFrameSize; ++i) rightMosts.push_back(-1);
  int rFrameVolume = rotateFrameSize_ * rotateFrameSize_;
  int rownum, colnum;
  for (int i = 0; i < rFrameVolume; ++i) {
    rownum = i / rotateFrameSize;
    colnum = i % rotateFrameSize;
    if (orientMap_[orientation_][i] > ' ') {
      rightMosts[rownum] = colnum; // Iterating forwards, final one is rightmost
    }
  }
  return rightMosts;
}

std::vector<int> lowestSquares() {
  std::vector<int> lowests;
  // CONSIDER using a range-based operation on the container instead
  // more efficient than looping
  for (int i = 0; i < rotateFrameSize; ++i) lowests.push_back(-1);
  int rFrameVolume = rotateFrameSize_ * rotateFrameSize_;
  int rownum, colnum;
  for (int i = 0; i < rFrameVolume; ++i) {
    rownum = i / rotateFrameSize;
    colnum = i % rotateFrameSize;
    if (orientMap_[orientation_][i] > ' ') {
      if (lowests[colnum] < rownum) lowests[colnum] = rownum;
    }
  }
}
