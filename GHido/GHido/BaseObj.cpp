#include "stdafx.h"
#include "BaseObj.h"


BaseObj::BaseObj(void) // khoi tao vi tri player ban dau
{
    m_ObjectTexture = NULL;
    m_Rect.x = 0;
    m_Rect.y = 0;
    m_Rect.w = 0;
    m_Rect.h = 0;
    m_Flip = false;
}


BaseObj::~BaseObj(void)
{
    Free();
}


void BaseObj::Render(SDL_Renderer* des, const SDL_Rect* clip) // ve lai texture, cho phep lat texture
{
    if (m_ObjectTexture == NULL)
        return;

    SDL_Rect rQuad = m_Rect;
    if (clip != NULL)
    {
        rQuad.w = clip->w;
        rQuad.h = clip->h;
    }

    INT iFlip = SDL_FLIP_NONE;
    if (m_Flip == true)
    {
        iFlip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(des, m_ObjectTexture, clip, &rQuad, 0, NULL, (SDL_RendererFlip)iFlip);
}

bool BaseObj::LoadImg(std::string path, SDL_Renderer* des)// nap anh tu file va su dung anh do de hien thi doi tuong
{
    Free();

    SDL_Texture* nTexture = NULL;
    SDL_Surface* img_surface = IMG_Load(path.c_str());
    if (img_surface != NULL)
    {
        Uint32 uKey = SDL_MapRGB(img_surface->format, 125, 125, 125);
        SDL_SetColorKey(img_surface, SDL_TRUE, uKey);

        nTexture = SDL_CreateTextureFromSurface(des, img_surface);
        if (nTexture != NULL)
        {
            m_Rect.w = img_surface->w;
            m_Rect.h = img_surface->h;

            m_ObjectTexture = nTexture;
        }
    }

    if (m_ObjectTexture != NULL)
    {
        return true;
    }

    return false;
}

void BaseObj::Free()
{
    if (m_ObjectTexture != NULL)
    {
        SDL_DestroyTexture(m_ObjectTexture);
        m_ObjectTexture = NULL;
    }
}