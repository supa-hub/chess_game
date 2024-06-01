#include "backend/helper_tools.hpp"
#include <stdint.h>
#include <tchar.h>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <windows.h>
#include <numeric>


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
                        WS_VISIBLE | WS_CHILD | WS_BORDER,
                        x, y, 80, 30,
                        hwnd, (HMENU) 1, NULL, NULL
                        );
}