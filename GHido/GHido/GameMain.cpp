#include "stdafx.h"
#include "GameMain.h"
#include "Music.h"


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

    // Make windows for Game
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

        // Khởi tạo chế độ load âm thanh
        if (Music::GetInstance()->Init() == false)
        {
            return false;
        }

    return bRet;
}

bool GameMain::InitData()// nap anh nen
{
    std::string path = "image\\bkgn.png";
    bool ret = m_Bkgn.LoadImg(path, m_Screen);

    GameMap* game_map = GameMap::GetInstance();
    game_map->SetPath("image\\map");
    game_map->LoadMap();
    game_map->LoadMapTiles(m_Screen);

    bool ret2 = m_Player.LoadImg("image\\hido_move.png", m_Screen);
    m_Player.Set_Pos(100, 50);

    return true;
}

void GameMain::LoopGame() // ve nen va cap nhat hien thi cho den khi co yeu cau quit game
{
    bool quit_game = false;

    ImpTimer fps;

    while(quit_game == false)
    {
        fps.start(); // bat dau chay, dong ho bat xac dinh thoi diem chay
        while (SDL_PollEvent(&m_event) != 0)
        {
            if (m_event.type == SDL_QUIT)
            {
                quit_game = true;
            }

            m_Player.HandleInputAction(m_event,m_Screen);
        }

        // Chạy bản nhạc xuyên suốt quá trình chơi game
        Music::GetInstance()->PlayMusic();

        SDL_Rect clip = SDL_Rect();
        clip.x = 0;
        clip.y = 0;
        clip.w = SCREEN_WIDTH;
        clip.h = SCREEN_HEIGHT;
        m_Bkgn.Render(m_Screen, &clip);

        GameMap::GetInstance()->DrawMap(m_Screen);

        m_Player.DoAction(m_Screen);
        m_Player.Show(m_Screen);

        SDL_RenderPresent(m_Screen);

        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
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