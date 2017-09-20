#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>

#include "Piece.h"
#include "Board.h"

/* Pieces and colours guide:
Piece 0: 'o'. Yellow:   255, 255,   0
Piece 1: 'l'. Cyan:       0, 255, 255
Piece 2: 's'. Green:      0, 255,   0
Piece 3: 'z'. Red:      255,   0,   0
Piece 4: 'j'. Blue:       0,   0, 255
Piece 5: '7'. Orange:   255, 128,   0
Piece 6: 't'. Purple:   128,   0, 255
Background:   Grey:     128, 128, 128
*/

class View {

  static constexpr int NUMTYPES = 7;
  static constexpr int LEFTPORT_WIDTH = 280;
  static constexpr int LEFTPORT_HEIGHT = 480;
  static constexpr int RIGHTPORT_WIDTH = 120;
  static constexpr int RIGHTPORT_HEIGHT = 240;
  
  static constexpr int LEFTPORT_PAD = 40;
  static constexpr int RIGHTPORT_VERT_PAD = 100;
  static constexpr int RIGHTPORT_HORZ_PAD = 40;
  static constexpr int PIECE_WIDTH = 10;
  
  Board& board_;

  void renderMainTetrisBoard();
  void renderPiece(Piece* _piece, int _x, int _y); // x is column (pixel), y is row (pixel)
  bool initGraphics(); // Do at construction
  bool loadMedia(); // Do at construction
  void drawBrickAt(char _type, int _row, int _col); // ' ' type draws black empty space
  // Every character maps onto a given RGB colour (look up classic tetris colours)

  // All the SDL items here
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  // Left viewport, upper-right viewport, and lower-right viewport
  // Left viewport has a dark-grey border (40 pixels on all sides)
  // Left viewport's border surrounds the tetris area, 200 pixels wide and 400 pixels tall
  // Every slot on the board is 10 by 10 pixels
  // Left viewport total dimensions: 280 wide, 480 tall
  // SDL_Rect leftViewport;
  // Upper-right viewport is 120 wide and 240 tall
  // Upper-right viewport displays the current piece, with 40-by-40 space reserved for it
  // Padding on all sides is 40 pixels around the current piece's display
  // SDL_Rect upperRightViewport;
  // Lower-right viewport is 120 wide and 240 tall
  // Lower-right viewport displays the next piece, with 40-by-40 space reserved for it
  // Padding on all sides is 40 pixels around the next piece's display
  // SDL_Rect lowerRightViewport;
  // This array of SDL_Rects holds the stuff we can render
  SDL_Rect squares[NUMTYPES];
  int redValues[NUMTYPES];
  int greenValues[NUMTYPES];
  int blueValues[NUMTYPES];
  
 public:
  
  View(Board& _board);
  void render() const;
  void closeGraphics();
  
};

#endif
