#pragma once

#include "framework.h"

class Piece
{
public:
    enum class Color
    {
        INVALID = 0,
        RED,
        BLUE
    };

public:
    Piece(Color color);
    virtual ~Piece();

public:
    Color GetColor();
    bool IsKing();
    void MakeKing();

private:
    bool m_king = false;
    Color m_color = Color::INVALID;
};
