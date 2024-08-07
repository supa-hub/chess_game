#include "backend/helper_tools.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>
#include "backend/board.hpp"
#include "backend/square.hpp"
#include "backend/chess_piece.hpp"
#include <iostream>


inline void render_backround()
{
    unsigned int* pixel = static_cast<unsigned int*>(render_state.memory);
    for ( int y = 0; y < render_state.height; y++ ) {
        for ( int x = 0; x < render_state.width; x++ ) {
            *pixel++ = static_cast<unsigned int>(x*y);
        }
    }
}


inline void clear_screen(unsigned int color)
{
    //unsigned int* pixel = (unsigned int*)render_state.memory;
    unsigned int* pixel = static_cast<unsigned int*>(render_state.memory); // we use static_cast for safety
    for ( int y = 0; y < render_state.height; y++ ) {
        for ( int x = 0; x < render_state.width; x++ ) {
            *pixel++ = color;
        }
    }
}



inline void draw_filled_circle(int x0, int y0, uint32_t color)
{
    x0 = helper::clamp<int>(x0, 0, render_state.width);
    y0 = helper::clamp<int>(y0, 0, render_state.height);
}

/*
the main function that writes into memory the color values of each pixel
of the rectangle that is drawn onto the window.

x0, y0: the starting position the rectangle.
x1, y1: the end position of the rectangle.

NOTE: it is  simpler to call the draw_rect() function which abstracts a lot of the calculations.
*/
inline void draw_filled_rect(int x0, int y0, int x1, int y1, uint32_t color)
{
    
    //unsigned int* pixel = (unsigned int*)render_state.memory;
    x0 = helper::clamp<int>(x0, 0, render_state.width);
    x1 = helper::clamp<int>(x1, 0, render_state.width);
    y0 = helper::clamp<int>(y0, 0, render_state.height);
    y1 = helper::clamp<int>(y1, 0, render_state.height);
    for ( int y = y0; y < y1; y++ ) {
        //unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y*render_state.width;
        unsigned int* pixel = static_cast<unsigned int*>(render_state.memory) + x0 + y*render_state.width;
        for ( int x = x0; x < x1; x++ ) {
            *pixel++ = color;
        }
    }
}


// basically a simplified version of draw_rect()
inline void draw_square(int& x, int& y, int& width, uint32_t color)
{

    int x0 = x - width;
    int x1 = x + width;
    int y0 = y - width;
    int y1 = y + width;

    draw_filled_rect(x0, y0, x1, y1, color);

}

// this function draws a rectangle of the given dimensions onto the window
inline void draw_rect(int& x, int& y, int& width, int& height, uint32_t color)
{
    
    int x0 = x - width;
    int x1 = x + width;
    int y0 = y - height;
    int y1 = y + height;

    draw_filled_rect(x0, y0, x1, y1, color);

}


// this will contain the position of the square that will be colored red
static helper::coordinates<int64_t> redSquare;


/*
 this function draws all the squares of the chessboard

 width: the width of the chessboard
 height: the height of the chessboard
*/
inline void draw_chessboard(int width, int height)
{
    int square_width = width/8;
    int square_height = height/8;

    // this is where we start drawing the first square
    int first_x = square_width;
    int first_y = 0;

    // we draw every square with this nested loop
    for ( int i = 0; i < 9; i++ ) {
        
        // the modulo operator ensures that we draw every square 
        // black and white in cycles
        for ( int j = i%2; j < 8 + i%2; j++ ) {
            
            switch (j%2) {
                case 0: 
                    draw_rect(first_x, first_y, square_width, square_height, 0xffffff);
                    break;
                case 1:
                    draw_rect(first_x, first_y, square_width, square_height, 0x000000);
                    break;
                default:
                    break;
            }

            /*
             with this if-statement we draw a red square if the player has clicked it
             because the location of the mouse is calculated from the top left corner
             the if-statement looks like this
            */
            if ( redSquare.x < first_x && redSquare.x > first_x-square_width 
                && redSquare.y < first_y && redSquare.y > first_y-square_height
                )  {
                    draw_rect(first_x, first_y, square_width, square_height, 0xff0000);  
                }
            

            first_x += square_width;
        }
        
        first_y += square_height;
        first_x = square_width;
        
    }
}

