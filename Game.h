#ifndef GAME_H
#define GAME_h

#include "CircularBuffer.h"

/*
The Game is the container for everything: it holds the Board and View
It serves the functionality of the Controller in the Model-View-Controller triad
It coordinates and launches the threads
It governs the timers and instructs the Board to proceed ("tick")
Commands:
0: no command, set by default
1: shift left (LEFT)
2: shift right (RIGHT)
3. rotate anticlockwise (t)
4. rotate clockwise (r)
5. speed up descent of piece (DOWN)
6. drop piece down to bottom immediately (SPACE)
*/

class Game {
  
  static constexpr int MILLISECS_PER_TIMESTEP = 50; // 20 frames per second
  static constexpr int COMMAND_BUFFER = 64;
  View view_;
  Board board_;
  int nextCommand_ = 0;
  CircularBuffer<int, COMMAND_BUFFER> cmdQueue_;
  
 public:
  
  Game(); // default constructor override
  void launchAllThreads(); // calls all 3 launch functions below
  void launchControllerThread(); // polls indefinitely to accept user input
  // whenever user makes an input, this is buffered for the next timestep, and all other
  // inputs are ignored until that timestep is processed
  void launchGameAndViewThread(); // unlike the Controller thread, does not poll
  // instead, it blocks waiting until a timestep happens (Clock thread notifies)
  // when unblocked, it figures out what the user's command since the last timestep was
  // with that command, it executes the change to the game
  // then once the game is changed, it updates the View (graphics)
  // NOTE: could be worth separating Game and View threads if in danger of lagging
  // graphics may lag but the game logic must not
  void launchClockThread(); // the clock gives the illusion of smooth real-time playing
  // WARNING: if processing takes longer than the timestep (50 ms) you could skip a timestep
  // this should never happen on a normal computer however
  void launchGameThread(); // does Model/View/Controller all together
  
};

#endif
