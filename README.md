# Chess game

A C++ program that runs a simple chess game by using Windows.h functions for rendering and getting user inputs. <br />
The backend code for creating and handling a chess game are self-made.

## Compile method
By running the following command in cmd, you can compile the code into a working executable: <br />
### g++ gui.cpp -lgdi32 -municode -O2 -o gui.exe


## Compatibility

Development and testing platforms: <br /> <br/>

OS: Windows 10  22H2. <br />
CPU: intel i7-1165g7 <br />
code version: C++17. <br />
compiler: gcc 12.2.0. <br /> <br />

NOTE: because the code uses the windows.h header, it is not compatible with Macos or Linux. <br />


## benchmark
fps: 1500 - 8000  (depends on how many objects are drawn and the size of the window) <br />
cpu load: 11% - 12% <br />

NOTE: currently there is no fps cap, so the program will use one cpu core at 100%. In future updates there will be a fps cap added.




