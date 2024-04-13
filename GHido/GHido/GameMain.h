#pragma once

#include "DefineGame.h"
#include "BaseObj.h"
#include "PlayerObj.h"
#include "MapData.h"
#include "ImpTimer.h"
#include "Geometric.h"
#include "TextObj.h"
#include "PlayerBlood.h"
#include "MenuGame.h"

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

    enum GameState
    {
        G_PLAYING = 300,
        G_START,
        G_PAUSE,
        G_OVER,
        G_END,
    };

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

    int m_GameState;

    TTF_Font*   m_Font;
    TTF_Font*   m_FontTitle;

    BaseObj m_Bkgn;
    PlayerObj m_Player;

    PlayerBlood m_playerBlood;
    BaseObj  m_Coin;

    MenuGameStart m_StartMenu; // thực hiện tạo màn hình start game
    MenuGamePause m_PauseMenu; // thực hiện tạo màn hình pause game, dùng phím space
    MenuGameEnd m_EndMenuOver; // màn hình endgame khi thua cuộc
    MenuGameEnd m_EndMenuWin;  // màn hình endgame khi chiến thắng
};


