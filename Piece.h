#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Piece {
 protected:

  static constexpr int BOARDWIDTH = 10;
  static constexpr int BOARDHEIGHT = 20;
  int topLeftRowPos_;
  int topLeftColPos_;
  const int rotateFrameSize_;
  // rotation frame is always square, and its length is 2 or 3 or 4
  int orientation_; // left is 0, top is 1, right is 2, bottom is 3
  static std::unordered_map<char, std::vector<char*> > orientMap_;
  char* board_; // pointer to the board so it can assess if a rotation or shift is illegal
  
  Piece();
  ~Piece(); // no need virtual destructor because subclass does not have extra data

  std::vector<int> leftMostSquares();
  std::vector<int> rightMostSquares();
  std::vector<int> lowestSquares();
  
 public:
  
  virtual char type() = 0; // Returns the type of piece, e.g. the long piece is 'l'
  void shiftLeft();
  void shiftRight();
  void tickDown();
  void rotateAnti();
  void rotateClock();
  bool checkIfHitBottom();
  void destructStaticMaps();
  
};

#endif
