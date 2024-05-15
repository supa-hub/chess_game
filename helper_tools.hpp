#ifndef HELPERTOOLS
#define HELPERTOOLS

#include <algorithm>
#include <stdint.h>
#include <memory>
#include <vector>
#include <cmath>


struct coordinates 
{
    int x = 0;
    int y = 0;
    
    
    //add a constructor for std::make_unique. Modified parameter names for clarity
    coordinates(int x1 = 0, int y1 = 0) noexcept : x(x1), y(y1) {}


    coordinates(const coordinates& a) noexcept : x(a.x), y(a.y) {}

    // we create some basic operations inside the struct
    inline coordinates operator + (coordinates a)
    {
        return { x+a.x, y+a.y };
    }

    inline coordinates operator - (coordinates a)
    {
        return { x-a.x, y-a.y };
    }

    inline bool operator == (coordinates a)
    {
        return { x == a.x && y == a.y };
    }

    inline bool operator != (coordinates a)
    {
        return { x != a.x || y != a.y };
    }


    inline coordinates operator * (int a)
    {
        return { x*a, y*a };
    }

    // increment is a method which only 
    // raises or lowers the value if its not 0.
    // If the value is < 0, it lowers it,
    // if its > 0, it raises it.
    void increment()
    {   
        // basically here we calculate:  x = x + (-1)^(1 + 0 or 1), which will give us x+1 or x-1,
        if ( x != 0 ) x = x + pow(-1, 1 + (x > 0));
        if ( y != 0 ) y = y + pow(-1, 1 + (y > 0));
    }

    /*
    inline bool opetator > (coordinates a)
    {
        return 
    }*/


};



// we create a clamp function to only choose the value if its
// in the accepted range
template <typename T>
inline T clamp(const T& value, const T& smallest, const T& largest) noexcept
{
    return std::min(largest, std::max(value, smallest));
}

 

enum pieces
{
    PAWN = 1,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,

    PIECES_COUNT
};

enum color_id
{
    WHITE,
    BLACK,
    GREY,
    RED,
    GREEN,
    BLUE,



    COLOR_COUNT
};


inline coordinates square_to_pos(const coordinates& coords, const int32_t& screen_width, const int32_t& screen_height, const bool& use_clamp)
{
    int32_t x = coords.x;
    int32_t y = coords.y;
    
    if ( use_clamp ) {
        x = clamp<int32_t>(x, 0, 7);
        y = clamp<int32_t>(y, 0, 7);
    }
    

    int square_width = screen_width/8;
    int square_height = screen_height/8;

    int x1 = x*square_width;
    int y1 = y*square_height;

    return coordinates{x1, y1};
}



class RGB_t
{
    public:
        uint32_t red = 0;
        uint32_t green = 0;
        uint32_t blue = 0;

        /*
        RGB(uint32_t red1, uint32_t green1, uint32_t blue)
        {
            this->red = red1;
            this->green = green1;
            this->blue = blue1;
        } */


        inline uint64_t to_hex()
        {
            return 65536 * this->red + 256*this->green + this->blue;
        }

        inline RGB_t operator + ( RGB_t color )
        {
            return {this->red + color.red, this->green + color.green, this->blue + color.blue};
        }

        inline RGB_t operator - ( RGB_t color )
        {
            return {this->red - color.red, this->green - color.green, this->blue - color.blue};
        }
};



/*
inline std::vector< std::unique_ptr<coordinates> > kings_possible_moves(std::weak_ptr<Board> board_ptr)
{

}
*/



// here we calculate if any of the given 2 vectors is a multiple of the other vector.
inline bool same_direction(coordinates a, coordinates b)
{   


    // with this if-statement we check the y-axis for possible moves
    if ( b.y == 0 && a.y == 0 ) {
        if ( b.x > 0 && a.x > 0 ) {
            return b.x >= a.x;
        }
        
        else if ( b.x < 0 && a.x < 0 ) {
            return b.x <= a.x;
        }
    }


    // we check the x-axis for possible moves
    if ( b.x == 0 && a.x == 0 ) {
        if ( b.y > 0 && a.y > 0 ) {
            return b.y >= a.y;
        }
        
        else if ( b.y < 0 && a.y < 0 ) {
            return b.y <= a.y;
        }

    }

    // in these if-statements we check the diagonals for possible moves.
    if ( b.y > 0 && a.y > 0 && b.x > 0 && a.x > 0) {
        return ( b.y >= a.y ) && (b.x >= a.x);
    }

    else if ( b.y < 0 && a.y < 0 && b.x < 0 && a.x < 0) {
        return ( b.y <= a.y ) && (b.x <= a.x);
    }
    else if ( b.y > 0 && a.y > 0 && b.x < 0 && a.x < 0) {
        return ( b.y >= a.y ) && (b.x <= a.x);
    }
    else if ( b.y < 0 && a.y < 0 && b.x > 0 && a.x > 0) {
        return ( b.y <= a.y ) && (b.x >= a.x);
    }


    return false;
}



#endif