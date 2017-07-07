#ifndef OPIECE_H
#define OPIECE_H

#include <unordered_map>

class OPiece : public Piece {
  static std::unordered_map<int, char*> orientMap_;
 public:
  OPiece(char* _board, int _row, int _col);
  virtual char type();
};

#endif
