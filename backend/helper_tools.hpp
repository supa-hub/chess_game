#ifndef HELPERTOOLS
#define HELPERTOOLS

#include <algorithm>
#include <stdint.h>
#include <memory>
#include <vector>
#include <cmath>
#include <string>
#include <array>

#ifdef RGB
#undef RGB
#endif

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


// create a namespace to stop any namespace errors
namespace helper
{

static std::array<std::string, 8> chess_letters = {"a", "b", "c", "d", "e", "f", "g", "h"};

template<typename T>
struct coordinates 
{
    int64_t x = 0;
    int64_t y = 0;
    

    //add a constructor for std::make_unique. Modified parameter names for clarity
    coordinates( T x1 = 0, T y1 = 0) noexcept : x( static_cast<int64_t>(x1) ), y( static_cast<int64_t>(y1) ) { } 


    coordinates(const coordinates& a) noexcept : x(a.x), y(a.y) { }

    // we create some basic operations inside the struct
    inline coordinates operator + (coordinates a) noexcept
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


    inline coordinates operator * (int32_t a)
    {
        return { x*a, y*a };
    }

    inline std::string toString()
    {
        return std::to_string(x) + " " + std::to_string(y);
    }

    inline std::string toChesstring()
    {
        // We get the letter for the x axis and we also clamp the value so we dont get segfault.
        return chess_letters[std::clamp<int64_t>(x, 0, 7)] + std::to_string(y+1);
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



};



// we create a clamp function to only choose the value if its
// in the accepted range
template <typename T>
constexpr inline T clamp(const T& value, const T& smallest, const T& largest) noexcept
{
    return std::min(largest, std::max(value, smallest));
}







inline coordinates<int64_t> square_to_pos(const coordinates<int64_t>& coords, const int32_t& screen_width, const int32_t& screen_height, const bool& use_clamp)
{
    int64_t x = coords.x;
    int64_t y = coords.y;
    
    if ( use_clamp ) {
        x = clamp<int64_t>(x, 0, 7);
        y = clamp<int64_t>(y, 0, 7);
    }
    

    int square_width = screen_width/8;
    int square_height = screen_height/8;

    int64_t x1 = x*square_width;
    int64_t y1 = y*square_height;

    return coordinates{x1, y1};
}


class RGB
{   
    private:
        /*
         The following hex values we'll use in bitwise operations to modify the RGB values into a integer and vise-versa,
         the compiler should replace calling these variables with just their values.

         By making them static we use the same values for every RGB_t class, so we save alot of memory
        */
        static const uint32_t red_hex = 0xff0000;
        static const uint32_t green_hex = 0x00ff00;
        static const uint32_t blue_hex = 0x0000ff;
        

    public:

        uint32_t color = 0;


        // define some constructors
        constexpr RGB() noexcept { }

        constexpr RGB(uint32_t red, uint32_t green, uint32_t blue) noexcept : color( ( red << 16 ) + ( green << 8 ) + blue ) { } 

        constexpr RGB(uint32_t hex) noexcept
        { 
            color = hex;
        } 

        
        inline uint32_t get_int() noexcept
        {
            return color;
        }

        inline uint32_t red() noexcept
        {
            return  ( color & red_hex ) >> 16;
        }

        inline uint32_t green() noexcept
        {
            return  ( color & green_hex ) >> 8;
        }

        inline uint32_t blue() noexcept
        {
            return  ( color & blue_hex );
        }

        // assign certain color values
        // assign the red value
        inline bool red(uint32_t num) noexcept
        {
            color = ( color & ~red_hex ) | ( ( num << 16 ) & red_hex );

            return true;
        }

        // assign the green value
        inline bool green(uint32_t num) noexcept
        {
            color = ( color & ~green_hex ) | ( ( num << 8 ) & green_hex );

            return true;
        }

        // assign the blue value
        inline bool blue(uint32_t num) noexcept
        {
            color = ( color & ~blue_hex ) | ( num & blue_hex );

            return true;
        }


        inline RGB operator + ( RGB color ) noexcept 
        {
            return { this->red() + color.red(), this->green() + color.green(), this->blue() + color.blue() };
        }

        inline RGB operator - ( RGB color ) noexcept
        {
            return { this->red() - color.red(), this->green() - color.green(), this->blue() - color.blue() };
        }

        inline RGB operator = ( uint32_t num ) noexcept
        {
            return { this->to_rgb(num) };
        }


        inline RGB to_rgb( uint32_t hex ) noexcept
        {
            return { color = hex };
        }



        template<typename T>
        inline bool operator < ( T value )
        {
            return { this->get_int() < value };
        }


};






// here we calculate if any of the given 2 vectors is a multiple of the other vector.
template<typename T>
inline bool same_direction(coordinates<T> a, coordinates<T> b)
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

}

#endif