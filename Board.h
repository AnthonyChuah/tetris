#ifndef BOARD_H
#define BOARD_H

#include "Grid.h"
#include "Piece.h"

/*
Think of the Board as the Model in the Model-View-Controller triad
It is the centre of the game logic
*/

class Board {

  static constexpr int MILLISECS_PER_TIMESTEP = 50;
  static constexpr int HEIGHT = 20;
  static constexpr int WIDTH = 10;
  static constexpr int NTYPES = 7;
  long timesteps_ = 0; // Number of timesteps that have elapsed so far
  long score_ = 0; // Score
  int timeToNextTick_ = 8;
  int timestepsPerTick_ = 8; // 8 to start with, can increase difficulty by reducing this
  bool periodBetweenPieces_ = false; // There is a pause period between laying one piece
  // and spawning the next piece
  bool gameLost_ = false;
  Piece* currentPiece_; // The current piece that is descending through the board
  Piece* nextPiece_; // The next piece after the current piece is done
  Piece pieces_[NTYPES];
  // char board_[HEIGHT][WIDTH];
  Grid<char, HEIGHT, WIDTH> board_;
  char ntypes_[NTYPES];
  
  void bringNextPieceUp(); // Get the next piece and make it the current piece
  // Remove the graphic from the right viewport
  void generateNextPiece(); // Randomly generate a next piece and update right viewport
  
 public:
  
  Board(int _stepsPerTick);
  bool timestep(int _command); // Each timestep, do the command entered by the player
  void tick(); // Each tick, the current piece descends one square
  void layCurrentPiece(); // This should do 3 steps:
  // 1. set the piece into its settled position
  // 2. check for rows completed: if any completed, collapse rows
  // 3. get the next piece, make it the current piece, and put it into play
  // 4. generate the next piece
  int tryCollapseRow(int _row);
  // Returns the score increment due to row collapse
  long getFinalScore() const;

  friend class View; // The View needs to be able to see everything in Board for graphics
  friend class BoardTester; // To allow for unit tests

};

#endif
