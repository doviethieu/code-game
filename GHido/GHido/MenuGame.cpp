#include "stdafx.h"
#include "MenuGame.h"


MenuGame::MenuGame(bool is_bkgn /*= true*/)
{
    m_Type = 0;
	m_isDrawBkgn = is_bkgn;
    m_Select = -1;
}


MenuGame::~MenuGame(void)
{
}


void MenuGame::LoadBkgn(const char* file, SDL_Renderer* screen)
{
    if (m_isDrawBkgn == true)
    {
        m_Background.LoadImg(file, screen);
    }
}

void MenuGame::Render(SDL_Renderer* screen)
{
    if (m_isDrawBkgn == true)
    {
        SDL_Rect clip = SDL_Rect();
        clip.x = 0;
        clip.y = 0;
        clip.w = SCREEN_WIDTH;
        clip.h = SCREEN_HEIGHT;

        m_Background.Render(screen, &clip);
    }
}

void MenuGame::MenuAction(SDL_Event events, SDL_Renderer* screen)
{
    
}

void MenuGame::Free()
{
    m_Background.Free();
}

bool MenuGame::OnRect(int x, int y, SDL_Rect& rect)
{
    if (rect.w > 0 && rect.h > 0)
    {
        if (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h)
        {
            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////
MenuGameStart::MenuGameStart()
{
    m_Select = -1;
}

MenuGameStart::~MenuGameStart()
{
    FreeData();
}


void MenuGameStart::InitOption(SDL_Renderer* screen)
{

}

void MenuGameStart::SetImgOptionList(const VT(BaseObj*)& img_list)
{
    FreeData();
    m_ImgOptionList = img_list;
}

void MenuGameStart::FreeData()
{
    if (m_ImgOptionList.empty())
    {
        for (auto x : m_ImgOptionList)
        {
            if (x != NULL)
            {
                x->Free();
            }
        }
        m_ImgOptionList.clear();
    }
}

void MenuGameStart::Render(SDL_Renderer* screen)
{
    MenuGame::Render(screen);
    for (auto x : m_ImgOptionList)
    {
        x->Render(screen);
    }
}

void MenuGameStart::MenuAction(SDL_Event events, SDL_Renderer* screen)
{

    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            break;
        }
        case SDLK_LEFT:
        {
            break;
        }
        }
    }


    switch (events.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                // khi bấm chuột vào vùng button, thì trả về index của button
                m_Select = i;
                break;
            }
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                 // không bấm chuột mà move chuột vào vùng button
                // thay đổi hình ảnh chữ trắng, sang chữ đổ tạo hiệu ứng trắng đỏ
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\start_red.png", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit_red.png", screen);
                }
            }
            else
            {
                // khi ra ngoài vùng button, thì lại load ảnh trắng
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\start.png", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit.png", screen);
                }
            }
        }
        break;
    }
    default: break;
    }
}

///////////////////////////////////////////////////////////////////////
MenuGamePause::MenuGamePause()
{
    m_Select = -1;
}

MenuGamePause::~MenuGamePause()
{
    FreeData();
}


void MenuGamePause::SetImgOptionList(const VT(BaseObj*)& img_list)
{
    FreeData();
    m_ImgOptionList = img_list;
}

void MenuGamePause::FreeData()
{
    if (m_ImgOptionList.empty())
    {
        for (auto x : m_ImgOptionList)
        {
            if (x != NULL)
            {
                x->Free();
            }
        }
        m_ImgOptionList.clear();
    }
}

