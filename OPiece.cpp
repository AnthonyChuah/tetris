#include "OPiece.h"

OPiece::OPiece(char* _board, int _row, int _col)
  : board_(_board), topLeftRowPos_(_row), topLeftColPos_(_col),
    rotateFrameSize_(2), orientation_(0),
{
  // Need static lazy initialization for orientMap_
  if (orientMap_.size() > 0) {
    for (int i = 0; i < 4; ++i) {
      orientMap_[i] = new char[4];
      for (int j = 0; j < 4; ++j)
	orientMap_[i][j] = 'o';
    }
  }
}

char OPiece::type() { return 'o'; }
