# Chess

A chess game project developed by Jake Edmonstone and Matthew Tchouikine. This implementation supports both text-based and graphical displays of the game board, with various features and player modes. This document provides an overview of how to build, run, and interact with the chess program.

## Table of Contents
1. [Building the Project](#building-the-project)
2. [Running the Game](#running-the-game)
3. [Command Interpreter](#command-interpreter)
4. [Features](#features)

## Building the Project

To build the chess game, ensure you have a working environment that supports graphical applications (X11 for Unix-based systems or XMing for Windows). Then, compile the source files using the following command:

```bash
make
```

This will generate the executable file that you can use to start the game. Make sure you have all the necessary libraries installed to support graphics compilation.

## Running the Game

Once built, you can run the game by executing the compiled binary:

```bash
./chess
```

The game starts in command-line mode, where you can input various commands to set up and play the game.

## Command Interpreter

The chess game uses a command interpreter to interact with the system. Below are the commands supported:

### 1. Starting a New Game
- **Command:** `game white-player black-player`
- **Description:** Starts a new game. The `white-player` and `black-player` parameters can be either `human` or `computer[1-4]` to specify human players or computer players of different difficulty levels.

Example:
```bash
game human computer2
```

### 2. Resigning the Game
- **Command:** `resign`
- **Description:** Concedes the game to your opponent.

### 3. Moving Pieces
- **Command:** `move start end`
- **Description:** Moves a piece from the starting coordinates to the ending coordinates.

Example:
```bash
move e2 e4
```

- **Special Moves:**
  - **Castling:** Use the king's two-square move, e.g., `move e1 g1` for white castling.
  - **Pawn Promotion:** If your pawn reaches the end of the board, you will be prompted to select which piece you would like to promote to.
- **For Computer Player:** Simply wait after making a move, and the computer will make a move.

### 4. Entering Setup Mode
- **Command:** `setup`
- **Description:** Enters setup mode to configure the initial board state. This mode can only be accessed when a game is not running. The following commands are supported within setup mode:
  - **Place a Piece:** `+ piece square`
    - Places the specified piece on the specified square. Example: `+ K e1`
  - **Remove a Piece:** `- square`
    - Removes a piece from the specified square. Example: `- e1`
  - **Set Turn:** `= color`
    - Sets the color to move next. Example: `= white`
  - **Exit Setup Mode:** `done`
    - Exits setup mode. The program verifies board validity before exiting.

### 5. Additional Features
- The board is redisplayed after every command, including text-based and graphical representations.
- If a command is misspelled, the program will handle it gracefully to prevent breakdowns.

## Features

- **Game Modes:** Supports human vs. human, human vs. computer, and computer vs. computer games.
- **Difficulty Levels:** Computer players operate at multiple difficulty levels (1-4).
- **Text and Graphical Display:** Provides both a console-based and graphical display of the chessboard.
- **Setup Mode:** Allows customization of the initial board configuration.
- **Scoring:** Displays the final score when the game ends.

