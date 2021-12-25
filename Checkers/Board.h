#pragma once

#include "framework.h"
#include "Square.h"

// this class represents the board checkers is played on
class Board
{
private:
    struct SelectedPiece
    {
        bool selected = false;
        int row = -1;
        int column = -1;
    };

public:
    Board();
    virtual ~Board();

public:
    // Board constructor takes board sidelength and the HDC
    void InitializeBoard(int sideLength, HDC hDC);

    // Initalizes board with pieces
    void SetupBoard();

    void DrawBoard();
    // select/deselect a piece, moves if piece is already
    void Select(int row, int column);
    // show all possible moves
    void ShowPossibleMoves();
    // paint a move (green square)
    void PaintPossibleMove(int row, int col);

    bool CanMove(int row, int column);

    void Move(int row, int column);
    // the computer makes a move
    void ComputerMove();
    // returns true or false if game is over or not
    bool IsGameOver();
    // show game over
    void ShowEndMessage(Piece::Color winner);
    // reset
    void Reset();
    // erases the top part
    void EraseTop();
    // checks if player can move
    bool HasPossibleMoves(Piece::Color pieceColor);

private:
    int m_boardSize = 0;
    Square** m_board = nullptr; // 2d array of squares
    HDC m_hDC = NULL;

    // stores the selected piece
    SelectedPiece m_pieceSelected;
};