/*
 We use this function to render a given image onto the screen.
 I removed this part from render_image() to simplify the code.

 pixels0: the std::vector that contains our color values for each pixel of the image

 x0, y0: the starting position of the image

 width: the width of the image

 height: the height of the image

 NOTE: I recommend that you do NOT DIRECTLY use this function and use the render_image() function, which uses this function
 but simplifies your job.
*/
inline void render_at_pos(const std::vector<uint32_t>& pixels0, int32_t x0, int32_t y0, const int64_t& width, const int64_t& height)
{

    // the first pixel value of the array
    std::vector<uint32_t>::const_iterator pixels = pixels0.cbegin();
    

    for ( int64_t y = y0; y < height+y0; y++ ) {
        uint32_t* pixel = static_cast<uint32_t*>(render_state.memory) + x0 + y*render_state.width;
        for ( int64_t x = 0; x < width; x++ ) {
            
            if ( x + y*render_state.width >= render_state.width*render_state.height || x + y*render_state.width < 0) {
                return;
            }

            else if ( x + x0 + y*render_state.width >= render_state.width*render_state.height || x + y*render_state.width < 0) {
                return;
            }

            else if ( x + x0 + y*render_state.width >= render_state.width*render_state.height || x + y*render_state.width < 0) {
                return;
            }

            if ( x < 0 || x > render_state.width ) {
                pixels++;
                continue;
                
            }


            // this if statement makes the bitmap "sort of " transparent,
            // by making the images pixels the same color as the backround
            //if ( *pixels < 16200000 && *pixel == 0x000000 ) {
            if ( *pixels < 16000000  ) {

                *pixel++ = *pixels++;
            }
            else {
                pixels++;
                pixel++;
            }
            
            
        }
    }

}


// we use this function to render images onto the window.
inline void render_image(const HANDLE image, const int32_t& x, const int32_t& y, bool invert = false)
{
    if ( image == NULL ) return;

    BITMAP bm; // a buffer containing the information of our image
    GetObject(image, sizeof(bm), static_cast<LPVOID>(&bm));//get bitmap dimension



    //LONG bits_per_pixel = bm.bmBitsPixel; // indicates how many bits are required to tell one pixel
    WORD bytes_per_scan_line = bm.bmWidthBytes;

    //BYTE* image_data = (BYTE*)bm.bmBits;
    BYTE* image_data = static_cast<BYTE*>(bm.bmBits);

    LONG image_height = bm.bmHeight;
    LONG image_width = bm.bmWidth;



    // because of how the image scanning is done ( because of CPU architecture ),
    // some rows have an additional pixel.
    int64_t remainder_of_row = bytes_per_scan_line % image_width; 

    std::vector<uint32_t> bytes(static_cast<size_t>(image_width * image_height));

    uint32_t count = 0;

    helper::RGB rgb{};


    if ( image_data == NULL ) return;


    // we use nested loops for easier debugging
    for ( int64_t y0 = 0; y0 < image_height; y0++ ) {
        for ( int64_t y1 = 0; y1 < image_width; y1++ ) {
            

            for ( uint32_t j = 0; j < 3; j++ ) {
                switch ( j ) {
                    case 0: 
                        rgb.blue( *image_data++ );
                        break;

                    case 1: 
                        rgb.green( *image_data++ );
                        break;

                    case 2: 
                        rgb.red( *image_data++ );
                        break;

                    default:
                        break;
                }

            }

            //rgb = 65536 * red + 256*green + blue;


            // we invert the colours if the piece is black 
            if ( invert && rgb < 16000000u ) { rgb = 0xffffff - rgb.get_int(); }
            
            bytes[count++] = rgb.get_int();

        }

        /*
        Below we skip over the padding bytes that are added into our image
        if the image is not word aligned, meaning that our image width
        is not divisible by 4.

        in this statement, we have arrived to the padding bytes (if there are any), 
        and we increment the pointer to the byte value by how many padding bytes we have, 
        so this way we jump over them.
        */
        image_data += remainder_of_row;
    }



    //std::vector<uint32_t>::iterator all_bytes = bytes.begin(); // we pass an iterator so we can go through each pixel easily.
    render_at_pos(bytes, x, y, image_width, image_height);
}




struct rendered_picture
{
    int64_t width = 0;
    int64_t height = 0;

    std::vector<uint32_t> begin;
};

