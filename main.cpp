#include "Game.h"
#include "Piece.h"

int main() {
  Piece::populateLookupMaps();
  Game g;
  g.launchAllThreads();
  Piece::destructStaticMaps();
  return 0;
}
