#include "View.h"

#include <iostream>

View::View(Board& _board) : board_(_board) {
  if (!this->initGraphics()) {
    throw std::exception("Graphics initialization failed in View constructor.");
  }
  if (!this->loadMedia()) {
    throw std::exception("Media loading failed in View constructor.");
  }
  // Colour in all the squares
  for (int i = 0; i < View::NUMTYPES; ++i) {
    squares[i].w = View::PIECE_WIDTH;
    squares[i].h = View::PIECE_WIDTH;
  }
  redValues[0] = 255;
  redValues[1] = 0;
  redValues[2] = 0;
  redValues[3] = 255;
  redValues[4] = 0;
  redValues[5] = 255;
  redValues[6] = 128;
  greenValues[0] = 255;
  greenValues[1] = 255;
  greenValues[2] = 255;
  greenValues[3] = 0;
  greenValues[4] = 0;
  greenValues[5] = 128;
  greenValues[6] = 0;
  blueValues[0] = 0;
  blueValues[1] = 255;
  blueValues[2] = 0;
  blueValues[3] = 0;
  blueValues[4] = 255;
  blueValues[5] = 0;
  blueValues[6] = 255;
}

void View::render() const {
  SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 0xFF);
  SDL_RenderClear(gRenderer);
  // Iterate over the board's rows and cols, and render each
  // SDL_RenderSetViewport(gRenderer, &leftViewport);
  this->renderMainTetrisBoard();
  // Next, render the image of the current piece
  // SDL_RenderSetViewport(gRenderer, &upperRightViewport);
  int startX, startY;
  startX = View::LEFTPORT_WIDTH + RIGHTPORT_HORZ_PAD;
  startY = View::RIGHTPORT_VERT_PAD;
  this->renderPiece(board_.currentPiece_, startX, startY);
  // Next, render the image of the next piece
  // SDL_RenderSetViewport(gRenderer, &lowerRightViewport);
  startY = View::RIGHTPORT_HEIGHT + View::RIGHTPORT_VERT_PAD;
  this->renderPiece(board_.nextPiece_, startX, startY);
  // Finally, render the graphics
  SDL_RenderPresent(gRenderer);
}

void View::closeGraphics() {
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = nullptr;
  gRenderer = nullptr;
  SDL_Quit();
}

// Private member functions

void View::renderMainTetrisBoard() {
  
}

void View::renderPiece(Piece* _piece, int _x, int _y) {
  char type = _piece->type(); int pieceIndex;
  if (type == 'o') {
    pieceIndex = 0;
  } else if (type == 'l') {
    pieceIndex = 1;
  } else if (type == 's') {
    pieceIndex = 2;
  } else if (type == 'z') {
    pieceIndex = 3;
  } else if (type == 'j') {
    pieceIndex = 4;
  } else if (type == '7') {
    pieceIndex = 5;
  } else if (type == 't') {
    pieceIndex = 6;
  }
  squares[pieceIndex].x = _x;
  squares[pieceIndex].y = _y;
  int r = redValues[pieceIndex];
  int g = greenValues[pieceIndex];
  int b = blueValues[pieceIndex];
  
}

bool View::initGraphics() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
    return false;
  }
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cout << "Warning: Linear texture filtering not enabled!";
  }
  gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_WIDTH, BOARD_HEIGHT + TEXT_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == nullptr) {
    std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
    return false;
  }
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == NULL) {
    std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
    return false;
  }
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
    return false;
  }
  // leftViewport.x = 0;
  // leftViewport.y = 0;
  // leftViewport.w = View::LEFTPORT_WIDTH;
  // leftViewport.h = View::LEFTPORT_HEIGHT;
  // upperRightViewport.x = View::LEFTPORT_WIDTH;
  // upperRightViewport.y = 0;
  // upperRightViewport.w = View::RIGHTPORT_WIDTH;
  // upperRightViewport.h = View::RIGHTPORT_HEIGHT;
  // lowerRightViewport.x = View::LEFTPORT_WIDTH;
  // lowerRightViewport.y = View::RIGHTPORT_HEIGHT;
  // lowerRightViewport.w = View::RIGHTPORT_WIDTH;
  // lowerRightViewport.h = View::RIGHTPORT_HEIGHT;
  return true;
}

bool View::loadMedia() {
}

void View::drawBrickAt(char _type, int _row, int _col) {
}
