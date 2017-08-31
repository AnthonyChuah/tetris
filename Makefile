COMPILER_FLAGS = -Wall -g -c -std=c++11

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

TestPiece: TestPiece.o Piece.o
	g++ TestPiece.o Piece.o -o TestPiece

TestBoard: TestBoard.o Board.o Piece.o
	g++ TestBoard.o Board.o Piece.o -o TestBoard

TestPiece.o: TestPiece.cpp Piece.h Grid.h
	g++ $(COMPILER_FLAGS) TestPiece.cpp

TestBoard.o: TestBoard.cpp Board.h Piece.h Grid.h
	g++ $(COMPILER_FLAGS) TestBoard.cpp

Piece.o: Piece.cpp Piece.h Grid.h
	g++ $(COMPILER_FLAGS) Piece.cpp

Board.o: Board.cpp Board.h Piece.h Grid.h
	g++ $(COMPILER_FLAGS) Board.cpp

clean:
	rm -rf TestPiece *.o
