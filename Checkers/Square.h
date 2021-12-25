#pragma once

#include "framework.h"

// this class represents an individual square on the board
class Square
{
public:
    enum class Color
    {
        WHITE = 0,
        BLACK
    };

public:
    Square();
    Piece::Color GetPieceColor();

    using DeletePiece = bool;

    void ClearPiece(DeletePiece deletePiece);
    void SetPiece(Piece* newPiece);
    Piece* GetPiece();
    Color GetColor();
    void SetColor(Color newColor);
    void Paint(HDC hDC, int left, int top, int right, int bottom);
    bool IsKing();
    void MakeKing();

private:
    Piece* m_piece = nullptr;
    Color m_color = Color::WHITE;
};
