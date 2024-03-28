#include "stdafx.h"
#include "GameMain.h"


GameMain* GameMain::m_Instance = NULL;

GameMain::GameMain(void)
{
    m_Window = NULL;
    m_Screen = NULL;
}


GameMain::~GameMain(void)
{

}

bool GameMain::Init()
{
    bool bRet = true;

    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret < 0)
    {
        return false;
    }

    m_Window = SDL_CreateWindow("Hido Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); 
    if (m_Window == NULL)
    {
        //std::cout << "Cannot create windows.";
        return false;
    }

     m_Screen = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
     if (m_Screen == NULL)
     {
         return false;
     }

     SDL_SetRenderDrawColor(m_Screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

     if (TTF_Init() == -1)
     {
         return false;
     }

    return bRet;
}

bool GameMain::InitData()
{
    std::string path = "image\\bkgn.png";
    bool ret = m_Bkgn.LoadImg(path, m_Screen);

    return true;
}

void GameMain::LoopGame()
{
    bool quit_game = false;

    while(quit_game == false)
    {
        while (SDL_PollEvent(&m_event) != 0)
        {
            if (m_event.type == SDL_QUIT)
            {
                quit_game = true;
            }
        }

        SDL_Rect clip = SDL_Rect();
        clip.x = 0;
        clip.y = 0;
        clip.w = SCREEN_WIDTH;
        clip.h = SCREEN_HEIGHT;
        m_Bkgn.Render(m_Screen, &clip);

        SDL_RenderPresent(m_Screen);
    }
}

void GameMain::Close()
{
    m_Bkgn.Free();

    SDL_DestroyRenderer(m_Screen);
    m_Screen = NULL;


    SDL_DestroyWindow(m_Window);
    m_Window = NULL;

    IMG_Quit();

    SDL_Quit();
}