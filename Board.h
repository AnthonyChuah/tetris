#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "View.h"

/*
Think of the Board as the Model in the Model-View-Controller triad
It is the centre of the game logic
*/

class Board {
  static int MILLISECS_PER_TIMESTEP = 50;
  static int TIMESTEPS_AFTER_LAYING = 10;
  static int HEIGHT = 20;
  static int WIDTH = 10;
  long timesteps_; // Number of timesteps that have elapsed so far
  long score_; // Score
  int timestepsPerTick_; // 8 to start with, can increase difficulty by reducing this
  Piece* currentPiece_; // The current piece that is descending through the board
  Piece* nextPiece_; // The next piece after the current piece is done
  View* view_; // The View is responsible for the graphical display
 public:
  void timestep(int _command); // Each timestep, do the command entered by the player
  void tick(); // Each tick, the current piece descends one square
  void layCurrentPiece(); // This should do 3 steps:
  // 1. set the piece into its settled position
  // 2. check for rows completed: if any completed, collapse rows
  // 3. 
  int collapseRows(int _row1, int _row2, int _row3, int _row4);
  // Returns the score increment due to row collapse
  void getNextPiece(); // Get the next piece and make it the current piece
  // Remove the graphic from the right viewport
  void generateNextPiece(); // Randomly generate a next piece and update right viewport
};

#endif
