#ifndef IPIECE_H
#define IPIECE_H

/*
The I shaped piece, a 4-by-1 piece that can be vertical or horizontally laid
Rotation Frame: 4-by-4

Possible orientations are: left-oriented vertical (2nd column),
top-oriented horizontal (2nd row), right-oriented vertical (3rd column),
and bottom-oriented horizontal (3rd row).

Whenever a Rotation would cause a conflict with an existing already-laid piece,
or with the bottom of the game, the Rotation must not happen.

Whenever a Rotation would cause some physical part of the piece to extend beyond
the rightmost or leftmost confines, the piece should be shifted to fit inside the confines.
However if the shift causes conflict with an existing already-laid piece, cancel Rotation.
*/

class IPiece : public Piece {
  
 public:
  
};

#endif
