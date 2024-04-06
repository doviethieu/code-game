#include "stdafx.h"
#include "GameMain.h"
#include "Music.h"
#include <Windows.h>
#include "ExplosionObj.h"

GameMain* GameMain::m_Instance = NULL;

GameMain::GameMain(void)
{
    m_Window = NULL;
    m_Screen = NULL;
    m_Font = NULL;
    m_FontTitle = NULL;

    m_GameState = G_START;
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

     m_Font = TTF_OpenFont(g_MenuFont, 30);
     m_FontTitle = TTF_OpenFont(g_MenuFont, 50);
     if (m_Font == NULL || m_FontTitle == NULL)
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

    m_TopFrame.m_rect.x = 0;
    m_TopFrame.m_rect.y = 0;
    m_TopFrame.m_rect.w = SCREEN_WIDTH;
    m_TopFrame.m_rect.h = TOP_FRAME_HEIGHT;
    m_TopFrame.color_ = ColorData(36, 36, 36);

    m_TopFrameBorder.m_rect.x = 1;
    m_TopFrameBorder.m_rect.y = 1;
    m_TopFrameBorder.m_rect.w =   m_TopFrame.m_rect.w -1;
    m_TopFrameBorder.m_rect.h = TOP_FRAME_HEIGHT - 2;
    m_TopFrameBorder.color_ = ColorData(255, 255, 0);

    m_playerBlood.Init(m_Screen);

    //Init Start Game
    m_StartMenu.LoadBkgn("image\\menu_start.png", m_Screen);
     // Màn hình start game có 2 button player và exist
    VT(BaseObj*) menu_list;
    BaseObj* playObj = new BaseObj();
    BaseObj* ExitObj = new BaseObj();

    int x_pos = H_WIDTH - 200;
    int y_pos = H_HEIGHT + 180;

    bool b1a = playObj->LoadImg("image\\start.png", m_Screen);
    if (b1a)
    {
        playObj->SetRect(x_pos, y_pos);
        menu_list.push_back(playObj);
    }

    bool b3a = ExitObj->LoadImg("image\\exit.png", m_Screen);
    if (b3a)
    {
        y_pos += 50;
        ExitObj->SetRect(x_pos, y_pos);
        menu_list.push_back(ExitObj);
    }

    m_StartMenu.SetImgOptionList(menu_list);
    m_StartMenu.InitOption(m_Screen);


    //Init Pause Game
     m_PauseMenu.SetIsBkgn(false);
     VT(BaseObj*) pause_list;

     // pause game có 2 button reusume và quit
    BaseObj* resumeObj = new BaseObj();
    BaseObj* quitObj = new BaseObj();

    bool ret1 = resumeObj->LoadImg("image\\resume.png", m_Screen);
    if (ret1)
    {
        pause_list.push_back(resumeObj);
    }

    bool ret3 = quitObj->LoadImg("image\\exit.png", m_Screen);
    if (ret3)
    {
        pause_list.push_back(quitObj);
    }

    m_PauseMenu.SetImgOptionList(pause_list);
    m_PauseMenu.InitFrameGeo();

    return true;
}

