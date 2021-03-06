#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <unordered_map>

#include "Grid.h"

class Piece {

  // std::vector of size 4, each element represents each orientation
  // the char* points to an array, of size rotateFrameSize_
  static std::unordered_map<char, std::vector<char*> > orientMap_;
  static std::unordered_map<char, int> rotateFrameWidths_;
  static bool mapsInitialized;

  int topLeftRowPos_;
  int topLeftColPos_;
  int orientation_; // left is 0, top is 1, right is 2, bottom is 3
  char type_;
  int rotateFrameWidth_; // rotation frame is always square, and its width is 2 or 3 or 4
  int rotateFrameSize_; // It's the area, i.e. 2-by-2 has size 4, width 2

  std::vector<int> leftMostSquares() const;
  std::vector<int> rightMostSquares() const;
  std::vector<int> lowestSquares() const;
  int shiftIfRotatePastLeftEdge() const;
  int shiftIfRotatePastRightEdge() const;
  bool checkIfHitBottom() const;

 public:

  static constexpr int BOARDWIDTH = 10;
  static constexpr int BOARDHEIGHT = 20;

  Grid<char, BOARDHEIGHT, BOARDWIDTH>* board_;

  Piece();
  Piece(char _type, Grid<char, BOARDHEIGHT, BOARDWIDTH>* _board);
  void operator=(const Piece& _other);
  
  char type() const; // Returns the type of piece, e.g. the long piece is 'l'
  int getOrient() const;
  bool checkIfRowColOccupied(int _row, int _col) const;
  int rotateFrameWidth() const;
  int getRowPos() const;
  int getColPos() const;
  bool resetPiece(); // If it returns false, GAME OVER
  void shiftLeft();
  void shiftRight();
  void tickDown();
  void dropToBottom();
  bool checkCollideBelow() const;
  bool rotateAnti();
  bool rotateClock();
  bool checkForRotateCollision() const;

  static void populateLookupMaps();
  static void destructStaticMaps();

  friend class View;
  
};

#endif
