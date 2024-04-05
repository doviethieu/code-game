#pragma once

#include "DefineGame.h"
#include "BaseObj.h"
#include "PlayerObj.h"
#include "MapData.h"
#include "ImpTimer.h"
#include "Geometric.h"
#include "TextObj.h"
#include "PlayerBlood.h"

class GameMain
{
public:
    GameMain(void);
    ~GameMain(void);

    static GameMain* GetInstance()
    {
        if (m_Instance == NULL)
        {
            m_Instance = new GameMain();
        }
        return m_Instance;
    }
    static GameMain* m_Instance;


    bool Init();
    bool InitData();
    void LoopGame();
    void Close();

    GeoObj m_TopFrame;
    GeoObj m_TopFrameBorder;

private:
    SDL_Window* m_Window;
    SDL_Renderer* m_Screen;
    SDL_Event m_event;

    TTF_Font*   m_Font;
    TTF_Font*   m_FontTitle;

    BaseObj m_Bkgn;
    PlayerObj m_Player;

    PlayerBlood m_playerBlood;
};


