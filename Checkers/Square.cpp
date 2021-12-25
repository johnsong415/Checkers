#include "Square.h"


Square::Square()
{
    m_piece = nullptr;
    m_color = Color::WHITE;
}

Piece::Color Square::GetPieceColor()
{
    if (!m_piece) { return Piece::Color::INVALID; }
    return m_piece->GetColor();
}

void Square::ClearPiece(DeletePiece deletePiece)
{
    if (deletePiece) {
        delete m_piece;
    }

    m_piece = nullptr;
}

void Square::SetPiece(Piece* newPiece)
{
    m_piece = newPiece;
}

Piece* Square::GetPiece()
{
    return m_piece;
}

Square::Color Square::GetColor()
{
    return m_color;
}

void Square::SetColor(Color newColor)
{
    m_color = newColor;
}

bool Square::IsKing()
{
    if (!m_piece) { return false; }
    return m_piece->IsKing();
}

void Square::MakeKing()
{
    if (!m_piece) return;
    m_piece->MakeKing();
}

void Square::Paint(HDC hDC, int left, int top, int right, int bottom)
{
    HGDIOBJ original = SelectObject(hDC, GetStockObject(DC_PEN));

    // drawing the squares
    if (m_color == Color::BLACK) {
        SelectObject(hDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hDC, RGB(100, 100, 100));
        Rectangle(hDC, left, top, right, bottom);
    }

    // drawing the pieces
    if (m_piece != nullptr) {
        SelectObject(hDC, GetStockObject(DC_BRUSH));

        if (m_piece->GetColor() == Piece::Color::BLUE) {
            SetDCBrushColor(hDC, RGB(0, 0, 204));
        }
        else {
            SetDCBrushColor(hDC, RGB(204, 0, 0));
        }
        Ellipse(hDC, left, top, right, bottom);
        
        if (m_piece->IsKing()) {
            // show "K" if king
            if (m_piece->GetColor() == Piece::Color::BLUE) { SetBkColor(hDC, RGB(0, 0, 204)); }
            else { SetBkColor(hDC, RGB(204, 0, 0)); }
            SetTextColor(hDC, RGB(204, 153, 0));
            TextOut(hDC, left + 20, top + 15, L"K", 1);
        }
    }

    SelectObject(hDC, original);
}
