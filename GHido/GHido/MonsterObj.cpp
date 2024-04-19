#include "stdafx.h"
#include "MonsterObj.h"


MonsterObj::MonsterObj(int clip_num)
{
    is_alive_ = true;

    on_ground_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    frame_ = 0;
    v_dir_ = -1;
    x_plus_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    m_ClipNum = clip_num;
    m_PassTime = 0;
    type_ = M_NONE;
    pMap_ = GameMap::GetInstance()->GetMap();

    for (int i = 0; i < m_ClipNum; i++)
    {
        m_DelayTimeList.push_back(100);
    }
}



MonsterObj::~MonsterObj(void)
{

}

bool MonsterObj::LoadImg(std::string path, SDL_Renderer* screen)
{
     bool ret = BaseObj::LoadImg(path, screen);
     if (ret == true)
     {
            width_frame_ = m_Rect.w / m_ClipNum;
            height_frame_ = m_Rect.h;
     }

     set_clips();

    return ret;
}

void MonsterObj::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < m_ClipNum; i++)
        {
            SDL_Rect rClip;
            rClip.x = width_frame_*i;
            rClip.y = 0;
            rClip.w = width_frame_;
            rClip.h = height_frame_;
            m_FrameClipList.push_back(rClip);
        }
    }
}

SDL_Rect MonsterObj::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = m_Rect.x;
    rect.y = m_Rect.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void MonsterObj::DoAction()
{
     if (is_alive_ == true)
    {
        y_val_ += GRAVITY_SPEED;
        if (y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }

        CheckToMap();
    }
}


void MonsterObj::CheckToMap()
{
    if (type_ == M_MOVING_SHOT)
    {
        if (on_ground_ == true)
        {
            if (x_val_ > 0)
            {
                DoRight();
            }
            else if (x_val_ < 0)
            {
                DoLeft();
            }

            x_pos_ += x_val_;
        }
    }

    if (y_val_ > 0)
    {
        DoDown();
    }

    y_pos_ += y_val_;
    if (y_pos_ > pMap_->getMaxY())
    {
        is_alive_ = false;
    }
}

void MonsterObj::DoLeft()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;
    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int yPos = (y_pos_ + height_frame_ - EPXILON);
    int xPosPrev = (x_pos_ + x_val_);

    int curTileY = yPos / TILE_SIZE;
    int prevTileX = xPosPrev / TILE_SIZE;

    int pX = xPosPrev - prevTileX*TILE_SIZE;
    int pY = yPos - curTileY*TILE_SIZE;

    bool IsInside = TBase::CheckInsideMap(prevTileX, curTileY);
    if (IsInside)
    {
        BlockMap* pBlock = tile_list[curTileY][prevTileX];
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                std::string sLType = pBlock->getType();
                bool bSkip = pMap->CheckSkipMap(sLType);
                if (bSkip == false)
                {
                    x_pos_ = prevTileX*TILE_SIZE + width_frame_;
                    x_val_ = 0;
                }
            }
        }
    }
    else
    {
        x_val_ = 0;
    }
}

void MonsterObj::DoRight()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;
    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int nextTX = (x_pos_ + x_val_ + width_frame_) / TILE_SIZE;
    int curTY = (y_pos_ + height_frame_ - EPXILON) / TILE_SIZE;

    bool bInside = TBase::CheckInsideMap(nextTX, curTY);
    if (bInside)
    {
        BlockMap* pBlock = tile_list[curTY][nextTX];
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                std::string sLType = pBlock->getType();
                bool bSkip = pMap->CheckSkipMap(sLType);
                if (bSkip == false)
                {
                    x_pos_ = nextTX*TILE_SIZE - width_frame_;
                    x_val_ = 0;
                }
            }
        }
    }
    else
    {
        x_val_ = 0;
    }
}

