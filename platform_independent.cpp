#include <array>
#include <string>

struct Button_State
{
    bool is_down;
    bool changed;
};


enum 
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MLEFT, // this is the mouse left button.

    BUTTON_COUNT,
};


struct Input 
{

    std::array<Button_State, BUTTON_COUNT> buttons;

};


// this function returns the window position relative to the display.
void WindowPosAbsolute(HWND& window, RECT* rect ) 
{
    GetClientRect(window, rect);
    MapWindowPoints(window, GetParent(window), (LPPOINT)rect, 2);
}



// we use this function to load up all of the images for the chess pieces
HANDLE getImageData(std::string filename)
{   
    std::string full_name = "chess_pieces\\" + filename + ".bmp";

    // we convert the std::string int a std::wstring because that is the
    // way to get a const wchar_t*
    std::wstring full_name1 = std::wstring(full_name.begin(), full_name.end());

    return LoadImageW(
        NULL,
        full_name1.c_str(),
        IMAGE_BITMAP,
        0,
        0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION
    );
}


struct 
{
    HANDLE pawn = getImageData("pawn1");
    HANDLE knight = getImageData("knight1");
    HANDLE bishop = getImageData("bishop1");
    HANDLE rook = getImageData("rook");
    HANDLE queen = getImageData("queen");
    HANDLE king = getImageData("king");
    HANDLE pawn_bl = getImageData("pawn_bl");
    HANDLE knight_bl = getImageData("knight_bl");
    HANDLE bishop_bl = getImageData("bishop_bl");
    HANDLE rook_bl = getImageData("rook_bl");
    HANDLE queen_bl = getImageData("queen_bl");
    HANDLE king_bl = getImageData("king_bl");
    HANDLE green_ball = getImageData("green_ball");
    HANDLE wking_check = getImageData("king_in_check_test");

} pieces;