// we use this function to render some frequently used pictures
// and store them in memory
inline rendered_picture render_image(HANDLE image, bool invert = false)
{
    rendered_picture picture;
    if ( image == NULL ) return picture;

    BITMAP bm; // a buffer containing the information of our image
    GetObject(image, sizeof(bm), static_cast<LPVOID>(&bm));//get bitmap dimension
    


    ///LONG bits_per_pixel = bm.bmBitsPixel; // indicates how many bits are required to tell one pixel
    WORD bytes_per_scan_line = bm.bmWidthBytes;

    //BYTE* image_data = (BYTE*)bm.bmBits;
    BYTE* image_data = static_cast<BYTE*>(bm.bmBits);

    LONG image_height = bm.bmHeight;
    LONG image_width = bm.bmWidth;
    
    // because of how the image scanning is done ( because of CPU architecture ),
    // some rows have an additional pixel.
    int64_t remainder_of_row = bytes_per_scan_line % image_width; 



    // this array will store the image bytes.
    //uint64_t length_of_bmBits = (image_width * image_height)*bits_per_pixel/8;

    //uint32_t bytes[(image_width * image_height)] = {0};

    std::vector<uint32_t> bytes(static_cast<size_t>(image_width * image_height));

    uint32_t count = 0;

    helper::RGB rgb{};
    


    if ( image_data == NULL ) return picture;
    

    // in this for loop we combine the bits that make up one byte and we add
    // all of the bytes into one array

    // we use nested loops for easier debugging
    for ( int64_t y = 0; y < image_height; y++ ) {
        for ( int64_t y1 = 0; y1 < image_width; y1++ ) {
            

            for ( uint32_t j = 0; j < 3; j++ ) {
                switch ( j ) {
                    case 0: 
                        rgb.blue( *image_data++ );
                        break;

                    case 1: 
                        rgb.green( *image_data++ );
                        break;

                    case 2: 
                        rgb.red( *image_data++ );
                        break;

                    default:
                        break;
                }

            }

            //rgb = 65536 * red + 256*green + blue;
            

            

            // we invert the colours if the piece is black 
            if ( invert && rgb < 16000000u ) { rgb = 0xffffff - rgb.get_int(); }
            
            bytes[count++] = rgb.get_int();


            //red = *image_data++;
            

        }

        /*
        Below we skip over the padding bytes that are added into our image
        if the image is not word aligned, meaning that our image width
        is not divisible by 4.

        in this statement, we have arrived to the padding bytes (if there are any), 
        and we increment the pointer to the byte value by how many padding bytes we have, 
        so this way we jump over them.
        */
        image_data += remainder_of_row;
    }
    


    //image_width = clamp<uint32_t>(image_width, 0, additional_width);
    //image_height = clamp<uint32_t>(image_height, 0, additional_height);


    picture.width = image_width;
    picture.height = image_height;
    picture.begin = bytes;

    
    return picture;
}


/*
 We render the images beforehand so we dont have to 
 load the images and calculate their pixel
 values every frame. Then we can just get the
 values from memory.
*/
struct 
{
    rendered_picture Pawn = render_image(pieces.pawn);
    rendered_picture Bishop = render_image(pieces.bishop);
    rendered_picture Knight = render_image(pieces.knight);
    rendered_picture Rook = render_image(pieces.rook);
    rendered_picture Queen = render_image(pieces.queen);
    rendered_picture King = render_image(pieces.king);
    rendered_picture Pawn_bl = render_image(pieces.pawn_bl);
    rendered_picture Knight_bl = render_image(pieces.knight_bl);
    rendered_picture Bishop_bl = render_image(pieces.bishop_bl);
    rendered_picture Rook_bl = render_image(pieces.rook_bl);
    rendered_picture Queen_bl = render_image(pieces.queen_bl);
    rendered_picture King_bl = render_image(pieces.king_bl);
    rendered_picture greenBall = render_image(pieces.green_ball);
    rendered_picture wKing_check = render_image(pieces.wking_check);
    rendered_picture blKing_check = render_image(pieces.blking_check);
} rendered_images;





