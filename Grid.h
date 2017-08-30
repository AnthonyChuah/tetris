#ifndef GRID_H
#define GRID_H

template <int Height, int Width>
class Grid {

  char data[Height][Width];

 public:

  void set(int _row, int _col, char _dat) {
    data[_row][_col] = _dat;
  }
  char get(int _row, int _col) {
    return data[_row][_col];
  }
  
};

#endif
