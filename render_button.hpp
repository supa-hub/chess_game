#ifndef RENDER_BUTTON
#define RENDER_BUTTON

#include "backend/helper_tools.hpp"
#include <stdint.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <windows.h>


// simple function to abstract the creation of a window button
inline void display_button(std::string text, const int32_t& x, const int32_t& y, HWND hwnd)
{
    std::string standard = "BUTTON";
    std::wstring standard1 = std::wstring(standard.begin(), standard.end());

    HWND button;

    // we modify the text into an array of characters that can be given to the CreateWindow function.
    std::wstring text1 = std::wstring(text.begin(), text.end());

    button = CreateWindow(standard1.c_str(),
                        text1.c_str(),
                        WS_VISIBLE | WS_CHILD | WS_BORDER | BS_LEFT,
                        x, y, a_button.width, a_button.height,
                        hwnd, (HMENU) 1, NULL, NULL
                        );

    if ( a_button.buttons.size() ) {
        DestroyWindow(a_button.buttons.front());
        a_button.buttons.pop_front();
    }

    a_button.buttons.push_back( button );
}


#endif