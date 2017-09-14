#include "Game.h"

#include <thread>
#include <chrono>

Game::Game() : board_(Game::TIMESTEPS_PER_TICK), view_(&board_) {}

void Game::launchAllThreads() {
  this->isRunning = true;
  std::thread controlThread(this->launchControlThread);
  std::thread gameThread(this->launchGameThread);
  std::thread clockThread(this->launchClockThread);
  controlThread.join();
  gameThread.join();
  clockThread.join();
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
    this->updateModelForThisFrame();
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
}

void Game::renderView() {
}
