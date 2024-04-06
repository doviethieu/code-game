#include "stdafx.h"
#include "ExplosionObj.h"


ExplosionObj::ExplosionObj(void)
{
    m_isActive = true;
    frame_ = -1;
    m_Frame = 4;
    frame_width_ = 0;
    frame_height_ = 0;

    for (int i = 0; i < m_Frame; i++)
    {
        SDL_Rect rt = { 0, 0, 0, 0 };
        frame_clip_.push_back(rt);
    }
}


ExplosionObj::ExplosionObj(int nFrame)
{
    m_isActive = true;
    frame_ = -1;
    m_Frame = nFrame;
    frame_width_ = 0;
    frame_height_ = 0;

    for (int i = 0; i < m_Frame; i++)
    {
        SDL_Rect rt = { 0, 0, 0, 0 };
        frame_clip_.push_back(rt);
    }
}



ExplosionObj::~ExplosionObj(void)
{
}


bool ExplosionObj::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    if (ret == true)
    {
        // tính toán kích thước 1 frame đơn
        // ví dụ 240/4 = 60
        frame_width_ = m_Rect.w / m_Frame;
        frame_height_ = m_Rect.h;
    }

    // xử lý xây dựng mảng clip thông tin cho từng frame
    set_clips();
    return ret;
}

void ExplosionObj::set_clips()
{
    if (frame_width_ > 0 && frame_height_ > 0)
    {
        // với 4 frame cho 1 tấm ảnh dài ví dụ 240x60
        for (int i = 0; i < m_Frame; i++)
        {
            // frame 0: 0,0,60,60
            // frame 1: 60,0, 60, 60...
            frame_clip_[i].x = frame_width_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = frame_width_;
            frame_clip_[i].h = frame_height_;
        }
    }
}

// tính toán vị trí xuất hiện của frame
void ExplosionObj::SetXP(SDL_Rect rect_pos)
{
    int x_pos = (int)(rect_pos.x - frame_width_*0.5);
    int y_pos = (int)(rect_pos.y - frame_height_*0.5);

    SetRect(x_pos, y_pos);
}

void ExplosionObj::ImpRender(SDL_Renderer* screen, bool is_pause /* false*/)
{
    // render toàn bộ frame, và frame sẽ chạy lần lượt từ 0 - numFame
    if (is_pause == false)
    {
        frame_++;
        if ((int)frame_ >= m_Frame)
        {
            // khi frame chạy = m_Frame
            // thì vụ nổ kết thúc, nó sẽ ko hiển thị nữa. và biến active sẽ quản lý vấn đề đó
            frame_ = 0;
            m_isActive = false;
        }
    }

    if (m_isActive)
    {
            SDL_Rect* currentClip = &frame_clip_[frame_];
            BaseObj::Render(screen, currentClip);
    }
}

//////////////////////////////////////////////////////////////

ExpList* ExpList::instance_ = NULL;
ExpList::ExpList()
{

}

ExpList::~ExpList()
{

}

// Hiển thị danh sách toàn bộ các vụ nổ
void ExpList::Render(SDL_Renderer* screen, bool is_pause /*false*/)
{
    for (size_t i = 0; i < m_ExpList.size(); i++)
    {
        ExplosionObj* pObj = m_ExpList.at(i);
        if (pObj != NULL)
        {
            pObj->ImpRender(screen, is_pause);
        }
    }

    for (size_t i = 0; i < m_ExpList.size(); i++)
    {
        ExplosionObj* pObj = m_ExpList.at(i);
        if (pObj != NULL)
        {
            if (pObj->GetActive() == false)
            {
                // khi một vụ nổ kết thúc, thông tin về nó sẽ được loại khỏi danh sách
                pObj->Free();
                m_ExpList.erase(m_ExpList.begin() + i);
                i--;
            }
        }
    }
}

// lưu thêm vụ nổ vào danh sách
void ExpList::Add(ExplosionObj* pObj)
{
    Music::GetInstance()->PlaySoundGame(Music::EXP_SOUND);
    if (pObj != NULL)
    {
        m_ExpList.push_back(pObj);
    }
}

bool ExpList::IsEmpty()
{
    return m_ExpList.empty();
}