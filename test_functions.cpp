#include "backend/helper_tools.hpp"
#include <stdint.h>
#include <memory>
#include <vector>
#include "backend/board.hpp"
#include "backend/square.hpp"
#include "backend/chess_piece.hpp"


inline void render_at_pos1(std::unique_ptr<std::vector<uint32_t>> pixels, int32_t x0, int32_t y0, const int64_t& width, const int64_t& height)
{
    //x0 = clamp<int>(x0, 0, render_state.width);
    //y0 = clamp<int>(y0, 0, render_state.height);

    if ( (*pixels).size() == 0) return;

    uint32_t index = 0;


    for ( int64_t y = y0; y < height+y0; y++ ) {
        uint32_t* pixel = static_cast<uint32_t*>(render_state.memory) + x0 + y*render_state.width;
        for ( int64_t x = x0; x < width+x0; x++ ) {
            
            if ( x + y*render_state.width >= render_state.width*render_state.height || x + y*render_state.width < 0) {
                break;
                //pixels++;
                //continue;
            }

            if ( x < 0 ||x > render_state.width ) {
                index++;
                continue;
            }


            // this if statement makes the bitmap "sort of " transparent,
            // by making the images pixels the same color as the backround
            //if ( *pixels < 16200000 && *pixel == 0x000000 ) {
            if ( (*pixels)[index] < 16000000  ) {
                //*pixel++ = 0x000000;
                //pixels++;
                *pixel++ = (*pixels)[index];
                index++;
            }
            else {
                index++;
                pixel++;
            }
            /*
            else {
                *pixel++ = *pixels++;
            }*/
            
            
        }
    }
}