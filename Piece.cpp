#include "Piece.h"

#include <algorithm>

{
  using Piece::orientMap_;
  int numOrientations = 4;

  // First, create the orientation map for 'o' pieces
  int oPieceRFrameSize = 4; // rotation frame for the 'o' piece is 2 by 2, i.e. total 4 squares
  std::vector<char*> fourOrientArraysO(numOrientations);
  for (std::vector<char*>::const_iterator i : fourOrientArraysO) {
    (*i) = new char[oPieceRFrameSize];
    for (int j = 0; j < oPieceRFrameSize; ++j)
      (**i)[j] = 'o';
  }
  orientMap_.insert('o', fourOrientArraysO);
  // Explanation: the char array is a representation of which squares are filled in this
  // orientation. Since this is the 'o' piece, it is the simplest: all 4 squares are filled
  // in all orientations.

  // Next, create the orientation map for 'l' pieces
  int lPieceRFrameSize = 16; // 4 by 4 rotation frame
  std::vector<char*> fourOrientArraysL(numOrientations);
  for (std::vector<char*>::const_iterator i : fourOrientArraysL)
    (*i) = new char[lPieceRFrameSize];
  // Left orientation (0th orientation) is vertical, filling up the 1th column
  for (int i = 0; i < lPieceRFrameSize; ++i) fourOrientArraysL[0][i] = ' ';
  for (int i = 1; i <= 13; i += 4) fourOrientArraysL[0][i] = 'l';
  // Top orientation (1th) is horizontal, filling up the 1th row
  // Right orientation (2th) is vertical, filling up the 2th col
  // Bottom orientation (3th) is horizontal, filling up the 2th row
  
  // REMEMBER this needs destruction
  // Since this is static, the destructor for Piece MUST NOT be responsible
  // I designed the Board class to contain the Piece, a la Composition
  // So the Board can be responsible for destruction of Piece's static members
}

void Piece::shiftLeft() {
  // First, find leftmost blocks
  std::vector<int> leftMostCols = leftMostSquares();
  // Check if it will clash or go out of bounds if shifted one step left
  // If clash or out of bounds, do nothing (return)
  for (int i = 0; i < rotateFrameSize_; ++i) {
    // Now I am checking each row in the rotationFrame, and finding the location of that row's
    // leftmost point. After I get it, check if it either hits the edge, or clashes with a
    // laid piece.
    if (leftMostCols[i] < 0) continue; // that means for this row this piece has no square
    int rownum = topLeftRowPos_ + i;
    int colToCheckForCollide = topLeftColPos_ + leftMostCols[i] - 1;
    if (board_[rownum][colToCheckForCollide] > ' ' || colToCheckForCollide < 0)
      return;
  }
  // If not clash, move all 4 blocks left by decrementing topLeftColPos
  --topLeftColPos;
}

void Piece::shiftRight() {
  // Similar reasoning to shiftLeft
  std::vector<int> rightMostCols = rightMostSquares();
  for (int i = 0; i < rotateFrameSize_; ++i) {
    if (rightMostCols[i] < 0) continue;
    int rownum = topLeftRowPos_ + i;
    int colToCheckForCollide = topLeftColPos_ + rightMostCols[i] + 1;
    if (board_[rownum][colToCheckForCollide] > ' ' || colToCheckForCollide >= BOARDWIDTH)
      return;
  }
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
  int startOrientation = orientation_;
  --orientation_;
  if (orientation_ < 0) orientation_ = 3;
  // Now check if this causes any clash
  if (!checkForRotateCollision())
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
  std::vector<int> leftMosts(rotateFrameSize_);
  std::fill(leftMosts.begin(), leftMosts.end(), -1);
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
  std::vector<int> rightMosts(rotateFrameSize_);
  std::fill(rightMosts.begin(), rightMosts.end(), -1);
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
  std::vector<int> lowests(rotateFrameSize_);
  std::fill(lowests.begin(), lowests.end(), -1);
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
