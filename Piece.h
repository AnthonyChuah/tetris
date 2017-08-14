#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Piece {

  static constexpr int BOARDWIDTH = 10;
  static constexpr int BOARDHEIGHT = 20;
  int topLeftRowPos_;
  int topLeftColPos_;
  // rotation frame is always square, and its length is 2 or 3 or 4
  int orientation_; // left is 0, top is 1, right is 2, bottom is 3
  const char type_;
  const int rotateFrameSize_; // Initialize from object constructor using rotateFrameSizes_ map
  static std::unordered_map<char, std::vector<char*> > orientMap_;
  static std::unordered_map<char, int> rotateFrameSizes_;
  char* board_; // pointer to the board so it can assess if a rotation or shift is illegal
  
  Piece();
  ~Piece(); // no need virtual destructor because subclass does not have extra data

  std::vector<int> leftMostSquares();
  std::vector<int> rightMostSquares();
  std::vector<int> lowestSquares();
  
 public:
  
  char type(); // Returns the type of piece, e.g. the long piece is 'l'
  int rotateFrameSize(); // returns the size of the rotation frame
  void shiftLeft();
  void shiftRight();
  void tickDown();
  void rotateAnti();
  void rotateClock();
  bool checkIfHitBottom();
  void destructStaticMaps();
  
};

#endif
