#include "Piece.h"

#include <limits>
#include <algorithm>
#include <fstream>
#include <iostream>

{
  // Populate the map of rotation frame sizes for each piece type
  int oPieceLen = 2; int oPieceRFrameSize = 4; // 4 = 2 * 2
  int lPieceLen = 4; int lPieceRFrameSize = 16; // 4 by 4 rotation frame
  int sPieceLen = 3; int sPieceRFrameSize = 9;
  int zPieceLen = 3; int zPieceRFrameSize = 9;
  int jPieceLen = 3; int jPieceRFrameSize = 9;
  int sevenPieceLen = 3; int sevenPieceRFrameSize = 9;
  int tPieceLen = 3; int tPieceRFrameSize = 9;
  Piece::rotateFrameWidths_.insert('o', oPieceLen);
  Piece::rotateFrameWidths_.insert('l', lPieceLen);
  Piece::rotateFrameWidths_.insert('s', sPieceLen);
  Piece::rotateFrameWidths_.insert('z', zPieceLen);
  Piece::rotateFrameWidths_.insert('j', jPieceLen);
  Piece::rotateFrameWidths_.insert('7', sevenPieceLen);
  Piece::rotateFrameWidths_.insert('t', tPieceLen);
  
  int numOrientations = 4;
  std::ifstream ifs;

  // First, create the orientation map for 'o' pieces
  std::vector<char*> fourOrientArraysO(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArraysO) {
    (*i) = new char[oPieceRFrameSize];
    for (int j = 0; j < oPieceRFrameSize; ++j)
      (**i)[j] = 'o';
  }
  Piece::orientMap_.insert('o', fourOrientArraysO);
  // Explanation: the char array is a representation of which squares are filled in this
  // orientation. Since this is the 'o' piece, it is the simplest: all 4 squares are filled
  // in all orientations.

  // 'l' piece
  std::vector<char*> fourOrientArraysL(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArraysL)
    (*i) = new char[lPieceRFrameSize];
  ifs.open("pieceL.dat");
  {
    int n;
    for (int i = 0; i < numOrientations; ++i) {
      for (int j = 0; j < lPieceLen; ++j) {
	for (int k = 0; k < lPieceLen; ++k) {
	  n = j * lPieceLen + k;
	  ifs.get(fourOrientArraysL[i][n]);
	}
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  ifs.close();
  Piece::orientMap_.insert('l', fourOrientArraysL);

  // 's' piece
  std::vector<char*> fourOrientArraysS(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArraysS)
    (*i) = new char[sPieceRFrameSize];
  ifs.open("pieceS.dat");
  {
    int n;
    for (int i = 0; i < numOrientations; ++i) {
      for (int j = 0; j < sPieceLen; ++j) {
	for (int k = 0; k < sPieceLen; ++k) {
	  n = j * sPieceLen + k;
	  ifs.get(fourOrientArraysS[i][n]);
	}
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  ifs.close();
  Piece::orientMap_.insert('s', fourOrientArraysS);

  // 'z' piece
  std::vector<char*> fourOrientArraysZ(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArraysZ)
    (*i) = new char[zPieceRFrameSize];
  ifs.open("pieceZ.dat");
  {
    int n;
    for (int i = 0; i < numOrientations; ++i) {
      for (int j = 0; j < zPieceLen; ++j) {
	for (int k = 0; k < zPieceLen; ++k) {
	  n = j * zPieceLen + k;
	  ifs.get(fourOrientArraysZ[i][n]);
	}
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  ifs.close();
  Piece::orientMap_.insert('z', fourOrientArraysZ);

  // 'j' piece
  std::vector<char*> fourOrientArraysJ(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArraysJ)
    (*i) = new char[jPieceRFrameSize];
  ifs.open("pieceJ.dat");
  {
    int n;
    for (int i = 0; i < numOrientations; ++i) {
      for (int j = 0; j < jPieceLen; ++j) {
	for (int k = 0; k < jPieceLen; ++k) {
	  n = j * jPieceLen + k;
	  ifs.get(fourOrientArraysJ[i][n]);
	}
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }    
  }
  ifs.close();
  Piece::orientMap_.insert('j', fourOrientArraysJ);

  // '7' piece
  std::vector<char*> fourOrientArrays7(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArrays7)
    (*i) = new char[sevenPieceRFrameSize];
  ifs.open("piece7.dat");
  {
    int n;
    for (int i = 0; i < numOrientations; ++i) {
      for (int j = 0; j < sevenPieceLen; ++j) {
	for (int k = 0; k < sevenPieceLen; ++k) {
	  n = j * sevenPieceLen + k;
	  ifs.get(fourOrientArrays7[i][n]);
	}
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  ifs.close();
  Piece::orientMap_.insert('7', fourOrientArrays7);

  // 't' piece
  std::vector<char*> fourOrientArraysT(numOrientations);
  for (std::vector<char*>::iterator i : fourOrientArraysT)
    (*i) = new char[tPieceRFrameSize];
  ifs.open("pieceT.dat");
  {
    int n;
    for (int i = 0; i < numOrientations; ++i) {
      for (int j = 0; j < tPieceLen; ++j) {
	for (int k = 0; k < tPieceLen; ++k) {
	  n = j * tPieceLen + k;
	  ifs.get(fourOrientArraysT[i][n]);
	}
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
  ifs.close();
  Piece::orientMap_.insert('t', fourOrientArraysT);
  
  // REMEMBER that orientMap_ needs destruction!
  // Since this is static, the destructor for Piece MUST NOT be responsible
  // I designed the Board class to contain the Piece, a la Composition
  // So the Board can be responsible for destruction of Piece's static members
}

Piece::Piece(char _type, char** _board) : type_(_type) {
  rotateFrameWidth_ = Piece::rotateFrameWidths_[type_];
  rotateFrameSize_ = rotateFrameWidth_ * rotateFrameWidth_;
  board_ = _board;
  resetPiece();
  // a 2-by-2 piece's left edge will be flush at col 4, but a 'l' piece will be displaced
  // Consider changing, but for now let's try this.
}

char Piece::type() const { return type_; }

bool Piece::checkIfRowColOccupied(int _row, int _col) const {
  int rowOffset = _row - topLeftRowPos_;
  int colOffset = _col - topLeftColPos_;
  if (rowOffset < 0 || rowOffset >= rotateFrameWidth_) return false;
  if (colOffset < 0 || colOffset >= rotateFrameWidth_) return false;
  int flatIndex = rowOffset * rotateFrameWidth_ + colOffset;
  char contentOfSquare = Piece::orientMap_[_type][_orient][flatIndex];
  if (contentOfSquare == ' ') return false;
  return true;
}

int Piece::rotateFrameWidth() const { return rotateFrameWidth_; }

int Piece::getRowPos() const { return topLeftRowPos_; }

void Piece::resetPiece() {
  orientation_ = 0; // Always start with orientation 0
  topLeftRowPos_ = 0;
  topLeftColPos_ = 3; // This means visually it may be jarring because
  if (rotateFrameWidth == 4) topLeftColPos_ = 2;
}

void Piece::shiftLeft() {
  // First, find leftmost blocks
  std::vector<int> leftMostCols = leftMostSquares();
  // Check if it will clash or go out of bounds if shifted one step left
  // If clash or out of bounds, do nothing (return)
  for (int i = 0; i < rotateFrameWidth_; ++i) {
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
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (rightMostCols[i] < 0) continue;
    int rownum = topLeftRowPos_ + i;
    int colToCheckForCollide = topLeftColPos_ + rightMostCols[i] + 1;
    if (board_[rownum][colToCheckForCollide] > ' ' || colToCheckForCollide >= Piece::BOARDWIDTH)
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

void Piece::dropToBottom() {
  std::vector<int> lowests = lowestSquares();
  std::vector<int> lowestRowPos = lowests;
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (lowests[i] < 0) {
      lowestRowPos[i] = -1;
      continue;
    }
    lowestRowPos[i] = topLeftRowPos_ + lowests[i];
  }
  int shortestFall = Piece::BOARDHEIGHT;
  int thisFall;
  int thisCol;
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (lowestRowPos[i] < 0) continue;
    thisCol = topLeftColPos_ + i;
    for (int j = lowestRowPos[i] + 1; j <= Piece::BOARDHEIGHT; ++j) {
      if (j == Piece::BOARDHEIGHT) {
	thisFall = Piece::BOARDHEIGHT - lowestRowPos[i] - 1;
	if (thisFall < shortestFall) shortestFall = thisFall;
	break;
      }
      if (board_[j][thisCol] > ' ') {
        thisFall = j - lowestRowPos[i] - 1;
	if (thisFall < shortestFall) shortestFall = thisFall;
        break;
      }
    }
  }
  this->topLeftRowPos_ -= shortestFall;
}

bool Piece::checkCollideBelow() const {
  std::vector<int> lowests = lowestSquares();
  std::vector<int> lowestRowPos = lowests;
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (lowests[i] < 0) {
      lowestRowPos[i] = -1;
      continue;
    }
    lowestRowPos[i] = topLeftRowPos_ + lowests[i];
  }
  int thisCol;
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (lowestRowPos[i] < 0) continue;
    thisCol = topLeftColPos_ + i;
    if (lowestRowPos[i] == Piece::BOARDHEIGHT - 1) return true; // At bottom-most
    if (board_[lowestRowPos[i] + 1][thisCol] > ' ') return true; // Collided with brick below
  }
  return false;
}

bool Piece::rotateAnti() {
  int startOrientation = orientation_;
  int startColPos = topLeftColPos_;
  --orientation_;
  if (orientation_ < 0) orientation_ = 3;
  // Now check if this causes any clash
  // First see if this moves us out of left or right bounds. If so, shift accordingly
  int shiftDueToPastRight, shiftDueToPastLeft;
  shiftDueToPastLeft = shiftIfRotatePastLeftEdge();
  shiftDueToPastRight = shiftIfRotatePastRightEdge();
  topLeftColPos_ += shiftDueToPastLeft;
  topLeftColPos_ -= shiftDueToPastRight;
  if (checkForRotateCollision()) {
    orientation_ = startOrientation;
    topLeftColPos_ = startColPos;
    return false;
  }
  if (checkIfHitBottom()) {
    orientation_ = startOrientation;
    topLeftColPos_ = startColPos;
    return false;
  }
  return true;
}

bool Piece::rotateClock() {
  int startOrientation = orientation_;
  int startColPos = topLeftColPos_;
  ++orientation_;
  if (orientation_ > 3) orientation_ = 0;
  int shiftDueToPastRight, shiftDueToPastLeft;
  shiftDueToPastLeft = shiftIfRotatePastLeftEdge();
  shiftDueToPastRight = shiftIfRotatePastRightEdge();
  if (shiftDueToPastLeft && shiftDueToPastRight) throw std::exception();
  topLeftColPos_ += shiftDueToPastLeft;
  topLeftColPos_ -= shiftDueToPastRight;
  if (checkForRotationCollision()) {
    orientation_ = startOrientation;
    topLeftColPos_ = startColPos;
    return false;
  }
  if (checkIfHitBottom()) {
    orientation_ = startOrientation;
    topLeftColPos_ = startColPos;
    return false;
  }
  return true;
}

bool Piece::checkIfHitBottom() const {
  // Get the lowest squares for this piece
  std::vector<int> lowests = this->lowestSquares();
  // Check if the lowest squares are already adjacent to any already-laid pieces,
  // OR if adjacent to the floor itself
  // mutate the vector so that it gives the rownum of each lowest square, UNLESS -1
  for (int i = 0; i < lowests.size(); ++i) {
    int colnum = topLeftColPos_ + i;
    if (lowests[i] < 0) continue;
    // Get the rownum of the lowest point for this col
    int rowToCheckForCollide = lowests[i] + topLeftRowPos_ + 1;
    if (board_[rowToCheckForCollide][colnum] > ' ') return true;
    if (rowToCheckForCollide >= Piece::BOARDHEIGHT) return true;
  }
  return false;
}

bool Piece::checkForRotateCollision() const {
  // topLeftRowPos_ and topLeftColPos_ specify the current Piece's location
  int rFrameSize = rotateFrameSize_;
  int rFrameWidth = rotateFrameWidth_;
  for (int i = 0; i < rFrameSize; ++i) {
    int rowNum = i / rotateFrameWidth_;
    int colNum = i % rotateFrameWidth_;
    // Now iterate over each possible square, and check for collisions
    // board_ holds the whole board's laid bricks
    // topLeftRowPos_ and topLeftColPos_, and add rowNum/colNum, to get each square
    char contentOfSquareInBoard = board_[topLeftRowPos_ + rowNum][topLeftColPos_ + colNum];
    if (contentOfSquareInBoard > ' ' && Piece::orientMap_[_type][_orient][i] > ' ') {
      // Then the piece is clashing with the board.
      return true;
    }
  }
  return false;
}

std::vector<int> Piece::leftMostSquares() const {
  std::vector<int> leftMosts(rotateFrameWidth_);
  std::fill(leftMosts.begin(), leftMosts.end(), -1);
  int rFrameVolume = rotateFrameWidth_ * rotateFrameWidth_;
  int rownum; int colnum;
  for (int i = rFrameVolume-1; i >= 0; --i) {
    rownum = i / rotateFrameWidth;
    colnum = i % rotateFrameWidth;
    if (Piece::orientMap_[type_][orientation_][i] > ' ') {
      // That means that square is non-empty
      leftMosts[rownum] = colnum;
      // Since iterating backwards, the final one is always leftmost
    }
  }
  return leftMosts;
}

std::vector<int> Piece::rightMostSquares() const {
  std::vector<int> rightMosts(rotateFrameWidth_);
  std::fill(rightMosts.begin(), rightMosts.end(), -1);
  int rFrameVolume = rotateFrameWidth_ * rotateFrameWidth_;
  int rownum, colnum;
  for (int i = 0; i < rFrameVolume; ++i) {
    rownum = i / rotateFrameWidth_;
    colnum = i % rotateFrameWidth_;
    if (Piece::orientMap_[type_][orientation_][i] > ' ') {
      rightMosts[rownum] = colnum; // Iterating forwards, final one is rightmost
    }
  }
  return rightMosts;
}

std::vector<int> Piece::lowestSquares() const {
  std::vector<int> lowests(rotateFrameWidth_);
  std::fill(lowests.begin(), lowests.end(), -1);
  int rFrameVolume = rotateFrameWidth_ * rotateFrameWidth_;
  int rownum, colnum;
  for (int i = 0; i < rFrameVolume; ++i) {
    rownum = i / rotateFrameWidth;
    colnum = i % rotateFrameWidth;
    if (Piece::orientMap_[type_][orientation_][i] > ' ') {
      if (lowests[colnum] < rownum) lowests[colnum] = rownum;
    }
  }
}

int Piece::shiftIfRotatePastLeftEdge() const {
  // Gives the number of columns to shift by if your rotation puts piece past the edge
  // If no shift necessary, returns 0
  // Worst-case: 'l' piece rotates anti-wise and its leftmost point is 2 steps leftwards
  // Therefore, the only place you need to start checking is when the topLeftColPos_ is
  // either 0 or 1.
  std::vector<int> leftMostCols = leftMostSquares();
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (leftMostCols[i] < 0) continue;
    int leftTipColPos = topLeftColPos_ + leftMostCols[i];
    if (leftTipColPos < 0) {
      return (-leftTipColPos);
    }
  }
  return 0;
}

int Piece::shiftIfRotatePastRightEdge() const {
  // Same as the function above, just reduces checking burden to separate the functions
  // Worst-case: 'l' piece rotates clockwise and its rightmost point becomes 2 steps rightwards
  // Rotation frame of 'l' piece is also the biggest possible: 4
  // Therefore, the only place you need to start checking is when the topLeftColPos_ is
  // WIDTH - 2 (topLeftColPos_ can never be greater than this!)
  std::vector<int> rightMostCols = rightMostSquares();
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (rightMostCols[i] < 0) continue;
    int rightTipColPos = topLeftColPos_ + rightMostCols[i];
    if (rightTipColPos >= Piece::BOARDWIDTH) {
      return (rightTipColPos - Piece::BOARDWIDTH);
    }
  }
  return 0;
}

void Piece::destructStaticMaps() {
  for (auto it = Piece::orientMap_.begin(); it != Piece::orientMap_.end(); ++it) {
    int vectorLength = it->second.length();
    for (int i = 0; i < vectorLength; ++i) {
      delete [] it->second[i];
    }
  }
}
