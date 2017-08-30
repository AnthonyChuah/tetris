#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <unordered_map>

#include "Grid.h"

class Piece {

  static std::unordered_map<char, std::vector<char*> > orientMap_;
  static std::unordered_map<char, int> rotateFrameWidths_;
  static bool mapsInitialized;

  int topLeftRowPos_;
  int topLeftColPos_;
  // rotation frame is always square, and its length is 2 or 3 or 4
  int orientation_; // left is 0, top is 1, right is 2, bottom is 3
  const char type_;
  int rotateFrameWidth_;
  int rotateFrameSize_; // It's the volume, i.e. 2-by-2 has size 4, width 2

  std::vector<int> leftMostSquares() const;
  std::vector<int> rightMostSquares() const;
  std::vector<int> lowestSquares() const;
  int shiftIfRotatePastLeftEdge() const;
  int shiftIfRotatePastRightEdge() const;

 public:

  static constexpr int BOARDWIDTH = 10;
  static constexpr int BOARDHEIGHT = 20;

  Grid<BOARDHEIGHT, BOARDWIDTH>& board_;

  Piece(char _type, Grid<BOARDHEIGHT, BOARDWIDTH>& _board);
  
  char type() const; // Returns the type of piece, e.g. the long piece is 'l'
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
  bool checkIfHitBottom() const;
  bool checkForRotateCollision() const;

  static void populateLookupMaps();
  static void destructStaticMaps();
  
};

#endif