/*
 this function draws all the pieces that are left on the board
 this code was part of the main function that ran the program,
 but I removed it and made it its own function for simplicity.
*/
inline void draw_pieces(const std::weak_ptr<Board> board_ptr)
{
    std::weak_ptr<Square> a_square = std::weak_ptr<Square>();

    int32_t x1 = 0;
    int32_t y1 = 0;
    //HANDLE* hImg_ptr = NULL;
    rendered_picture picture;


    if ( board_ptr.expired() ) return;

    for ( int32_t x = 0; x < 8; x++ ) {
        x1 = x*(render_state.width/8);
        for ( int32_t y = 0; y < 8; y++ ) {

            y1 = y*(render_state.height/8);

            a_square = ((board_ptr.lock())->get_square( helper::coordinates<int64_t>{x, y}));
            std::weak_ptr<Piece> a_piece = (a_square.lock())->get_piece();

            if ( !(a_piece.expired()) ) {
                    
                // we render the pieces onto the window
                // we use the already calculated arrays of the pictures
                switch (a_piece.lock()->tell_id()) {
                    
                    case PAWN:
                        //hImg_ptr = &pieces.pawn;
                        picture = rendered_images.Pawn;
                        break;
                    
                    
                    case KNIGHT:
                        //hImg_ptr = &pieces.knight;
                        picture = rendered_images.Knight;
                        break;
                    
                    case BISHOP:
                        //hImg_ptr = &pieces.bishop;
                        picture = rendered_images.Bishop;
                        break;

                    case ROOK:
                        //hImg_ptr = &pieces.rook;
                        picture = rendered_images.Rook;
                        break;
                    
                    case QUEEN:
                        //hImg_ptr = &pieces.queen;
                        picture = rendered_images.Queen;
                        break;

                    case KING:
                        //hImg_ptr = &pieces.king;

                        // if the king is in check, then we draw a different picture, if its not in check, then 
                        // we draw the normal king piece
                        if ( board_ptr.lock()->is_check(a_piece.lock()->tell_color()) ) 
                            picture = rendered_images.wKing_check;
                        else { picture = rendered_images.King; }
                        break;

                    case PAWN*10:
                        picture = rendered_images.Pawn_bl;
                        break;
                    
                    case KNIGHT*10:
                        picture = rendered_images.Knight_bl;
                        break;
                    
                    case BISHOP*10:
                        picture = rendered_images.Bishop_bl;
                        break;
                    
                    case ROOK*10:
                        picture = rendered_images.Rook_bl;
                        break;
                    
                    case QUEEN*10:
                        picture = rendered_images.Queen_bl;
                        break;
                    
                    case KING*10:

                        if ( board_ptr.lock()->is_check(a_piece.lock()->tell_color()) ) picture = rendered_images.blKing_check;
                        else { picture = rendered_images.King_bl; }
                        break;

                    

                    default:
                        picture = rendered_images.Pawn;
                        break;
                }
                


                //render_image(hImg_ptr, x1, y1, invert = invert);
                render_at_pos(picture.begin, x1, y1, picture.width, picture.height);
            }
            
        }
    }
}

inline void drawRedSquare(int32_t x, int32_t y)
{
    if ( x == 0 && y == 0 ) return; 

    redSquare.x = x;
    redSquare.y = y;

}


// this method renders all the green squares where the piece can move
inline void drawPossibleMoves( const std::vector< helper::coordinates<int64_t> >& moves, const helper::coordinates<int64_t>& current, int32_t screen_width, int32_t screen_height)
{


    // base values
    helper::coordinates<int64_t> aux = current;

    int32_t square_width = screen_width/8;
    int32_t square_height = screen_height/8;

    std::vector<helper::coordinates<int64_t>> directions_cannot_go;

    
    helper::coordinates<int64_t> aux0 = square_to_pos(current, square_width*8, square_height*8, false);



    //render_image(&pieces.green_ball, aux.x, aux.y);

    for ( const helper::coordinates<int64_t>& a_move : moves ) {
        aux = aux0 + square_to_pos(a_move, square_width*8, square_height*8, false);


        render_image(pieces.green_ball, aux.x, aux.y);
        
    }
}





inline void drawPossibleMoves1( const std::vector< helper::coordinates<int64_t> >& moves, const helper::coordinates<int64_t>& current, int32_t screen_width, int32_t screen_height)
{

    helper::coordinates<int64_t> aux = current;

    int32_t square_width = screen_width/8;
    int32_t square_height = screen_height/8;

    std::vector<helper::coordinates<int64_t>> directions_cannot_go;

    // base values
    helper::coordinates<int64_t> aux0 = square_to_pos(current, square_width*8, square_height*8, false);


    for ( const helper::coordinates<int64_t>& a_move : moves ) {
        aux = aux0 + square_to_pos(a_move, square_width*8, square_height*8, false);

        //rendered_picture aa = rendered_images.greenBall;
        render_at_pos(rendered_images.greenBall.begin, aux.x, aux.y, rendered_images.greenBall.width, rendered_images.greenBall.height);
        
    }
}




inline void drawPossibleMoves2( const std::vector< coordinate_ptr >& moves, const helper::coordinates<int64_t>& current, int32_t screen_width, int32_t screen_height)
{

    helper::coordinates<int64_t> aux = current;

    int square_width = screen_width/8;
    int square_height = screen_height/8;

    std::vector<helper::coordinates<int64_t>> directions_cannot_go;

    // base values
    helper::coordinates<int64_t> aux0 = square_to_pos(current, square_width*8, square_height*8, false);


    for ( const coordinate_ptr& a_move : moves ) {
        aux = aux0 + square_to_pos(*a_move, square_width*8, square_height*8, false);

        //rendered_picture aa = rendered_images.greenBall;
        render_at_pos(rendered_images.greenBall.begin, aux.x, aux.y, rendered_images.greenBall.width, rendered_images.greenBall.height);
        
    }
}


