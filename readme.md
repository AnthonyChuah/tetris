# Tetris game using SDL2 graphics engine

## Dependencies:

Linux OS: I built successfully on Ubuntu 16.04, but other versions should work fine.

make, gcc, and SDL2

```
sudo apt-get install build-essential
sudo apt-get install libsdl2-dev
```

## How to install

Clone this repository

`git clone https://github.com/AnthonyChuah/tetris.git`

In the main directory, build the program:

`make tetris`

Then launch the program:

`./tetris`

## Controls

* LEFT key press: shifts the current piece left
* RIGHT key press: shifts the current piece right
* r key press: rotates current piece clockwise
* q key press: rotates current piece anti-clockwise
* DOWN key press: shifts the current piece down one step
* SPACE key press: drops the current piece to the bottom

There is no pause button.

## Interface

The left viewport is where the game happens.

On the right, you will see two pieces on display. The upper piece is your current piece. The lower piece is the next incoming piece.

The score is defined by how many rows you have destroyed.

There is no animation for row destruction.

There is no music.

## Game parameters

A _piece_ is one of the Tetris pieces, i.e. the long 'l' shaped piece, the square piece, etc.

The _grid_ is the area in which the game takes place. It has 20 rows and 10 columns.

A _tick_ is an event in the game which changes a piece's row position or the grid's contents.

A _timestep_ is a mini-event, in which you may rotate the piece or shift the piece horizontally or inch the piece downwards or drop the piece to the bottom.

Every timestep takes 50 milliseconds, and every tick is forced to happen after 8 timesteps. These parameters may be changed easily in the source code.

The game ends when the new piece does not have the space to be put into play.
