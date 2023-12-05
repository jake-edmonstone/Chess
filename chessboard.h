#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <memory>
#include <string>
#include "abstractpiece.h"
#include "graphicsdisplay.h"

class ChessBoard {
  public:
  // A 2D vector of unique_ptrs to Pieces
  std::vector<std::vector<std::unique_ptr<AbstractPiece>>> board;
  // Vectors of raw const pointers to Pieces
  std::vector<const AbstractPiece*> whitePieces;
  std::vector<const AbstractPiece*> blackPieces;
  // graphics display
  std::unique_ptr<GraphicsDisplay> gd;
  // Checks if a pair (r,c) is within board limits
  bool inrange(int r, int c) const;
  // swicthes between eg. e4 -> 3,4
  std::pair<int,int> rankFileToIntPair(std::string rf) const;
  // inverse of previous
  std::string intPairToRankFile(int row, int col) const;
  // update list of owned pieces
  void updatePieceLists();
  // called by calculateAvailableMoves to remove the possibility of putting yourself in check
  void dontCheckYourself();
  // called by calculate moves to make sure you get out of check if you are in it
  void getOutOfCheck();
  // Adds available moves based on each piece's potential moves, implementing standard Pawn capture and movement
  void basicAddAvailableMoves();
  // Adds threats to the pieces based on targets stored in each piece
  void addThreats();
  // Adds threats to Pawns that are EnPassantable, as well as adding available moves and targets to pawns that are threats
  void addEnPassantMoves();
  // Adds castling moves for all Rooks and Kings is they are Castleable
  void addCastlingMoves();
  // Returns if castling is an option for the X... half of White's side
  bool whiteRightCastleOpen() const;
  bool whiteLeftCastleOpen() const;
  bool blackRightCastleOpen() const;
  bool blackLeftCastleOpen() const;
  // Returns if position is being targetted by another piece
  bool isTarget(std::string position) const;
  // updates positions of all pieces on the grid
  void updatePositions();
 public:
  // ChessBoard default ctor, makes an empty board
  ChessBoard();
  // ChessBoard copy ctor (deep copy)
  ChessBoard(const ChessBoard &other);
  // clears chessboard (removes pieces)
  void clear();
  // Sets up a chess board to offical "default" position
  void defaultSetup();
  // determines whether or not a Piece is blocked by another Piece from moving by (xshift, yshift)
  bool isBlocked(int firstpiecey, int firstpiecex, int yshift, int xshift) const;
  // updates each pieces fields (threats, targets, availaible moves)
  void calculateAvailableMoves();
  // returns whether or not a particular colour is in check
  bool isCheck(std::string colour) const;
  bool isCheckMate(std::string colour) const;
  bool isStaleMate(std::string colour) const;
  bool isInsufficientMaterial() const;
  // moves a piece from one location on the board to another; for example, "f3" to "d3"
  // requires: there exists a piece at start
  bool movePiece(std::string start, std::string end);
  // returns a const ref to the list of colour's pieces
  const std::vector<const AbstractPiece*> &getPieces(std::string colour) const; 
  // places a piece of type type at position square
  void placePiece(char type, std::string square);
  // returns the piece at position square
  const AbstractPiece* getPiece(std::string square) const;
  // removes a piece at position square
  void removePiece(std::string square);
  // returns true if there is a pawn on the first or last rank
  bool pawnOnFirstOrLastRank() const;
  // returns 2d vec of chars corresponding to piece types and positions
  const std::vector<std::vector<char>> getBoard() const;
  // gets the value of a board, where whitepieces are weighed positively and Black pieces are negative
  int getValue() const;
  // returns whether or not the board is in a terminal state
  bool isTerminalState() const;
  // prints out a text display of the board with the white pieces on top
  friend std::ostream &operator<<(std::ostream &out, const ChessBoard &board);
};

// determines if ap is a valid EnPassantable piece
bool enPassantable(const AbstractPiece * ap, std::string name);

template<typename T> bool in(const std::vector<T>& vec, const T& element);
template<typename T> bool myRemove(std::vector<T>& vec, const T& element);

std::ostream &operator<<(std::ostream &out, const ChessBoard &chessboard);

#endif