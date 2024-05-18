#include "backend/helper_tools.hpp"
#include <stdint.h>
#include <tchar.h>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <windows.h>
#include <numeric>

inline void display_text(std::string text, const int32_t& x, const int32_t& y, HWND hwnd)
{   
    std::string standard = "STATIC";
    std::wstring standard1 = std::wstring(standard.begin(), standard.end());
    
    
    // we modify the text into an array of characters that can be given to the CreateWindow function.
    std::wstring text1 = std::wstring(text.begin(), text.end());


    HWND textfield;

    textfield = CreateWindow(standard1.c_str(), text1.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, x, y, text_field.width - 50, text_field.height, hwnd, NULL, NULL, NULL );

    if ( text_field.boxes.size() ) {
        // delete the old child window
        DestroyWindow(text_field.boxes.front());
        text_field.boxes.pop_front();
    }

    text_field.boxes.push_back(textfield);
}


inline void display_all_text(const int32_t& x, const int32_t& y, HWND hwnd)
{
    std::string all_text = std::reduce(text_field.text.begin(), text_field.text.end(), std::string(""));


    display_text(all_text, render_state.width + 20, 0, hwnd);
}