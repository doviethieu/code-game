#pragma once

#include "DefineGame.h"

#define K_EPXILON_2  0.01
#define K_EPXILON_3  0.001
#define K_EPXILON_4  0.0001
#define K_EPXILON_5  0.00001
#define K_EPXILON_6  0.000001

class GeoObj
{
public:
    ~GeoObj(void);

    GeoObj() 
    { 
        m_rect.x = 0;
        m_rect.y = 0;
        m_rect.w = 0;
        m_rect.h = 0;
        color_ = ColorData(255, 255, 255);
    }

    GeoObj(SDL_Rect rect, const ColorData& color)
    {
        m_rect = rect;
        color_ = color;
    }

    SDL_Rect m_rect;
    ColorData color_;
};

class Gemometric
{
public:
   static void RenderRectange(const GeoObj& geo_data, SDL_Renderer* screen);
   static void RenderOutline(const GeoObj& geo_data,  SDL_Renderer* screen);
};
