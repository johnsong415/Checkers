#include "Board.h"


Board::Board()
{
}

void Board::InitializeBoard(int sideLength, HDC hDC)
{
    m_pieceSelected = SelectedPiece();
    m_boardSize = sideLength;
    m_board = new Square*[sideLength];
    for (int i = 0; i < sideLength; ++i) {
        m_board[i] = new Square[sideLength];
    }
    m_hDC = hDC;
}

Board::~Board()
{
    for (int i = 0; i < m_boardSize; ++i) {
        delete[] m_board[i];
    }
    delete[] m_board;
    m_board = nullptr;
}

void Board::SetupBoard()
{
    // sets the square colors of the board according to the checkers board
    // all squares are white by default
    for (int row = 0; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            if (row % 2 == 0) {
                if (column % 2 == 1) {
                    m_board[row][column].SetColor(Square::Color::BLACK);
                }
            }
            else {
                if (column % 2 == 0) {
                    m_board[row][column].SetColor(Square::Color::BLACK);
                }
            }
        }
    }

    // sets top (opponent's) pieces
    ///*
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            if (m_board[row][column].GetColor() == Square::Color::BLACK) {
                m_board[row][column].SetPiece(new Piece(Piece::Color::BLUE));
            }
        }
    }
    //*/
    //m_board[2][3].SetPiece(new Piece(Piece::Color::BLUE));
    
    // sets bottom (player's) pieces
    
    ///*
    for (int row = m_boardSize - 3; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            if (m_board[row][column].GetColor() == Square::Color::BLACK) {
                m_board[row][column].SetPiece(new Piece(Piece::Color::RED));
            }
        }
    }
    //*/
    
    //m_board[5][0].SetPiece(new Piece(Piece::Color::RED), false);
}

void Board::DrawBoard()
{
    // draw grid lines
    for (int i = 1; i < m_boardSize + 2; ++i) {
        MoveToEx(m_hDC, 50, 50 * i, nullptr);
        LineTo(m_hDC, 50 * (m_boardSize + 1), 50 * i);
        MoveToEx(m_hDC, 50 * i, 50, nullptr);
        LineTo(m_hDC, 50 * i, 50 * (m_boardSize + 1));
    }

    // draw squares
    for (int row = 0; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            int x = 50 * (column + 1);
            int y = 50 * (row + 1);
            m_board[row][column].Paint(m_hDC, x, y, x + 50, y + 50);
        }
    }

    //
    SetBkColor(m_hDC, RGB(255, 255, 255));
    SetTextColor(m_hDC, RGB(0, 0, 0));
    TextOut(m_hDC, 0, 0, L"Press Q to quit.", 16);
    TextOut(m_hDC, 0, 15, L"Press R to restart.", 19);
}

void Board::Select(int row, int column)
{
    if (!HasPossibleMoves(Piece::Color::RED)) {
        ShowEndMessage(Piece::Color::BLUE);
        return;
    }

    // if coordinates are invalid
    if (row < 0 || row > m_boardSize - 1 || column < 0 || column > m_boardSize - 1) { return; }


    if (m_pieceSelected.selected) {
        m_pieceSelected.selected = false;
        if (CanMove(row, column)) {
            Move(row, column);
            EraseTop();
            ComputerMove();
            DrawBoard();
            IsGameOver();
            return;
        }

        DrawBoard();
    }
    else {
        // not selected => select!
        if (m_board[row][column].GetPieceColor() == Piece::Color::INVALID || m_board[row][column].GetPieceColor() == Piece::Color::BLUE) { return; }
        m_pieceSelected.selected = true;
        m_pieceSelected.row = row;
        m_pieceSelected.column = column;
        ShowPossibleMoves();
    }
}

