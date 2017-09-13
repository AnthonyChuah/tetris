#ifndef GRID_H
#define GRID_H

#include <exception>
#include <iostream>

template <int Height, int Width>
class Grid {

  char data[Height][Width];

 public:

  void set(int _row, int _col, char _dat) {
    if (_row < 0 || _row >= Height || _col < 0 || _col >= Width) {
      std::cout << "Grid::set out of bounds.\n";
      std::cout << "Row: " << _row << " Col: " << _col << "\n";
      throw std::exception();
      // throw std::runtime_error("Grid::set out of bounds.");
    }
    data[_row][_col] = _dat;
  }
  char get(int _row, int _col) {
    if (_row < 0 || _row >= Height || _col < 0 || _col >= Width) {
      std::cout << "Grid::get out of bounds.\n";
      std::cout << "Row: " << _row << " Col: " << _col << "\n";
      throw std::exception();
      // throw std::runtime_error("Grid::get out of bounds.");
    }
    return data[_row][_col];
  }
  
};

#endif
