#ifndef UNICODE
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <deque>

static bool running = true;

struct renderState {
    int height;
    int width;
    void* memory;

    BITMAPINFO bitmap_info;
};

struct textField {
    int32_t height = 550;
    int32_t width = 300;

    std::deque<std::string> text = {"aamuja\n", "moro\n", "test\n"};  // we use deque for more efficient value storing compared to std::vector.

    std::deque<HWND> boxes; // will store all the created text windows that are created and will delete the old ones.
};



renderState render_state;
textField text_field;

HBITMAP hBitmap = NULL;


#include "platform_independent.cpp"
#include "renderer.cpp"
#include "render_text.hpp"
#include "board.hpp"
#include "game.hpp"

std::shared_ptr<Board> board_ptr;

#define process_button(b, vk)\
case vk: {\
    input.buttons[b].is_down = is_down; \
    input.buttons[b].changed = true; \
} break;


/*
// we create a macro to shorten the function call
#define piece_possible_moves(square)  board_ptr->find_possible_tiles_to_move_to( \
                    square.lock()->get_piece().lock()->possible_moves(), \
                    square.lock()->coordinates(), \
                    render_state.width, \
                    render_state.height,\
                    square.lock()->get_piece().lock()->tell_color()\
                );
*/


extern const UINT_PTR IDT_TIMER1 = 0;
long count = 0;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    LRESULT result = 0;

    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {

            PostQuitMessage(0);
            KillTimer(hwnd, IDT_TIMER1);
            running = false;
        } break;

        case WM_TIMER: {
            case IDT_TIMER1: {
                // we use the below std::cout for displaying the total frames of the last 10 seconds
                std::cout << "count: " << count/10 << "\n";
                count = 0;
                return 0;
            }

        } break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            text_field.width = (rect.right - rect.left)/3;
            render_state.width = rect.right - rect.left - text_field.width;
            render_state.height = rect.bottom - rect.top;

            int size = render_state.width * render_state.height * sizeof(unsigned int);

            if ( render_state.memory ) { VirtualFree(render_state.memory, 0, MEM_RELEASE); }

            render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

            
            text_field.height = render_state.height;

            
            draw_chessboard(render_state.width, render_state.height);
            draw_pieces(board_ptr);
            display_all_text(600, 0, hwnd);
        } break;



    

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return result;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nShowCmd)
{
    Game game_object;
    
    board_ptr = game_object.new_game().lock();
    board_ptr->add_pieces();

    std::weak_ptr<Square> a_square = std::weak_ptr<Square>();

    std::weak_ptr<Square> clicked_square = std::weak_ptr<Square>();

    std::vector< coordinate_ptr > chosen_piece_moves = std::vector< coordinate_ptr >();

    coordinates square_pos;
    coordinates move_vec; // well use this to check if the piece can move to a new location
    sharedPiecePtr clicked_piece;

    //HBITMAP hBmp = (HBITMAP)LoadImage(NULL, _T("C:\\Users\\bilcu\\Downloads\\chess_pieces2.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    
    coordinates mouse_click;

    coordinates old_clicked_square;

    bool piece_just_moved = false; // checks whether we just moved a piece

    // we use these to get the position of the window
    RECT rect;


    // Create window class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Game Window Class";
    window_class.lpfnWndProc = window_callback;


    // Register class
    RegisterClass(&window_class);

    // create window
    HWND window = CreateWindowEx(
        0,
        window_class.lpszClassName, 
        L"test window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600 + text_field.width,
        600,
        0,
        0,
        hInstance,
        0 
    );

    if (window == NULL) {
        return 0;
    }


    

    ShowWindow(window, nShowCmd);




    HDC hdc = GetDC(window);

    Input input = {};

    unsigned long long button_state = 0;
    
    // we create a timer to calculate framerate
    SetTimer(window,             // handle to main window 
            IDT_TIMER1,          // timer identifier 
            10000,                // 10-second interval 
            (TIMERPROC) NULL);

    

    draw_chessboard(render_state.width, render_state.height);

    draw_pieces(board_ptr);

    display_all_text(600, 0, window);


    while (running) {

        MSG message;

        for ( int i = 0; i < BUTTON_COUNT; i++ ) {
            input.buttons[i].changed = false;
        }

        
        StretchDIBits(
            hdc, 
            0, 
            0, 
            render_state.width,
            render_state.height, 
            0, 
            0, 
            render_state.width,
            render_state.height,  
            render_state.memory, 
            &render_state.bitmap_info, 
            DIB_RGB_COLORS, 
            SRCCOPY);
        

        
        

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {

                case WM_KEYUP:
                case WM_KEYDOWN: {
                    unsigned int vk_code = (unsigned int)message.wParam;
                    bool is_down = ((message.lParam & ( 1 << 31 )) == 0);


                    switch (vk_code) {
                        case VK_UP: 
                            button_state = BUTTON_UP;
                            break;

                        case VK_DOWN: 
                            button_state = BUTTON_DOWN;
                            break;

                        case VK_LEFT: 
                            button_state = BUTTON_LEFT;
                            break;

                        case VK_RIGHT: 
                            button_state = BUTTON_RIGHT;
                            break;

                        
                    }

                    input.buttons[button_state].is_down = is_down; 
                    input.buttons[button_state].changed = true;

                } break;

                case WM_LBUTTONDOWN: {
                    POINT p;
                    GetCursorPos(&p);

                    
                    WindowPosAbsolute(window, &rect);

                    mouse_click.x = p.x - rect.left;
                    mouse_click.y = p.y - rect.top;

                    // we do this calculation so the mouse coordinates start 
                    // from the bottom left corner
                    mouse_click.y = render_state.height - mouse_click.y;


                    a_square = (board_ptr->get_square( board_ptr->convert_pos(mouse_click.x, mouse_click.y, render_state.width, render_state.height) ));


                    /*
                    we use these if-statements to click on chess pieces and move them around.
                    */
                    if ( !(clicked_square.expired()) && !(a_square.expired()) ) {
                        if ( clicked_square.lock()->coordinates() != a_square.lock()->coordinates() && 
                        !(clicked_square.lock()->get_piece().expired()) ) {
                            
                            // with this for loop we check if the square that we clicked on can be moved to by our piece,
                            // so basically if the square is in the possible moves.
                            clicked_piece = clicked_square.lock()->get_piece().lock();
                            for (  const coordinates& a_move : board_ptr->find_possible_tiles_to_move_to(
                                                                clicked_square.lock()->coordinates(),
                                                                clicked_piece
                                                                 ) ) {

                                // this is the difference between the clicked_square and a_square.
                                move_vec =  board_ptr->convert_pos(mouse_click.x, mouse_click.y, render_state.width, render_state.height) - clicked_square.lock()->coordinates();

                                if ( move_vec  == a_move ) {
                                    std::cout << clicked_square.lock()->get_piece().lock()->tell_name() << "\n";
                                    text_field.text.push_back( clicked_square.lock()->get_piece().lock()->tell_color() + " " + clicked_square.lock()->get_piece().lock()->tell_name() + "\n" );

                                    board_ptr->move_piece(clicked_square, a_square);

                                    display_all_text(600, 0, window);

                                    
                                    /*
                                    draw_chessboard(render_state.width, render_state.height);
                                    drawRedSquare(mouse_click.x, mouse_click.y);

                                    draw_pieces(board_ptr);*/

                                    piece_just_moved = true;
                                    break;
                                }
                            }

                            
                        }
                        clicked_square = a_square;

                        
                        
                    }

                    // this is basically for the first time we click.
                    else if ( clicked_square.expired() ) clicked_square = a_square;
                    

                    drawRedSquare(mouse_click.x, mouse_click.y);
                    draw_chessboard(render_state.width, render_state.height);
                    

                    draw_pieces(board_ptr);

                    
                } break;
                

                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
            
        }


        
        //render_backround();
        //draw_rect(100, 100, 50, 0x00ff22);
        
        
        if ( !(clicked_square.expired()) ) { 
            
            /*
             We do this check so we only have to render the possible moves
             when we click a new piece, this makes our program more efficient
             by only requiring us to render again when the player hsa done an action.

             Of course in a game with NPC characters this wouldnt work.
             */
            if ( !( old_clicked_square == clicked_square.lock()->coordinates() && clicked_square.lock()->coordinates() != a_square.lock()->coordinates() ) ) {
                if ( clicked_square.lock()->has_piece()) {
                        
                        // If we just moved a piece as white, then don't keep showing the white pieces moves.
                        // We'll wait until we click a black piece.
                        if ( !piece_just_moved ) {
                            clicked_piece = clicked_square.lock()->get_piece().lock();
                            std::vector<coordinates> can_go = board_ptr->find_possible_tiles_to_move_to(
                                clicked_square.lock()->coordinates(),
                                clicked_piece
                            );
                            
                            
                                drawPossibleMoves1(
                                    can_go, 
                                    clicked_square.lock()->coordinates(), 
                                    render_state.width, 
                                    render_state.height
                                );
   
                        }

                    }
                piece_just_moved = false;
                old_clicked_square = clicked_square.lock()->coordinates();
            }
            
        }
        
        
        
        count++;
        //std::cout << count << "\n";
            
    }




    return 0;
}