void GameMain::LoopGame() // ve nen va cap nhat hien thi cho den khi co yeu cau quit game
{
    bool quit_game = false;

    ImpTimer fps;

    TextObj pTime;
    INT time_down = 200;

    int xPos = SCREEN_WIDTH - 150;
    pTime.SetPos(xPos, 15);

    TextObj pTitleGame;
    pTitleGame.SetText("HIDO-GAME-2024");
    pTitleGame.SetColor(TextObj::ORANGE_TEXT);
    pTitleGame.LoadFromRenderedText(m_FontTitle, m_Screen);
    pTitleGame.SetPos(SCREEN_WIDTH*0.5-200, 10);

    ExpList* pExpList = ExpList::GetInstance();

   unsigned int frame_count = 0; 

    while(quit_game == false)
    {
        fps.start(); // bat dau chay, dong ho bat xac dinh thoi diem chay
        while (SDL_PollEvent(&m_event) != 0)
        {
            if (m_event.type == SDL_QUIT)
            {
                quit_game = true;
            }

            if (m_event.type == SDL_KEYDOWN)
            {
                // Nếu bấm space thì game pause
                if (m_event.key.keysym.sym == SDLK_SPACE)
                {
                    m_GameState = G_PAUSE;
                    Music::GetInstance()->PlaySoundGame(Music::GAME_PAUSE);
                }
            }

            if (m_GameState == GameState::G_START)
            {
                m_StartMenu.MenuAction(m_event, m_Screen);
            }

            if (m_GameState == GameState::G_PLAYING)
            {
                m_Player.HandleInputAction(m_event,m_Screen);
            }

            if (m_GameState == GameState::G_PAUSE)
            {
                m_PauseMenu.MenuAction(m_event, m_Screen);
            }
            if (m_GameState == GameState::G_END)
            {
                m_EndMenuOver.MenuAction(m_event, m_Screen);
                m_EndMenuWin.MenuAction(m_event, m_Screen);
            }
        }

        // Chạy bản nhạc xuyên suốt quá trình chơi game
        Music::GetInstance()->PlayMusic();

        if (m_GameState == GameState::G_START)
        {
            m_StartMenu.Render(m_Screen);
            int is = m_StartMenu.GetSelect();
            if (is == 0) // bấm vào play
            {
                fps.start();
                // chuyển game sang trạng thái playing
                m_GameState = G_PLAYING;

                // đưa kết quả phím bấm của start_menu về -1(vì lúc này màn hình start sẽ biến mất)
                m_StartMenu.SetIsSelect(-1);

            }
            else if (is == 1) // bấm vào exit game
            {
                quit_game = true;
                m_StartMenu.SetIsSelect(-1);
                continue;
            }
        }
        else if (m_GameState == GameState::G_PLAYING)
        {
            SDL_Rect clip = SDL_Rect();
            clip.x = 0;
            clip.y = 0;
            clip.w = SCREEN_WIDTH;
            clip.h = SCREEN_HEIGHT;
            m_Bkgn.Render(m_Screen, &clip);

            GameMap::GetInstance()->DrawMap(m_Screen);

            // Draw RectFrame
            Gemometric::RenderRectange(m_TopFrame, m_Screen);
            Gemometric::RenderOutline(m_TopFrameBorder, m_Screen);

            m_Player.Show(m_Screen);
            m_Player.DoAction(m_Screen);
            m_Player.HandleBullet(m_Screen);

            if (m_Player.CheckMinusBlood() == true)
            {
                m_playerBlood.MinusUpdate();
                m_Player.ResetFlagBlood();
            }

            // check player get coin 
            // check ret coin > 100
            // m_playerBlood.Plus...

            // Blood/Alive Player
            m_playerBlood.Show(m_Screen);

            if (m_playerBlood.IsEmpty() == true)
            {
                MessageBox(NULL, L"GAME OVER", L"Game Information", MB_ICONWARNING | MB_OK);
                quit_game = true;
            }

             // Title
             pTitleGame.RenderText(m_Screen);

             // Time
             std::string str_val = std::to_string(time_down);
             std::string str_time = "Time: " + str_val;

              pTime.SetText(str_time);
              pTime.LoadFromRenderedText(m_Font, m_Screen);
              pTime.RenderText(m_Screen);

            pExpList->Render(m_Screen);
  
            frame_count++;
            if (frame_count == FRAMES_PER_SECOND)
            {
                time_down--;
                frame_count = 0;
            }

            if (time_down <= 0)
            {
                 MessageBox(NULL, L"GAME OVER", L"Game Information", MB_ICONWARNING | MB_OK);
                 quit_game = true;
            }
        }
        else if (m_GameState == GameState::G_PAUSE)
        {
             // Background
            SDL_Rect clip = SDL_Rect();
            clip.x = 0;
            clip.y = 0;
            clip.w = SCREEN_WIDTH;
            clip.h = SCREEN_HEIGHT;
            m_Bkgn.Render(m_Screen, &clip);

             // Map
            GameMap::GetInstance()->DrawMap(m_Screen);

            Gemometric::RenderRectange(m_TopFrame, m_Screen);
            Gemometric::RenderOutline(m_TopFrameBorder, m_Screen);

            m_playerBlood.Show(m_Screen);

            pTitleGame.RenderText(m_Screen);

            // Time
             std::string str_val = std::to_string(time_down);
             std::string str_time = "Time: " + str_val;

              pTime.SetText(str_time);
              pTime.LoadFromRenderedText(m_Font, m_Screen);
              pTime.RenderText(m_Screen);

              m_Player.Show(m_Screen);
              m_Player.HandleBullet(m_Screen, true);

               pExpList->Render(m_Screen, true);

               m_PauseMenu.Render(m_Screen);

               int is = m_PauseMenu.GetSelect();
               if (is == 0)
               {
                    // với resume thì thay đổi trạng thái game = playing để hoạt động bình thường
                    m_GameState = G_PLAYING;
                    m_PauseMenu.SetIsSelect(-1);
                }
                else if (is == 1)
                {
                    // kết thúc game
                    m_PauseMenu.SetIsSelect(-1);

                    m_GameState = G_START;
                    m_Player.ReStart();
                
                    GameMap::GetInstance()->GetMap()->SetStartX(0);
                    GameMap::GetInstance()->GetMap()->SetStartY(0);

                    //pThreatsAd->ReStart(m_Screen);
                    time_down = 300;
                    //m_player_coin.SetCount(0);
                    m_playerBlood.ReStart(m_Screen);

                    GameMap::GetInstance()->ResetMap(m_Screen);
                    pExpList->ResetExp();
                    continue;
                }
        }
        else if (m_GameState == GameState::G_END)
        {

        }

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