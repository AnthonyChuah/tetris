#ifndef GRID_H
#define GRID_H

#include <exception>
#include <iostream>

template <typename T, int Height, int Width>
class Grid {

  T data[Height][Width];

 public:

  void set(int _row, int _col, T _dat) {
    if (_row < 0 || _row >= Height || _col < 0 || _col >= Width) {
      std::cout << "Row: " << _row << " Col: " << _col << "\n";
      std::cout << "Grid::set out of bounds error.\n";
      throw std::exception();
    }
    data[_row][_col] = _dat;
  }
  T get(int _row, int _col) {
    if (_row < 0 || _row >= Height || _col < 0 || _col >= Width) {
      std::cout << "Row: " << _row << " Col: " << _col << "\n";
      std::cout << "Grid::get out of bounds error.\n";
      throw std::exception();
    }
    return data[_row][_col];
  }
  
};

#endif
