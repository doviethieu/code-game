#pragma once

#include "DefineGame.h"
#include "BaseObj.h"

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

private:
    SDL_Window* m_Window;
    SDL_Renderer* m_Screen;
    SDL_Event m_event;

    BaseObj m_Bkgn;
};


