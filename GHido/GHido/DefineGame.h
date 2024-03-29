
#ifndef GAME_DEFINE_H_
#define GAME_DEFINE_H_

#include<Windows.h>
#include <string>
#include <vector>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

#define TILE_SIZE 64

#define RENDER_DRAW_COLOR 0X00

#define VT(T) std::vector<T>

enum ColorBase
{
    RED_COLOR = 0,
    WHITE_COLOR = 1,
    BLACK_COLOR = 2,
    GREEN_COLOR = 3,
    BLUE_COLOR = 4,
    ORANGE_COLOR = 5,
    GRAY_COLOR = 6,
};


typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
} Input;

class ColorData
{
public:
    ColorData() { ; }
    ColorData(Uint8 r, Uint8 g, Uint8 b)
    {
        red_ = r;
        green_ = g;
        blue_ = b;
    }

    Uint8 red_;
    Uint8 green_;
    Uint8 blue_;

};
#endif