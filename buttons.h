#include <iostream>
#include <SDL2/SDL.h>

class button
{
    public:
    SDL_Rect buttonRect;
    string value;

    button(int x = 0, int y = 0, int h = 0, int w = 0) : buttonRect{x,y,h,w}
    {
        value = "null";
    };
        
};

class buttons
{
    public:
    button *number;

    buttons(int count)
    {
        number = new button[count];
    }

    ~buttons()
    {
        delete[] number;
    }
};