void MonsterObj::DoDown()
{
    on_ground_ = 0;
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;
    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int curTileX1 = x_pos_ / TILE_SIZE;
    int curTileX2 = (x_pos_ + width_frame_) / TILE_SIZE;

    int yPosNext = y_pos_ + height_frame_ + y_val_;
    int nextTileY = yPosNext / TILE_SIZE;

    bool bInside1 = TBase::CheckInsideMap(curTileX1, nextTileY);
    bool bInside2 = TBase::CheckInsideMap(curTileX2, nextTileY);
    if (bInside1 && bInside2)
    {
        BlockMap* pBlock1 = tile_list[nextTileY][curTileX1];
        if (pBlock1 != NULL && pBlock1->GetTile() != NULL)
        {
            std::string tp1 = pBlock1->getType();
            bool bSkip1 = pMap->CheckSkipMap(tp1);
            bool is_coin = pMap->GetValueProduct(tp1);
            if (bSkip1 == false && is_coin == 0)
            {
                on_ground_ = true;
                y_pos_ = nextTileY*TILE_SIZE - height_frame_;
                y_val_ = 0;
            }
        }

        if (nextTileY >= 0 && nextTileY < tile_list.size())
        {
            if (curTileX2 >= 0 && curTileX2 < tile_list[nextTileY].size())
            {
                BlockMap* pBlock2 = tile_list[nextTileY][curTileX2];
                if (pBlock2 != NULL && pBlock2->GetTile() != NULL)
                {
                    std::string tp2 = pBlock2->getType();
                    bool bSkip2 = pMap->CheckSkipMap(tp2);
                    bool is_coin = pMap->GetValueProduct(tp2);
                    if (bSkip2 == false && is_coin == 0)
                    {
                        on_ground_ = true;
                        y_pos_ = nextTileY*TILE_SIZE - height_frame_;
                        y_val_ = 0;
                    }
                }
            }
        }
    }
}


void MonsterObj::Show(SDL_Renderer* des, bool is_pause /*false*/)
{
    if (is_alive_ == true)
    {
        if (is_pause == false)
        {
            Update();
        }

        m_Rect.x = x_pos_ - pMap_->getStartX();
        m_Rect.y = y_pos_ - pMap_->getStartY();

        if (is_pause == false)
        {
            if (SDL_GetTicks() - m_DelayTimeList[frame_] > m_PassTime)
            {
                    m_PassTime = SDL_GetTicks();
                    frame_++;
                    if (frame_ > m_ClipNum - 1)
                    {
                        frame_ = 0;
                    }
            }
        }

        SDL_Rect* currentClip = &m_FrameClipList[frame_];
        BaseObj::Render(des, currentClip);
    }
}

void MonsterObj::Update()
{
    if (type_ == M_MOVING_SHOT)
    {
        if (on_ground_ == 1)
        {
            // di chuyển theo 2 chiều khác nhau
            if (v_dir_ == 1)
            {
                // ví dụ về bên trái
                x_val_ = -2;
            }
            else if (v_dir_ == -1)
            {
                // về bên phải
                x_val_ = 2;
            }

            // mỗi lần di chuyển, sẽ tính toán tổng lượng thay đổi
            x_plus_ += 3;

            // nếu tổng lượng thay đổi đó lớn hơn 192 thì sẽ đổi chiều
            // nghĩa là monster đi về bên trái một lượng giá trị = 192
            // thì nó sẽ quay ngược lại đi về bên phải
            if (x_plus_ >= 192)
            {
                v_dir_ *= -1;
                if (v_dir_ == -1)
                {
                    // và khi thay đổi chiều di chuyển
                    // thì thay đổi thông số flip của ảnh
                    // để anh được đảo chiều ngược lại khi hiển thị
                    this->SetIsFlip(false);
                }
                else
                {
                    this->SetIsFlip(true);
                }

                x_plus_ = 0;
            }

            // Dùng cho các monster di chuyển 1 chiều liên tục
            // khi vượt quá bản đồ, thì coi như đã chết
            // ví dụ game nấm mario, con goombasss là 1 kiểu như vậy
            if (x_pos_ + width_frame_ < 0 ||
                x_pos_ > pMap_->getMaxX())
            {
                is_alive_ = false;
            }
        }
    }
    else if (type_ == M_BIRD)
    {
        x_pos_ -= 5;
        int start_x = GameMap::GetInstance()->GetMap()->getStartX();
        if (x_pos_ + width_frame_ < (start_x-TILE_SIZE))
        {
             is_alive_ = false;
        }
    }
}

void MonsterObj::ResetBird()
{
    if (type_ == M_BIRD)
    {
        int start_x = GameMap::GetInstance()->GetMap()->getStartX();
        x_pos_ = start_x + 22*TILE_SIZE;
        y_pos_ = 2*TILE_SIZE;
        is_alive_ = true;
    }
}


