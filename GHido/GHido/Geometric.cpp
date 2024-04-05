#include "stdafx.h"
#include "Geometric.h"


GeoObj::~GeoObj(void)
{

}


////////////////////////////////////////////

void Gemometric::RenderRectange(const GeoObj& geo_data, SDL_Renderer* screen)
{
    ColorData color = geo_data.color_;

    SDL_Rect fillRect = geo_data.m_rect;
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF);        
    SDL_RenderFillRect(screen, &fillRect );
}

void Gemometric::RenderOutline(const GeoObj& geo_data, SDL_Renderer* screen)
{
    ColorData color = geo_data.color_;
    SDL_Rect outlineRect = geo_data.m_rect; 
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF);
    SDL_RenderDrawRect(screen, &outlineRect );
}