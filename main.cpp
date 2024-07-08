using namespace std;
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <string>
#include <regex>

#include "buttons.h"
#include "text.h"
#include "calculator.h"
#include "misc.h"

#define FPS 60
#define FRAMEDELAY 1000 / FPS
#define WINDOW_WIDTH 250
#define WINDOW_HEIGHT 400
#define S_NULL " "
#define TOTAL_BUTTONS 17



bool event_manager(SDL_Event &_event, buttons &_calcbutton, string &_numbers, bool &_canCalculate, bool &_invsyntax)
{
    static int mouseX, mouseY;
    static bool clickMouse = false;
    static bool canPush = false;

    while (SDL_PollEvent(&_event))
    {
        if (_event.type == SDL_QUIT)
        {
            return true;
        }

       if (_event.type == SDL_KEYDOWN)
       {
            switch (_event.key.keysym.sym)
            {
                case SDLK_0:
                    _numbers.append("0");
                    break;

                case SDLK_1:
                    _numbers.append(_calcbutton.number[0].value);
                    break;

                case SDLK_2:
                    _numbers.append(_calcbutton.number[1].value);
                    break;

                case SDLK_3:
                    _numbers.append(_calcbutton.number[2].value);
                    break;
                
                case SDLK_4:
                    _numbers.append(_calcbutton.number[3].value);
                    break;
                
                case SDLK_5:
                    _numbers.append(_calcbutton.number[4].value);
                    break;

                case SDLK_6:
                    _numbers.append(_calcbutton.number[5].value);
                    break;

                case SDLK_7:
                    _numbers.append(_calcbutton.number[6].value);
                    break;

                case SDLK_8:
                    _numbers.append(_calcbutton.number[7].value);
                    break;

                case SDLK_9:
                    _numbers.append(_calcbutton.number[8].value);
                    break;

                case SDLK_EQUALS:
                    _numbers.append(_calcbutton.number[12].value);
                    break;
                
                case SDLK_MINUS:
                    _numbers.append(_calcbutton.number[13].value);
                    break;

                case SDLK_ASTERISK:
                    _numbers.append(_calcbutton.number[14].value);
                    break;
                
                case SDLK_SLASH:
                    _numbers.append(_calcbutton.number[15].value);
                    break;

                case SDLK_BACKSPACE:
                    _numbers = S_NULL;
                    break;

                case SDLK_RETURN:
                    _canCalculate = true;
                    _invsyntax = false;
                    break;

                case SDLK_PERIOD:
                    _numbers.append(_calcbutton.number[11].value);
                        break;
            
            default:
                break;
            }
       }
       
       
        
        if (_event.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
            
        }

        if (_event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (_event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    clickMouse = true;
                    break;
                
                default:
                    break;
            }
            
        }
            
        if (_event.type == SDL_MOUSEBUTTONUP)
        {
            switch (_event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    clickMouse = false;
                    break;
                
                default:
                    break;
            }
        }
        

        if (clickMouse)
        {
            for (int i = 0; i < TOTAL_BUTTONS; i++)
            {   
                if ((mouseX > _calcbutton.number[i].buttonRect.x && mouseX < (_calcbutton.number[i].buttonRect.x + _calcbutton.number[i].buttonRect.w)) && (mouseY > _calcbutton.number[i].buttonRect.y && mouseY < (_calcbutton.number[i].buttonRect.y + _calcbutton.number[i].buttonRect.h)))
                {
                    if (_numbers == S_NULL)
                    {
                        _numbers = _calcbutton.number[i].value;
                        clickMouse = false; 
                        break;
                    } 

                    if (_numbers == "+" || _numbers == "*" || _numbers == "/" || _numbers == "-")
                    {
                        _numbers = S_NULL;
                    }
                    
                    
                    if (i == 10) // Equals to button
                    {
                        _canCalculate = true;
                        _invsyntax = false;
                        break;
                    }

                    if (i == 16) // Reset Button
                    {
                        _numbers = S_NULL;
                        break;
                    }
                    
                    _numbers += _calcbutton.number[i].value;
                    clickMouse = false; 
                    break; 
                }
            }
        }

    }

    return false;
}

