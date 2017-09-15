#include "Game.h"

#include <thread>
#include <chrono>
#include <iostream>

/* Stuff to Google
   If one thread sleeps for a second, does it actually wake after exactly 1 second
   when other threads are processing?
 */

Game::Game() : board_(Game::TIMESTEPS_PER_TICK), view_(&board_) {}

void Game::launchAllThreads() {
  this->isRunning = true;
  std::cout << "Welcome to tetris! The controls are:\n"
	    << "LEFT key: shift piece left\n"
	    << "RIGHT key: shift piece right\n"
	    << "q key: rotate piece anti-clockwise\n"
	    << "r key: rotate piece clockwise\n"
	    << "DOWN key: shift piece down\n"
	    << "SPACE key: drop piece to bottom\n"
	    << "Good luck and have fun!\n";
  std::this_thread::sleep_for(chrono::milliseconds(1000));
  std::thread controlThread(this->launchControlThread);
  std::thread gameThread(this->launchGameThread);
  std::thread clockThread(this->launchClockThread);
  controlThread.join();
  gameThread.join();
  clockThread.join();
  std::cout << "Game over! Your final score is: " << board_.getFinalScore() << "\n";
}

void Game::launchControlThread() {
  SDL_Event e;
  while (this->isRunning) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
	isRunning = false;
      } else if (e.type == SDL_KEYDOWN) {
	switch (e.key.keysym.sym) {
	case SDLK_LEFT:
	  cmdQueue_.push(CMD_LEFT);
	  break;
	case SDLK_RIGHT:
	  cmdQueue_.push(CMD_RIGHT);
	  break;
	case SDLK_q:
	  cmdQueue_.push(CMD_ANTI);
	  break;
	case SDLK_r:
	  cmdQueue_.push(CMD_CLOCK);
	  break;
	case SDLK_DOWN:
	  cmdQueue_.push(CMD_DOWN);
	  break;
	case SDLK_SPACE:
	  cmdQueue_.push(CMD_DROP);
	  break;
	default:
	  break;
	}
      }
    }
  }
}

void Game::launchGameThread() {
  while (this->isRunning) {
    std::unique_lock<std::mutex> locker(mutexClock);
    condvarClock.wait(locker);
    bool gameLost = this->updateModelForThisFrame();
    this->renderView();
  }
}

void Game::launchClockThread() {
  while (this->isRunning) {
    std::this_thread::sleep_for(std::chrono::milliseconds(Game::MILLISECS_PER_TIMESTEP));
    condvarClock.notify_all();
  }
}

// Private member functions

void Game::updateModelForThisFrame() {
  // If cmdQueue is not empty, pop one command off and do board's timestep once
  bool isGameLost = false;
  if (cmdQueue_.getNumel() == 0) {
    isGameLost = board_.timestep(0); // Do nothing
  } else {
    int thisCommand = cmdQueue_.pop();
    isGameLost = board_.timestep(thisCommand);
  }
  if (isGameLost) {
    this->isRunning = false; // Serves as shutdown signal
  }
}

void Game::renderView() {
}
