COMPILER_FLAGS = -Wall -g -c -std=c++11 -pthread

LINKER_FLAGS = -lSDL2 -pthread

tetris: main.o Game.o View.o Board.o Piece.o
	g++ main.o Game.o View.o Board.o Piece.o $(LINKER_FLAGS) -o tetris

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

View.o: View.cpp View.h Board.h Piece.h Grid.h
	g++ $(COMPILER_FLAGS) View.cpp

Game.o: Game.cpp Game.h View.h Board.h Piece.h Grid.h
	g++ $(COMPILER_FLAGS) Game.cpp

main.o: main.cpp Game.h View.h Board.h Piece.h Grid.h
	g++ $(COMPILER_FLAGS) main.cpp

.PHONY: clean

clean:
	rm -rf TestPiece TestBoard tetris *.o
