#include "Piece.h"

#include <limits>
#include <algorithm>
#include <fstream>
#include <exception>

std::unordered_map<char, std::vector<char*> > Piece::orientMap_;
std::unordered_map<char, int> Piece::rotateFrameWidths_;
bool Piece::mapsInitialized = false;

Piece::Piece() : type_(' '), board_(nullptr) {}

void Piece::operator=(const Piece& _other) {
  topLeftRowPos_ = _other.topLeftRowPos_;
  topLeftColPos_ = _other.topLeftColPos_;
  orientation_ = _other.orientation_;
  type_ = _other.type_;
  rotateFrameWidth_ = _other.rotateFrameWidth_;
  rotateFrameSize_ = _other.rotateFrameSize_;
  board_ = _other.board_;
}

Piece::Piece(char _type, Grid<char, BOARDHEIGHT, BOARDWIDTH>* _board) : type_(_type), board_(_board) {
  if (!(Piece::mapsInitialized)) {
    throw std::logic_error("Piece was constructed before static initializations.");
  }
  rotateFrameWidth_ = Piece::rotateFrameWidths_[type_];
  rotateFrameSize_ = rotateFrameWidth_ * rotateFrameWidth_;
  resetPiece();
  // a 2-by-2 piece's left edge will be flush at col 4, but a 'l' piece will be displaced
  // Consider changing, but for now let's try this.
}

// I need an overloaded assignment operator
// By the Rule of 3, this means I should also define copy constructor and destructor

char Piece::type() const { return type_; }
int Piece::getOrient() const { return orientation_; }

bool Piece::checkIfRowColOccupied(int _row, int _col) const {
  int rowOffset = _row - topLeftRowPos_;
  int colOffset = _col - topLeftColPos_;
  if (rowOffset < 0 || rowOffset >= rotateFrameWidth_) return false;
  if (colOffset < 0 || colOffset >= rotateFrameWidth_) return false;
  int flatIndex = rowOffset * rotateFrameWidth_ + colOffset;
  char contentOfSquare = Piece::orientMap_[type_][orientation_][flatIndex];
  if (contentOfSquare == ' ') return false;
  return true;
}

int Piece::rotateFrameWidth() const { return rotateFrameWidth_; }

int Piece::getRowPos() const { return topLeftRowPos_; }

int Piece::getColPos() const { return topLeftColPos_; }

bool Piece::resetPiece() {
  orientation_ = 0; // Always start with orientation 0
  topLeftRowPos_ = 0;
  topLeftColPos_ = 3; // This means visually it may be jarring because it skews left
  if (rotateFrameWidth_ == 4) topLeftColPos_ = 2;
  if (checkForRotateCollision()) {
    // If resetting the Piece to the top clashes with the board,
    // then the game is over.
    return false;
  }
  return true;
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
    if (colToCheckForCollide < 0) return;
    if (board_->get(rownum, colToCheckForCollide) > ' ') return;
  }
  // If not clash, move all 4 blocks left by decrementing topLeftColPos
  --topLeftColPos_;
}

void Piece::shiftRight() {
  // Similar reasoning to shiftLeft
  std::vector<int> rightMostCols = rightMostSquares();
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    if (rightMostCols[i] < 0) continue;
    int rownum = topLeftRowPos_ + i;
    int colToCheckForCollide = topLeftColPos_ + rightMostCols[i] + 1;
    if (colToCheckForCollide >= Piece::BOARDWIDTH) return;
    if (board_->get(rownum, colToCheckForCollide) > ' ') return;
  }
  ++topLeftColPos_;
}

void Piece::tickDown() {
  // Piece is NOT RESPONSIBLE for checking if it should be laid
  // the Board should know if it is time to permanently lay this piece
  // So tickDown is called only when you know that there is space to move down
  // Think of shiftRight/Left as player-initiated, tickDown as tick-initiated
  // Since at each tick you should always check for permanent-layings, the tickDown
  // is merely a setter function to alter the position of the Piece
  ++topLeftRowPos_;
}