void MonsterObj::AddBullet(SDL_Renderer* screen)
{
    BulletObj* pBul = NULL;
    pBul = new BulletObj();

    // hướng từ trai ve phai
    if (v_dir_ == -1)
    {
        pBul->set_dir_bullet(BulletObj::BD_RIGHT);
    }
    else
    {
        pBul->set_dir_bullet(BulletObj::BD_LEFT);
    }

     // vị trí phát ra đạn
     int xBul = 0, yBul = 0;
     InitStartPos(xBul, yBul);
     pBul->set_xy_pos(xBul, yBul);
     pBul->Init(kImgBulletCir, screen);

    // tốc độ 8
    pBul->set_x_val(8);

    m_BulletList.push_back(pBul);
}

void MonsterObj::DoBullet(SDL_Renderer* screen, bool is_pause /*false*/)
{
    
    if (is_alive_ == false)
        return;

    for (size_t iBul = 0; iBul < m_BulletList.size(); iBul++)
    {
        BulletObj* pBul = m_BulletList.at(iBul);
        if (pBul != NULL)
        {
            if (pBul->get_is_move() == true)
            {
                int xOffset = 0;
                if (pBul->get_dir_bullet() == BulletObj::BD_LEFT)
                {
                    xOffset = -300;
                }
                else
                {
                     xOffset = 300;
                }

                int xLimit = x_pos_ + xOffset;
                pBul->Show(screen);
                if (is_pause == false)
                {
                    pBul->HandleMove(xLimit, xLimit);
                }
            }
            else
            {
                // sau đó nó được xét lại vị trí ban đầu
                int xBul = 0, yBul = 0;
                InitStartPos(xBul, yBul);
                pBul->set_xy_pos(xBul, yBul);

                // và bắt đầu tái xuát hiện trở lại
                // cách làm này mục đích tái sử dụng tài nguyên
                // không muốn thêm xử lý xóa, rồi lại new
                pBul->set_is_move(true);
                if (v_dir_ == -1)
                {
                    pBul->set_dir_bullet(BulletObj::BD_RIGHT);
                }
                else
                {
                     pBul->set_dir_bullet(BulletObj::BD_LEFT);
                }
            }
        }
    }
}

void MonsterObj::InitStartPos(int& xBul, int& yBul)
{
    if (v_dir_ == -1) // to right
    {
        xBul = x_pos_ + width_frame_;
        yBul = y_pos_ + height_frame_*0.5;
    }
    else
    {
        xBul = x_pos_;
        yBul = y_pos_ + height_frame_*0.5;
    }
}

/////////////////////////////////////////////////
MList* MList::instance_ = NULL;

MList::MList()
{

}

MList::~MList()
{

}

void MList::DestroyInstance()
{
    Free();
    if (instance_ != NULL)
    {
        delete instance_;
        instance_ = 0;
    }
}

// giải phòng toàn bộ layer khi stop game
void MList::Free()
{
    for (int i = 0; i < m_MonsterList.size(); i++)
    {
        MonsterObj* pObj = m_MonsterList[i];
        pObj->Free();
    }

    m_MonsterList.clear();
}

void MList::BuildMonster(SDL_Renderer* screen)
{
    MakeMShot(screen); // loại monster di chuyển
    MakeMStand(screen); // loại monster bắn đạn
    MakeMBird(screen);
}

void MList::MakeMShot(SDL_Renderer* screen)
{
    UINT gbNum = sizeof(gMonsterShot) / sizeof(gMonsterShot[0]);
    if (gbNum > 0)
    {
        for (int i = 0; i < gbNum; i++)
        {
            MonsterObj* pMonster = new MonsterObj(8);
            pMonster->set_type(MonsterObj::M_MOVING_SHOT);
            int x_tile = gMonsterShot[i];
            int y_tile = 4;
            pMonster->set_xpos(x_tile*TILE_SIZE);
            pMonster->set_ypos(y_tile*TILE_SIZE);
            bool ret = pMonster->LoadImg(sMShot, screen);
            if (ret == true)
            {
                pMonster->AddBullet(screen);
                m_MonsterList.push_back(pMonster);
            }
        }
    }
}

