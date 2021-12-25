#include "Piece.h"


Piece::Piece(Color color)
{
    m_color = color;
    m_king = false;
}

Piece::~Piece()
{
}

Piece::Color Piece::GetColor()
{
    return m_color;
}

bool Piece::IsKing()
{
    return m_king;
}

void Piece::MakeKing()
{
    m_king = true;
}