void Piece::dropToBottom() {
  // std::cout << "Piece::dropToBottom\n";
  std::vector<int> lowests = lowestSquares();
  std::vector<int> lowestRowPos(rotateFrameWidth_);
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
      if (board_->get(j, thisCol) > ' ') {
        thisFall = j - lowestRowPos[i] - 1;
	if (thisFall < shortestFall) shortestFall = thisFall;
        break;
      }
    }
  }
  this->topLeftRowPos_ += shortestFall;
}

bool Piece::checkCollideBelow() const {
  // std::cout << "Piece::checkCollideBelow\n";
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
    // std::cout << "lowestRowPos[i] is: " << lowestRowPos[i] << "\n";
    if (lowestRowPos[i] < 0) continue;
    thisCol = topLeftColPos_ + i;
    if (lowestRowPos[i] == Piece::BOARDHEIGHT - 1) return true; // At bottom-most
    // std::cout << "checkCollide went through to check for bricks\n";
    if (board_->get(lowestRowPos[i] + 1, thisCol) > ' ') return true; // Collided with brick below
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
  if (shiftDueToPastLeft && shiftDueToPastRight)
    throw std::runtime_error("Piece::rotateClock error");
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

bool Piece::checkForRotateCollision() const {
  // topLeftRowPos_ and topLeftColPos_ specify the current Piece's location
  int rFrameSize = rotateFrameSize_;
  for (int i = 0; i < rFrameSize; ++i) {
    int rowNum = i / rotateFrameWidth_;
    int colNum = i % rotateFrameWidth_;
    // Now iterate over each possible square, and check for collisions
    // board_ holds the whole board's laid bricks
    // topLeftRowPos_ and topLeftColPos_, and add rowNum/colNum, to get each square
    char contentOfSquareInBoard = board_->get(topLeftRowPos_ + rowNum, topLeftColPos_ + colNum);
    if (contentOfSquareInBoard > ' ' && Piece::orientMap_[type_][orientation_][i] > ' ') {
      // Then the piece is clashing with the board.
      return true;
    }
  }
  return false;
}

// Private member functions

bool Piece::checkIfHitBottom() const {
  // Get the lowest squares for this piece
  std::vector<int> lowests = this->lowestSquares();
  // Check if the lowest squares are already adjacent to any already-laid pieces,
  // OR if adjacent to the floor itself
  // mutate the vector so that it gives the rownum of each lowest square, UNLESS -1
  for (int i = 0; i < rotateFrameWidth_; ++i) {
    int colnum = topLeftColPos_ + i;
    if (lowests[i] < 0) continue;
    // Get the rownum of the lowest point for this col
    int rowToCheckForCollide = lowests[i] + topLeftRowPos_ + 1;
    if (board_->get(rowToCheckForCollide, colnum) > ' ') return true;
    if (rowToCheckForCollide >= Piece::BOARDHEIGHT) return true;
  }
  return false;
}

std::vector<int> Piece::leftMostSquares() const {
  std::vector<int> leftMosts(rotateFrameWidth_);
  std::fill(leftMosts.begin(), leftMosts.end(), -1);
  int rFrameVolume = rotateFrameWidth_ * rotateFrameWidth_;
  int rownum; int colnum;
  for (int i = rFrameVolume-1; i >= 0; --i) {
    rownum = i / rotateFrameWidth_;
    colnum = i % rotateFrameWidth_;
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
    rownum = i / rotateFrameWidth_;
    colnum = i % rotateFrameWidth_;
    if (Piece::orientMap_[type_][orientation_][i] > ' ') {
      if (lowests[colnum] < rownum) lowests[colnum] = rownum;
    }
  }
  return lowests;
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

// Static member functions

void Piece::populateLookupMaps() {
  // Populate the map of rotation frame sizes for each piece type
  int oPieceLen = 2; int oPieceRFrameSize = 4; // 4 = 2 * 2
  int lPieceLen = 4; int lPieceRFrameSize = 16; // 4 by 4 rotation frame
  int sPieceLen = 3; int sPieceRFrameSize = 9;
  int zPieceLen = 3; int zPieceRFrameSize = 9;
  int jPieceLen = 3; int jPieceRFrameSize = 9;
  int sevenPieceLen = 3; int sevenPieceRFrameSize = 9;
  int tPieceLen = 3; int tPieceRFrameSize = 9;
  Piece::rotateFrameWidths_['o'] = oPieceLen;
  Piece::rotateFrameWidths_['l'] = lPieceLen;
  Piece::rotateFrameWidths_['s'] = sPieceLen;
  Piece::rotateFrameWidths_['z'] = zPieceLen;
  Piece::rotateFrameWidths_['j'] = jPieceLen;
  Piece::rotateFrameWidths_['7'] = sevenPieceLen;
  Piece::rotateFrameWidths_['t'] = tPieceLen;
  int numOrientations = 4;
  std::ifstream ifs;

  // First, create the orientation map for 'o' pieces
  std::vector<char*> fourOrientArraysO(numOrientations);
  for (auto i = fourOrientArraysO.begin(); i != fourOrientArraysO.end(); ++i) {
    (*i) = new char[oPieceRFrameSize];
    for (int j = 0; j < oPieceRFrameSize; ++j)
      (*i)[j] = 'o';
  }
  Piece::orientMap_['o'] = fourOrientArraysO;
  // Explanation: the char array is a representation of which squares are filled in this
  // orientation. Since this is the 'o' piece, it is the simplest: all 4 squares are filled
  // in all orientations.

  // 'l' piece
  std::vector<char*> fourOrientArraysL(numOrientations);
  for (auto i = fourOrientArraysL.begin(); i != fourOrientArraysL.end(); ++i)
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
  Piece::orientMap_['l'] = fourOrientArraysL;
  
  // 's' piece
  std::vector<char*> fourOrientArraysS(numOrientations);
  for (auto i = fourOrientArraysS.begin(); i != fourOrientArraysS.end(); ++i)
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
  Piece::orientMap_['s'] = fourOrientArraysS;
  
  // 'z' piece
  std::vector<char*> fourOrientArraysZ(numOrientations);
  for (auto i = fourOrientArraysZ.begin(); i != fourOrientArraysZ.end(); ++i)
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
  Piece::orientMap_['z'] = fourOrientArraysZ;

  // 'j' piece
  std::vector<char*> fourOrientArraysJ(numOrientations);
  for (auto i = fourOrientArraysJ.begin(); i != fourOrientArraysJ.end(); ++i)
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
  Piece::orientMap_['j'] = fourOrientArraysJ;

  // '7' piece
  std::vector<char*> fourOrientArrays7(numOrientations);
  for (auto i = fourOrientArrays7.begin(); i != fourOrientArrays7.end(); ++i)
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
  Piece::orientMap_['7'] = fourOrientArrays7;

  // 't' piece
  std::vector<char*> fourOrientArraysT(numOrientations);
  for (auto i = fourOrientArraysT.begin(); i != fourOrientArraysT.end(); ++i)
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
  Piece::orientMap_['t'] = fourOrientArraysT;
  
  // REMEMBER that orientMap_ needs destruction!
  // Since this is static, the destructor for Piece MUST NOT be responsible
  // I designed the Board class to contain the Piece, a la Composition
  // So the Board can be responsible for destruction of Piece's static members
  Piece::mapsInitialized = true;
}

void Piece::destructStaticMaps() {
  for (auto it = Piece::orientMap_.begin(); it != Piece::orientMap_.end(); ++it) {
    int vectorLength = it->second.size();
    for (int i = 0; i < vectorLength; ++i) {
      delete [] it->second[i];
    }
  }
}
