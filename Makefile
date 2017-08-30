COMPILER_FLAGS = -Wall -g -c -std=c++11

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

TestPiece: TestPiece.o Piece.o
	g++ TestPiece.o Piece.o -o TestPiece

TestPiece.o: TestPiece.cpp Piece.h Grid.h
	g++ $(COMPILER_FLAGS) TestPiece.cpp

Piece.o: Piece.cpp Piece.h Grid.h
	g++ $(COMPILER_FLAGS) Piece.cpp

clean:
	rm -rf TestPiece *.o