void MenuGamePause::InitFrameGeo()
{
    int x1 = H_WIDTH - 120;
    int y1 = H_HEIGHT - 60;
    m_Rect.m_rect.x = x1;
    m_Rect.m_rect.y = y1;
    m_Rect.m_rect.w = 200;
    m_Rect.m_rect.h = 100;

    m_Rect.color_ = TBase::GetBaseColor(GRAY_COLOR);

    int x2 = m_Rect.m_rect.x + 1;
    int y2 = m_Rect.m_rect.y + 1;
    int w2 = m_Rect.m_rect.w - 1;
    int h2 = m_Rect.m_rect.h - 1;

    m_RectOutLine.m_rect.x = x2;
    m_RectOutLine.m_rect.y = y2;
    m_RectOutLine.m_rect.w = w2;
    m_RectOutLine.m_rect.h = h2;

    m_RectOutLine.color_ = TBase::GetBaseColor(WHITE_COLOR);

    int xPos = m_Rect.m_rect.x + 70;
    int yPos = m_Rect.m_rect.y + 10;

    for (size_t i = 0; i < m_ImgOptionList.size(); i++)
    {
        m_ImgOptionList[i]->SetRect(xPos, yPos);
        yPos += 50;
    }
}

void MenuGamePause::Render(SDL_Renderer* screen)
{
    Gemometric::RenderRectange(m_Rect, screen);
    Gemometric::RenderOutline(m_RectOutLine, screen);

    MenuGame::Render(screen);
    for (auto x : m_ImgOptionList)
    {
        x->Render(screen);
    }
}

void MenuGamePause::MenuAction(SDL_Event events, SDL_Renderer* screen)
{
    switch (events.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                m_Select = i;
                break;
            }
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                if (i == 0)
                {
                     m_ImgOptionList[i]->LoadImg("image\\resume_red.png", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit_red.png", screen);
                }
            }
            else
            {
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\resume.png", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit.png", screen);
                }
            }
        }
        break;
    }
    default: break;
    }
}

///////////////////////////////////////////////////////////////

MenuGameEnd::MenuGameEnd()
{
    m_Select = -1;
    m_Font = NULL;
    m_State = GA_NONE;
}

MenuGameEnd::~MenuGameEnd()
{

}

void MenuGameEnd::SetImgOptionList(const VT(BaseObj*)& img_list)
{
    FreeData();
    m_ImgOptionList = img_list;
}


void MenuGameEnd::FreeData()
{
    if (m_ImgOptionList.empty())
    {
        for (auto x : m_ImgOptionList)
        {
            if (x != NULL)
            {
                x->Free();
            }
        }
        m_ImgOptionList.clear();
    }

    for (auto t : m_TextList)
    {
        t->Free();
    }
    m_TextList.clear();
}

void MenuGameEnd::RemoveTextList()
{
    for (int i = 0; i < m_TextList.size(); i++)
    {
        TextObj* pText = m_TextList[i];
        if (pText != NULL)
        {
            delete pText;
            pText = NULL;
        }
    }

    m_TextList.clear();
}

void MenuGameEnd::Render(SDL_Renderer* screen)
{
    MenuGame::Render(screen);
    for (auto x : m_TextList)
    {
        x->LoadFromRenderedText(m_Font, screen);
        x->RenderText(screen);
    }
      
    for (int i = 0; i < m_ImgOptionList.size(); i++)
    {
        BaseObj* pObj = m_ImgOptionList.at(i);
        if (pObj != NULL)
        {
            if (i == 1)
            {
                if (m_State == GA_WIN)
                {
                    pObj->Render(screen);
                }
                else if (m_State == GA_LOSE)
                {
                    ;// pObj->SetTextureIndex(2);
                    pObj->Render(screen);
                }
                else
                {
                    ;// pObj->SetTextureIndex(2);
                    pObj->Render(screen);
                }
            }
            else
            {
                pObj->Render(screen);
            }
        }
    }
}

void MenuGameEnd::MenuAction(SDL_Event events, SDL_Renderer* screen)
{
    switch (events.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                m_Select = i;
            }
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit_red.bmp", screen);
                }
            }
            else
            {
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit.bmp", screen);
                }
            }
        }
    }
    default: break;
    }
}

void MenuGameEnd::SetTextContent(std::string sData, int idx)
{
    if (idx >= 0 && idx < (int)m_TextList.size())
    {
        TextObj* pText = m_TextList[idx];
        if (pText)
        {
            pText->SetText(sData);
        }
    }
}