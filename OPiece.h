#ifndef OPIECE_H
#define OPIECE_H

class OPiece : public Piece {
 public:
  OPiece(char* _board, int _row, int _col);
  virtual char type();
};

#endif
