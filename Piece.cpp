#include "Piece.h"

// All these below may belong to Piece.cpp instead!

void Piece::shiftLeft() {
  // First, find leftmost blocks
  // Check if it will clash or go out of bounds if shifted one step left
  // If clash, do nothing
  // If not clash, move all 4 blocks left by setting topLeftColPos
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

bool Piece::checkIfHitBottom() {
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
  for (int i = 0; i < rotateFrameSize; ++i) lowests.push_back(-1);
  int rFrameVolume = rotateFrameSize_ * rotateFrameSize_;
  int rownum, colnum;
}
