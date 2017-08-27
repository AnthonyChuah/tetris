#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Piece {

  static constexpr int BOARDWIDTH = 10;
  static constexpr int BOARDHEIGHT = 20;
  static std::unordered_map<char, std::vector<char*> > orientMap_;
  static std::unordered_map<char, int> rotateFrameWidths_;

  int topLeftRowPos_;
  int topLeftColPos_;
  // rotation frame is always square, and its length is 2 or 3 or 4
  int orientation_; // left is 0, top is 1, right is 2, bottom is 3
  const char type_;
  int rotateFrameWidth_;
  int rotateFrameSize_; // Initialize from object constructor using rotateFrameSizes_ map
  char** board_; // pointer to the board so it can assess if a rotation or shift is illegal

  std::vector<int> leftMostSquares() const;
  std::vector<int> rightMostSquares() const;
  std::vector<int> lowestSquares() const;
  int shiftIfRotatePastLeftEdge() const;
  int shiftIfRotatePastRightEdge() const;

 public:

  Piece(char _type, char** _board);
  
  char type() const; // Returns the type of piece, e.g. the long piece is 'l'
  bool checkIfRowColOccupied(int _row, int _col) const;
  int rotateFrameWidth() const;
  int getRowPos() const;
  void resetPiece();
  void shiftLeft();
  void shiftRight();
  void tickDown();
  void dropToBottom();
  bool checkCollideBelow() const;
  bool rotateAnti();
  bool rotateClock();
  bool checkIfHitBottom() const;
  bool checkForRotateCollision() const;
  
  static void destructStaticMaps();
  
};

#endif