bool Board::CanMove(int row, int column)
{
    if (row > m_boardSize - 1 || row < 0 || column > m_boardSize - 1 || column < 0) {
        return false;
    }
    
    bool canJump = false;
    for (int r = 0; r < m_boardSize; ++r) {
        for (int c = 0; c < m_boardSize; ++c) {
            if (m_board[r][c].GetPieceColor() == Piece::Color::RED) {
                if (m_board[r][c].IsKing()) {
                    if (r < m_boardSize - 2 && c > 1 && m_board[r + 1][c - 1].GetPieceColor() == Piece::Color::BLUE && m_board[r + 2][c - 2].GetPieceColor() == Piece::Color::INVALID) {
                        canJump = true;
                    }
                    if (r < m_boardSize - 2 && c < m_boardSize - 2 && m_board[r + 1][c + 1].GetPieceColor() == Piece::Color::BLUE && m_board[r + 2][c + 2].GetPieceColor() == Piece::Color::INVALID) {
                        canJump = true;
                    }
                }
                if (r > 1 && c > 1 && m_board[r - 1][c - 1].GetPieceColor() == Piece::Color::BLUE && m_board[r - 2][c - 2].GetPieceColor() == Piece::Color::INVALID) {
                    canJump = true;
                }
                if (r > 1 && c < m_boardSize - 2 && m_board[r - 1][c + 1].GetPieceColor() == Piece::Color::BLUE && m_board[r - 2][c + 2].GetPieceColor() == Piece::Color::INVALID) {
                    canJump = true;
                }
            }
        }
    }

    // if moving up
    if (row < m_pieceSelected.row) {
        // if moving one row up
        if (!canJump && row == m_pieceSelected.row - 1) {
            // if moving left
            if (column == m_pieceSelected.column + 1) {
                return m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
            }
            // if moving right
            else if (column == m_pieceSelected.column - 1) {
                return m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
            }
            else {
                return false;
            }
        }
        else if (row == m_pieceSelected.row - 2) {
            if (column == m_pieceSelected.column + 2) {
                return m_board[row + 1][column - 1].GetPieceColor() == Piece::Color::BLUE && m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
            }
            else if (column == m_pieceSelected.column - 2) {
                return m_board[row + 1][column + 1].GetPieceColor() == Piece::Color::BLUE && m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    if (row > m_pieceSelected.row) {
        if (m_board[m_pieceSelected.row][m_pieceSelected.column].IsKing()) {
            if (!canJump && row == m_pieceSelected.row + 1) {
                if (column == m_pieceSelected.column + 1) {
                    return m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
                }
                else if (column == m_pieceSelected.column - 1) {
                    return m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
                }
                else {
                    return false;
                }
            }
            else if (row == m_pieceSelected.row + 2) {
                if (column == m_pieceSelected.column + 2) {
                    return m_board[row - 1][column - 1].GetPieceColor() == Piece::Color::BLUE && m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
                }
                else if (column == m_pieceSelected.column - 2) {
                    return m_board[row - 1][column + 1].GetPieceColor() == Piece::Color::BLUE && m_board[row][column].GetPieceColor() == Piece::Color::INVALID;
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }

    return false;
}

void Board::ShowPossibleMoves()
{
    /* move for upper left */
    // move one square
    if (CanMove(m_pieceSelected.row - 1, m_pieceSelected.column - 1)) {
        PaintPossibleMove(m_pieceSelected.row - 1, m_pieceSelected.column - 1);
    }
    // jump
    if (CanMove(m_pieceSelected.row - 2, m_pieceSelected.column - 2)) {
        PaintPossibleMove(m_pieceSelected.row - 2, m_pieceSelected.column - 2);
    }
    
    /* move for upper right */	
    // move one square
    if (CanMove(m_pieceSelected.row - 1, m_pieceSelected.column + 1)) {
        PaintPossibleMove(m_pieceSelected.row - 1, m_pieceSelected.column + 1);
    }
    // jump
    if (CanMove(m_pieceSelected.row - 2, m_pieceSelected.column + 2)) {
        PaintPossibleMove(m_pieceSelected.row - 2, m_pieceSelected.column + 2);
    }
    
    /* move for lower left */
    // move one square
    if (CanMove(m_pieceSelected.row + 1, m_pieceSelected.column - 1)) {
        PaintPossibleMove(m_pieceSelected.row + 1, m_pieceSelected.column - 1);
    }
    // jump
    if (CanMove(m_pieceSelected.row + 2, m_pieceSelected.column - 2)) {
        PaintPossibleMove(m_pieceSelected.row + 2, m_pieceSelected.column - 2);
    }
    
    /* move for lower right */
    // move one square
    if (CanMove(m_pieceSelected.row + 1, m_pieceSelected.column + 1)) {
        PaintPossibleMove(m_pieceSelected.row + 1, m_pieceSelected.column + 1);
    }
    //jump
    if (CanMove(m_pieceSelected.row + 2, m_pieceSelected.column + 2)) {
        PaintPossibleMove(m_pieceSelected.row + 2, m_pieceSelected.column + 2);
    }
}

void Board::PaintPossibleMove(int row, int col)
{
    HGDIOBJ original = SelectObject(m_hDC, GetStockObject(DC_PEN));
    Square dest = m_board[row][col];
    // if destination is for some reason not empty, return
    if (dest.GetPieceColor() != Piece::Color::INVALID) { return; }
    int x = 50 * (col + 1);
    int y = 50 * (row + 1);
    SelectObject(m_hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(m_hDC, RGB(0, 225, 0));
    Rectangle(m_hDC, x, y, x + 50, y + 50);
    SelectObject(m_hDC, original);
}

void Board::Move(int row, int column)
{
    // if jumping
    if (row - m_pieceSelected.row == -2 || row - m_pieceSelected.row == 2) {
        int rFactor = (row - m_pieceSelected.row) / 2;
        int cFactor = (column - m_pieceSelected.column) / 2;
        // remove piece
        m_board[row - rFactor][column - cFactor].ClearPiece(Square::DeletePiece{ true });
    }

    // move piece
    m_board[row][column].SetPiece(m_board[m_pieceSelected.row][m_pieceSelected.column].GetPiece());
    m_board[m_pieceSelected.row][m_pieceSelected.column].ClearPiece(Square::DeletePiece{ false });

    if (row == 0) {
        m_board[row][column].MakeKing();
    }
}

void Board::ComputerMove()
{
    for (int row = 0; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            if (m_board[row][column].GetPieceColor() == Piece::Color::BLUE) {
                if (m_board[row][column].IsKing()) {
                    if (row > 1 && column > 1 && m_board[row - 1][column - 1].GetPieceColor() == Piece::Color::RED && m_board[row - 2][column - 2].GetPieceColor() == Piece::Color::INVALID) {
                        m_board[row - 2][column - 2].SetPiece(m_board[row][column].GetPiece());
                        m_board[row - 1][column - 1].ClearPiece(Square::DeletePiece{ true }); 
                        m_board[row][column].ClearPiece(Square::DeletePiece{ false });
                        return;
                    }
                    if (row > 1 && column < m_boardSize - 2 && m_board[row - 1][column + 1].GetPieceColor() == Piece::Color::RED && m_board[row - 2][column + 2].GetPieceColor() == Piece::Color::INVALID) {
                        m_board[row - 2][column + 2].SetPiece(m_board[row][column].GetPiece());
                        m_board[row - 1][column + 1].ClearPiece(Square::DeletePiece{ true });
                        m_board[row][column].ClearPiece(Square::DeletePiece{ false });
                        return;
                    }
                }
                if (row < m_boardSize - 2 && column > 1 && m_board[row + 1][column - 1].GetPieceColor() == Piece::Color::RED && m_board[row + 2][column - 2].GetPieceColor() == Piece::Color::INVALID) {
                    if (row + 2 == m_boardSize - 1) {
                        m_board[row][column].MakeKing();
                    }
                    m_board[row + 2][column - 2].SetPiece(m_board[row][column].GetPiece());
                    m_board[row + 1][column - 1].ClearPiece(Square::DeletePiece{ true });
                    m_board[row][column].ClearPiece(Square::DeletePiece{ false });
                    return;
                }
                if (row < m_boardSize - 2 && column < m_boardSize - 2 && m_board[row + 1][column + 1].GetPieceColor() == Piece::Color::RED && m_board[row + 2][column + 2].GetPieceColor() == Piece::Color::INVALID) {
                    if (row + 2 == m_boardSize - 1) {
                        m_board[row][column].MakeKing();
                    }
                    m_board[row + 2][column + 2].SetPiece(m_board[row][column].GetPiece());
                    m_board[row + 1][column + 1].ClearPiece(Square::DeletePiece{ true });
                    m_board[row][column].ClearPiece(Square::DeletePiece{ false }); 
                    return;
                }
            }
            EraseTop();
        }
    }
    
    for (int row = 0; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            if (m_board[row][column].GetPieceColor() == Piece::Color::BLUE) {
                if (m_board[row][column].IsKing()) {
                    if (row > 0 && column > 0 && m_board[row - 1][column - 1].GetPieceColor() == Piece::Color::INVALID) {
                        m_board[row - 1][column - 1].SetPiece(m_board[row][column].GetPiece());
                        m_board[row][column].ClearPiece(Square::DeletePiece{ false }); 
                        return;
                    }
                    if (row > 0 && column < m_boardSize - 1 && m_board[row - 1][column + 1].GetPieceColor() == Piece::Color::INVALID) {
                        m_board[row - 1][column + 1].SetPiece(m_board[row][column].GetPiece());
                        m_board[row][column].ClearPiece(Square::DeletePiece{ false }); 
                        return;
                    }

                }
                if (row < m_boardSize - 1 && column > 0 && m_board[row + 1][column - 1].GetPieceColor() == Piece::Color::INVALID) {
                    if (row + 1 == m_boardSize - 1) {
                        m_board[row][column].MakeKing();
                    }
                    m_board[row + 1][column - 1].SetPiece(m_board[row][column].GetPiece());
                    m_board[row][column].ClearPiece(Square::DeletePiece{ false });
                    return;
                }
                if (row < m_boardSize - 1 && column < m_boardSize - 1 && m_board[row + 1][column + 1].GetPieceColor() == Piece::Color::INVALID) {
                    if (row + 1 == m_boardSize - 1) {
                        m_board[row][column].MakeKing();
                    }
                    m_board[row + 1][column + 1].SetPiece(m_board[row][column].GetPiece());
                    m_board[row][column].ClearPiece(Square::DeletePiece{ false });
                    return;
                }
            }
            EraseTop();
        }
    }

    ShowEndMessage(Piece::Color::RED);
}

bool Board::IsGameOver()
{
    bool rAlive = false;
    bool bAlive = false;
    for (int row = 0; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            if (m_board[row][column].GetPieceColor() == Piece::Color::RED) {
                rAlive = true;
            }
            if (m_board[row][column].GetPieceColor() == Piece::Color::BLUE) {
                bAlive = true;
            }
        }
    }

    if (rAlive && bAlive) { return false; }
    else if (rAlive) {
        ShowEndMessage(Piece::Color::RED);
    }
    else if (bAlive) {
        ShowEndMessage(Piece::Color::BLUE);
    }
    
    return true;
}

void Board::ShowEndMessage(Piece::Color winner)
{
    // TODO: Add message box showing win/defeat, using MessageBox func
    // TODO: Add screenshots and desc. in README.md

    EraseTop();
    SetBkColor(m_hDC, RGB(255, 255, 255));
    SetTextColor(m_hDC, RGB(0, 0, 0));

    TextOut(m_hDC, 200, 0, L"The winner is:", 14);
    if (winner == Piece::Color::RED) {
        SetTextColor(m_hDC, RGB(204, 0, 0));
        TextOut(m_hDC, 300, 0, L"RED", 3);
    }
    else if (winner == Piece::Color::BLUE) {
        SetTextColor(m_hDC, RGB(0, 0, 204));
        TextOut(m_hDC, 300, 0, L"BLUE", 4);
    }

}

void Board::Reset()
{
    for (int row = 0; row < m_boardSize; ++row) {
        for (int column = 0; column < m_boardSize; ++column) {
            m_board[row][column].ClearPiece(Square::DeletePiece{ true });
        }
    }
    SetupBoard();
    DrawBoard();
    EraseTop();
}

void Board::EraseTop()
{
    RECT rect;
    rect.left = 150;
    rect.right = 400;
    rect.top = 0;
    rect.bottom = 49;
    SetDCBrushColor(m_hDC, RGB(255, 255, 255));
    FillRect(m_hDC, &rect, (HBRUSH)GetStockObject(DC_BRUSH));
}

bool Board::HasPossibleMoves(Piece::Color pieceColor)
{
    bool hasMoves = false;
    for (int r = 0; r < m_boardSize; ++r) {
        for (int c = 0; c < m_boardSize; ++c) {
            if (m_board[r][c].GetPieceColor() == pieceColor) {
                if (m_board[r][c].IsKing()) {
                    if (r < m_boardSize - 2 && c > 1 && m_board[r + 1][c - 1].GetPieceColor() == Piece::Color::BLUE && m_board[r + 2][c - 2].GetPieceColor() == Piece::Color::INVALID) {
                        hasMoves = true;
                    }
                    if (r < m_boardSize - 2 && c < m_boardSize - 2 && m_board[r + 1][c + 1].GetPieceColor() == Piece::Color::BLUE && m_board[r + 2][c + 2].GetPieceColor() == Piece::Color::INVALID) {
                        hasMoves = true;
                    }
                }
                if (r > 1 && c > 1 && m_board[r - 1][c - 1].GetPieceColor() == Piece::Color::BLUE && m_board[r - 2][c - 2].GetPieceColor() == Piece::Color::INVALID) {
                    hasMoves = true;
                }
                if (r > 1 && c < m_boardSize - 2 && m_board[r - 1][c + 1].GetPieceColor() == Piece::Color::BLUE && m_board[r - 2][c + 2].GetPieceColor() == Piece::Color::INVALID) {
                    hasMoves = true;
                }
            }
        }
    }

    if (!hasMoves) {
        for (int r = 0; r < m_boardSize; ++r) {
            for (int c = 0; c < m_boardSize; ++c) {
                if (m_board[r][c].GetPieceColor() == Piece::Color::RED) {
                    if (m_board[r][c].IsKing()) {
                        if (r < m_boardSize - 1 && c > 0 && m_board[r + 1][c - 1].GetPieceColor() == Piece::Color::INVALID) {
                            hasMoves = true;
                        }
                        if (r < m_boardSize - 1 && c < m_boardSize - 1 && m_board[r + 1][c + 1].GetPieceColor() == Piece::Color::INVALID) {
                            hasMoves = true;
                        }
                    }
                    if (r > 0 && c > 0 && m_board[r - 1][c - 1].GetPieceColor() == Piece::Color::INVALID) {
                        hasMoves = true;
                    }
                    if (r > 0 && c < m_boardSize - 1 && m_board[r - 1][c + 1].GetPieceColor() == Piece::Color::INVALID) {
                        hasMoves = true;
                    }
                }
            }
        }
    }

    return hasMoves;
}
