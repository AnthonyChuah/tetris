#ifndef VIEW_H
#define VIEW_H

class View {

  Board& board_;
  bool initGraphics(); // Do at construction
  bool loadMedia(); // Do at construction
  void drawBrickAt(char _type, int _row, int _col); // ' ' type draws black empty space
  // Every character maps onto a given RGB colour (look up classic tetris colours)

  // All the SDL items here
  // Left viewport, upper-right viewport, and lower-right viewport
  // Left viewport has a dark-grey border (40 pixels on all sides)
  // Left viewport's border surrounds the tetris area, 200 pixels wide and 400 pixels tall
  // Every slot on the board is 10 by 10 pixels
  // Left viewport total dimensions: 280 wide, 480 tall
  // Upper-right viewport is 120 wide and 240 tall
  // Upper-right viewport displays the current piece, with 40-by-40 space reserved for it
  // Padding on all sides is 40 pixels around the current piece's display
  // Lower-right viewport is 120 wide and 240 tall
  // Lower-right viewport displays the next piece, with 40-by-40 space reserved for it
  // Padding on all sides is 40 pixels around the next piece's display
  
 public:

  View(Board& _board);
  void render() const;
  
};

#endif
