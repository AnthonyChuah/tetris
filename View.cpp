#include "View.h"

#include <iostream>

View::View(Board& _board) : board_(_board) {
  if (!this->initGraphics()) {
    std::cout << "Graphics initialization failed in View constructor.\n";
    throw std::exception();
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

View::~View() {
  this->closeGraphics();
}

void View::render() {
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
  int height = Board::HEIGHT;
  int width = Board::WIDTH;
  int x, y; char type;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      x = LEFTPORT_PAD + j * View::PIECE_WIDTH;
      y = LEFTPORT_PAD + i * View::PIECE_WIDTH;
      type = board_.board_.get(i, j);
      this->renderSquare(type, x,  y);
    }
  }
  int row = board_.currentPiece_->getRowPos();
  int col = board_.currentPiece_->getColPos();
  x = LEFTPORT_PAD + col * View::PIECE_WIDTH;
  y = LEFTPORT_PAD + row * View::PIECE_WIDTH;
  this->renderPiece(board_.currentPiece_, x, y);
}

void View::renderSquare(char _type, int _x, int _y) {
  int pieceIndex;
  bool isEmptySquare = false;
  if (_type == 'o') {
    pieceIndex = 0;
  } else if (_type == 'l') {
    pieceIndex = 1;
  } else if (_type == 's') {
    pieceIndex = 2;
  } else if (_type == 'z') {
    pieceIndex = 3;
  } else if (_type == 'j') {
    pieceIndex = 4;
  } else if (_type == '7') {
    pieceIndex = 5;
  } else if (_type == 't') {
    pieceIndex = 6;
  } else {
    isEmptySquare = true;
  }
  if (isEmptySquare) {
    SDL_Rect fillRect = { _x, _y, View::PIECE_WIDTH, View::PIECE_WIDTH };
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &fillRect);
    return;
  }
  squares[pieceIndex].x = _x;
  squares[pieceIndex].y = _y;
  int r = redValues[pieceIndex];
  int g = greenValues[pieceIndex];
  int b = blueValues[pieceIndex];
  SDL_SetRenderDrawColor(gRenderer, r, g, b, 255);
  SDL_RenderFillRect(gRenderer, &squares[pieceIndex]);
}

void View::renderPiece(Piece* _piece, int _x, int _y) {
  char type = _piece->type();
  int orientation = _piece->getOrient();
  int frameWidth = _piece->rotateFrameWidth();
  char* array = Piece::orientMap_[type][orientation];
  int x, y, flatIndex;
  for (int i = 0; i < frameWidth; ++i) {
    for (int j = 0; j < frameWidth; ++j) {
      x = _x + j * View::PIECE_WIDTH;
      y = _y + i * View::PIECE_WIDTH;
      flatIndex = i * frameWidth + j;
      if (array[flatIndex] > ' ') {
	this->renderSquare(type, x, y);
      }
    }
  }
}

bool View::initGraphics() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
    return false;
  }
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cout << "Warning: Linear texture filtering not enabled!";
  }
  gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, View::WINDOW_WIDTH, View::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == nullptr) {
    std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
    return false;
  }
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == nullptr) {
    std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
    return false;
  }
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
