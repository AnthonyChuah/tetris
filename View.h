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

  static constexpr int WINDOW_WIDTH = 400;
  static constexpr int WINDOW_HEIGHT = 480;
  static constexpr int NUMTYPES = 7;
  static constexpr int LEFTPORT_WIDTH = 280;
  static constexpr int LEFTPORT_HEIGHT = 480;
  static constexpr int RIGHTPORT_WIDTH = 120;
  static constexpr int RIGHTPORT_HEIGHT = 240;
  
  static constexpr int LEFTPORT_PAD = 40;
  static constexpr int RIGHTPORT_VERT_PAD = 100;
  static constexpr int RIGHTPORT_HORZ_PAD = 40;
  static constexpr int PIECE_WIDTH = 20;
  
  Board& board_;

  void renderMainTetrisBoard();
  void renderSquare(char _type, int _x, int _y); // x is column (pixel), y is row (pixel)
  void renderPiece(Piece* _piece, int _x, int _y);
  bool initGraphics(); // Do at construction

  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  // The window is 400 pixels wide, 480 pixels tall
  // The game region is 200 wide, 400 tall, and its top-left corner is at 40, 40
  // On the right side, you have two displayed Pieces (current and next pieces)
  // The current piece is upper-right, the next piece is lower-right
  SDL_Rect squares[NUMTYPES];
  int redValues[NUMTYPES];
  int greenValues[NUMTYPES];
  int blueValues[NUMTYPES];
  
 public:
  
  View(Board& _board);
  ~View();
  void render();
  void closeGraphics();
  
};

#endif
