#ifndef GAME_H
#define GAME_H

#include <mutex>
#include <condition_variable>
#include <SDL2/SDL.h>

#include "CircularBuffer.h"
#include "Board.h"
#include "View.h"

/*
The Game is the container for everything: it holds the Board and View
It serves the functionality of the Controller in the Model-View-Controller triad
The Model is the Board, the View is View
It coordinates and launches the threads
It governs the timers and instructs the Board to proceed ("timestep")
After the Model updates from the timestep, the View renders
The clock thread ensures that every timestep happens per >= 50 ms

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

  static constexpr int TIMESTEPS_PER_TICK = 8;
  static constexpr int MILLISECS_PER_TIMESTEP = 50;
  static constexpr int COMMAND_BUFFER = 64;
  static constexpr int CMD_LEFT = 1;
  static constexpr int CMD_RIGHT = 2;
  static constexpr int CMD_ANTI = 3;
  static constexpr int CMD_CLOCK = 4;
  static constexpr int CMD_DOWN = 5;
  static constexpr int CMD_DROP = 6;
  Board board_;
  View view_;
  int nextCommand_ = 0;
  bool isRunning = false;
  std::mutex mutexClock;
  std::condition_variable condvarClock;
  CircularBuffer<int, COMMAND_BUFFER> cmdQueue_;
  // This FIFO queue holds all the commands the player inputs
  // At each timestep, one command is popped and processed

  void updateModelForThisFrame();
  
 public:
  
  Game(); // default constructor override
  void launchAllThreads(); // calls all 3 launch functions below
  void launchControlThread(); // listens for user input
  void launchGameThread(); // updates the Model, then the View renders graphics
  void launchClockThread(); // the clock gives the illusion of smooth real-time playing
  // WARNING: if processing takes longer than the timestep (50 ms) you could skip
  // a timestep. This should never happen on a normal computer
  
};

#endif