void MList::MakeMStand(SDL_Renderer* screen)
{
    UINT gbNum = sizeof(gMonsterStand) / sizeof(gMonsterStand[0]);
    if (gbNum > 0)
    {
        for (int i = 0; i < gbNum; i++)
        {
            MonsterObj* pMonster = new MonsterObj(4);
            pMonster->set_type(MonsterObj::M_STAND_ATTACK);
            int x_tile = gMonsterStand[i];
            int y_tile = 2;
            pMonster->set_xpos(x_tile*TILE_SIZE);
            pMonster->set_ypos(y_tile*TILE_SIZE);
            bool ret = pMonster->LoadImg(sMStand, screen);
            if (ret == true)
            {
                m_MonsterList.push_back(pMonster);
            }
        }
    }
}

void MList::MakeMBird(SDL_Renderer* screen)
{
    MonsterObj* pMonster = new MonsterObj(4);
    pMonster->set_type(MonsterObj::M_BIRD);
    int start_x = GameMap::GetInstance()->GetMap()->getStartX();
    int x_pos = start_x + 22*TILE_SIZE;
    int y_pos = 2*TILE_SIZE;
    pMonster->set_xpos(x_pos);
    pMonster->set_ypos(y_pos);
    bool ret = pMonster->LoadImg(sMBird, screen);
    if (ret == true)
    {
        m_MonsterList.push_back(pMonster);
    }
}

void MList::Render(SDL_Renderer* screen, bool is_pause /*false*/)
{
    for (int i = 0; i < m_MonsterList.size(); i++)
    {
        MonsterObj* pMonster = m_MonsterList.at(i);

        if (pMonster->get_is_alive() == true)
        {
            if (pMonster->GetType() == MonsterObj::M_MOVING_SHOT)
            {
                if (is_pause == false)
                {
                    // di chuyển trên bản đồ cho monster
                    pMonster->DoAction();
                }
                pMonster->Show(screen, is_pause);
                pMonster->DoBullet(screen, is_pause);
            }
            else if (pMonster->GetType() == MonsterObj::M_STAND_ATTACK)
            {
                pMonster->Show(screen);
                if (is_pause == false)
                {
                    // di chuyển trên bản đồ cho monster
                    pMonster->DoAction();
                }
            }
            else if (pMonster->GetType() == MonsterObj::M_BIRD)
            {
                pMonster->Show(screen, is_pause);
            }
        }
        else
        {
            if (pMonster->GetType() == MonsterObj::M_BIRD)
            {
                pMonster->ResetBird();
            }
            else
            {
                // xóa monster khi bị giết
                pMonster->Free();
                m_MonsterList.erase(m_MonsterList.begin() + i);
            }
        }
    }
}

bool MList::CheckCol(const SDL_Rect& rect, SDL_Rect& exp_rect,  bool bDel /*= true*/)
{
    bool bRet = false;
    for (int i = 0; i < m_MonsterList.size(); i++)
    {
        MonsterObj* pMonster = m_MonsterList.at(i);
        if (pMonster != NULL)
        {
            SDL_Rect rect_monst = pMonster->GetRectFrame();
            bool bColRect = TBase::CheckCollision(rect, rect_monst);
            if (bColRect)
            {
                bRet = true;

                if (bDel == true)
                {
                    pMonster->Free();
                    m_MonsterList.erase(m_MonsterList.begin() + i);
                }
                exp_rect = rect_monst;
                break;
            }
        }
    }

    return bRet;
}

bool MList::CheckColBul(const SDL_Rect& rect_object)
{
    bool bFindCol = false;
    for (int i = 0; i < m_MonsterList.size(); i++)
    {
        MonsterObj* pMonster = m_MonsterList.at(i);
        if (pMonster != NULL && pMonster->GetType() == MonsterObj::M_MOVING_SHOT)
        {
                std::vector<BulletObj*> bullet_list = pMonster->GetBuls();
                for (int bul = 0; bul < bullet_list.size(); bul++)
                {
                    BulletObj* pBull = bullet_list[bul];
                    if (pBull != NULL)
                    {
                        bool bCol = TBase::CheckCollision(rect_object, pBull->GetRect());
                        if (bCol == true)
                        {
                            bFindCol = true;
                            pBull->set_is_move(false);
                            break;
                        }
                    }
                }
        }

        if (bFindCol == true)
        {
            break;
        }
    }

    return bFindCol;
}

void MList::ReStart(SDL_Renderer* screen)
{
    Free();
    BuildMonster(screen);
}