int main(int argc, char* argv[]) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Initialization Failed " << SDL_GetError() << endl;
        SDL_Quit();
        return 1; 
    }

    if (TTF_Init() < 0) {
        cout << "Initialization Failed " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }  

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        cout << "Initialization Failed " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Window *window = SDL_CreateWindow("Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "images\\Floor.png");
    
    SDL_Rect screen = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

    TTF_Font *myfont = TTF_OpenFont("font.ttf", 24);
    TTF_Font *myfont2 = TTF_OpenFont("font.ttf", 16);
    

    text btntext(myfont, "", {255,255,255});
    text resultText(myfont, "", {100,0,100});
    text historyText(myfont2, "", {128,128,128});
    text resetText(myfont2, "", {200,200,200});
    text syntaxText(myfont2, "", {150,0,0});

    buttons calcbutton(TOTAL_BUTTONS);

    string numbers = S_NULL;
    

    int offsetX = 10;
    int offsetY = 160;
    
    // initialized buttons
    for (int i = 0; i < TOTAL_BUTTONS; i++)
    {
        int row = i / 3;
        int col = i % 3;

        if (i < 9)
        {
            calcbutton.number[i] = button(offsetX + (col * (30 + 30)), offsetY + (row * (30 + 30)), 50, 50);
            calcbutton.number[i].value = to_string(i+1);
        }
        
        if (i == 9)
        {
            calcbutton.number[i] = button(offsetX + (1 * (30 + 30)), offsetY + (row * (30 + 30)), 50, 50);
            calcbutton.number[i].value = to_string(0);
        }
        
        if (i == 10)
        {
            calcbutton.number[i] = button(offsetX + (2 * (30 + 30)), offsetY + (row * (30 + 30)), 50, 50);
            calcbutton.number[i].value = "=";
        }

        if (i == 11)
        {
            calcbutton.number[i] = button(offsetX + (0 * (30 + 30)), offsetY + (row * (30 + 30)), 50, 50);
            calcbutton.number[i].value = ".";
        }

        if (i == 12)
        {
            calcbutton.number[i] = button(offsetX + (3 * (30 + 30)), offsetY + (0 * (30 + 30)), 50, 50);
            calcbutton.number[i].value = "+";
        }

        if (i == 13)
        {
            calcbutton.number[i] = button(offsetX + (3 * (30 + 30)), offsetY + (1 * (30 + 30)), 50, 50);
            calcbutton.number[i].value = "-";
        }

        if (i == 14)
        {
            calcbutton.number[i] = button(offsetX + (3 * (30 + 30)), offsetY + (2 * (30 + 30)), 50, 50);
            calcbutton.number[i].value = "*";
        }

        if (i == 15)
        {
            calcbutton.number[i] = button(offsetX + (3 * (30 + 30)), offsetY + (3 * (30 + 30)), 50, 50);
            calcbutton.number[i].value = "/";
        }

        if (i == 16)
        {
            calcbutton.number[i] = button(((WINDOW_WIDTH - 100)/2), offsetY-30 , 100, 20);
            calcbutton.number[i].value = S_NULL;
        }
    }
    
    SDL_Rect bg = {0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect scr = {0, 0, WINDOW_WIDTH, 140};
    SDL_Event event;
    Uint32 frameStart, frameEnd;
    
    bool canCalculate = false;
    bool pressedOperator = false;
    bool invsyntax = false;

    int frametime;
    int screenOffsetX = 100;
    int screenOffsetY = 100;
    float x;
    float result = 0;

    string history = "History";

    bool quit = false;

    while (!quit)
    {
        frameStart = SDL_GetTicks();
        quit = event_manager(event, calcbutton, numbers, canCalculate, invsyntax);

        

        numbers = regex_replace(numbers, regex("/{2,}"), "/");
        numbers = regex_replace(numbers, regex("\\+{2,}"), "+");
        numbers = regex_replace(numbers, regex("\\*{2,}"), "*");
        numbers = regex_replace(numbers, regex("-{2,}"), "-");
        numbers = regex_replace(numbers, regex("\\+\\-"), "+");
        numbers = regex_replace(numbers, regex("\\-\\+"), "-");
        numbers = regex_replace(numbers, regex("\\*\\/"), "*");
        numbers = regex_replace(numbers, regex("\\/\\*"), "/");
        numbers = regex_replace(numbers, regex("\\*\\+"), "*");
        numbers = regex_replace(numbers, regex("\\+\\*"), "+");
        numbers = regex_replace(numbers, regex("\\-\\*"), "-");
        numbers = regex_replace(numbers, regex("\\/\\+"), "/");
        numbers = regex_replace(numbers, regex("\\+\\/"), "+");
        numbers = regex_replace(numbers, regex("\\/\\-"), "/");
        numbers = regex_replace(numbers, regex("\\-\\/"), "-");
        

        
        
        if (canCalculate)
        {
            
            if (numbers.back() == '/' || numbers.back() == '-' || numbers.back() == '+' || numbers.back() == '*' || numbers.back() == '.'|| numbers == "Empty")
            {
                numbers = "0";
                invsyntax = true;
                
            }
            history = numbers;
            result = evaluateExpression(numbers);
            numbers = convert_3f(&result);
            canCalculate = false;
        }
        
                
        SDL_RenderClear(renderer);
        
        // Render Background here
        SDL_SetRenderDrawColor(renderer, 50, 0, 50, 255);
        SDL_RenderFillRect(renderer, &bg);

        SDL_SetRenderDrawColor(renderer, 35, 0, 35, 255);
        SDL_RenderFillRect(renderer, &scr);

        for (int i = 0; i < TOTAL_BUTTONS-1; i++)
        {
            SDL_SetRenderDrawColor(renderer, 75, 0, 75, 0);
            SDL_RenderFillRect(renderer, &calcbutton.number[i].buttonRect);
            btntext.DrawText(*renderer, (calcbutton.number[i].buttonRect.x + 10), (calcbutton.number[i].buttonRect.y + 6), calcbutton.number[i].value);
        }

        SDL_SetRenderDrawColor(renderer, 75, 0, 75, 0);
        SDL_RenderFillRect(renderer, &calcbutton.number[16].buttonRect);
        resetText.DrawText(*renderer, (94), (calcbutton.number[16].buttonRect.y + 2), "< reset >");

        
        
        if (numbers == S_NULL)
        {
            resultText.DrawText(*renderer, 10 + 10, 20, "Empty"); 
        } else
        {
            resultText.DrawText(*renderer, 10 + 10, 20, numbers); 
        }
        
        
         

        if (convert_3f(&result).length() <= 6)
        {
            screenOffsetX = 180;
        }

        if (convert_3f(&result).length() > 6)
        {
            screenOffsetX = WINDOW_WIDTH - convert_3f(&result).length()*12;
        }
            
        historyText.DrawText(*renderer, screenOffsetX, screenOffsetY-20, history);
        resultText.DrawText(*renderer, screenOffsetX, screenOffsetY, convert_3f(&result));
        
        if (invsyntax)
        {
            syntaxText.DrawText(*renderer, 0, 0, "Invalid Syntax");
        }
        
        

        SDL_RenderPresent(renderer);

        
            
        // Frame Displacement
        frametime = SDL_GetTicks() - frameStart;
        if (FRAMEDELAY > frametime)
        {
            SDL_Delay(FRAMEDELAY - frametime);
        }
        
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}