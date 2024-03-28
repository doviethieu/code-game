
#pragma once

#include "DefineGame.h"

class BaseObj
{
public:
    BaseObj(void);
    ~BaseObj(void);

    void SetRect(const int& x, const int& y) { m_Rect.x = x; m_Rect.y = y;}
    SDL_Rect GetRect() {return m_Rect;}

    void SetIsFlip(const bool& is_flip) {m_Flip = is_flip;}
    bool GetIsFlip() {return m_Flip;}

    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    virtual bool LoadImg(std::string path, SDL_Renderer* des);

    void Free();
protected:
    SDL_Texture* m_ObjectTexture;
    SDL_Rect m_Rect;
    bool m_Flip;
};

