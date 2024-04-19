#include "stdafx.h"
#include "BossMonsterObj.h"


BossMonsterObj::BossMonsterObj(void)
{
    is_alive_ = true;
    x_pos_ = 0;
    y_pos_ = 0;
    y_val_ = 0;
    on_ground_ = false;
     m_PassTime = 0;
     frame_ = 0;
    for (int i = 0; i < BOSS_FRAME_NUM; i++)
    {
        m_DelayTimeList.push_back(100);
    }

    m_HitCount = 0;
}


BossMonsterObj::~BossMonsterObj(void)
{
}


bool BossMonsterObj::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = m_Rect.w/BOSS_FRAME_NUM;
        height_frame_ = m_Rect.h;
        SetClip();
    }

    return ret;
}

void BossMonsterObj::UpdateImg(std::string path, SDL_Renderer* screen)
{
    BaseObj::LoadImg(path, screen);
}


void BossMonsterObj::SetClip()
{
  if (width_frame_ > 0 && height_frame_ > 0)
  {
      for (int i = 0; i < BOSS_FRAME_NUM; i++)
      {
          m_FrameClip[i].x = width_frame_*i;
          m_FrameClip[i].y = 0;
          m_FrameClip[i].w = width_frame_;
          m_FrameClip[i].h = height_frame_;
      }
  }
}

SDL_Rect BossMonsterObj::GetRectFrame()
{
  SDL_Rect rect;
  rect.x = m_Rect.x;
  rect.y = m_Rect.y;
  rect.w = width_frame_;
  rect.h = height_frame_;
  return rect;
}


void BossMonsterObj::AddBullet(SDL_Renderer* screen)
{
    if (is_alive_ == false)
        return;

    BulletObj* pBul = NULL;
    pBul = new BulletObj();
    pBul->set_dir_bullet(BulletObj::BD_LEFT);

     // vị trí phát ra đạn
     int xBul = 0, yBul = 0;
     InitStartPos(xBul, yBul);
     pBul->set_xy_pos(xBul, yBul);
     pBul->Init(sBossFire, screen);

    pBul->set_x_val(10);
    m_BulList.push_back(pBul);
}

void BossMonsterObj::DoBullet(SDL_Renderer* screen, bool is_pause /*false*/)
{
    if (is_alive_ == false)
        return;

    if (frame_ == 22 && on_ground_)
    {
        AddBullet(screen);
    }

    for (size_t iBul = 0; iBul < m_BulList.size(); iBul++)
        {
            BulletObj* pBul = m_BulList.at(iBul);
            if (pBul != NULL)
            {
                if (pBul->get_is_move() == true)
                {
                    int xOffset = -400;
                    int xLimit = x_pos_ + xOffset;
                    pBul->Show(screen);
                    if (is_pause == false)
                    {
                        pBul->HandleMove(xLimit, xLimit);
                    }
                }
                else
                {
                    pBul->Free();
                    m_BulList.erase(m_BulList.begin() + iBul);
                }
            }
        }
}


void BossMonsterObj::InitStartPos(int& xBul, int& yBul)
{
    xBul = x_pos_ - 120;
    yBul = y_pos_ + height_frame_ - 35;
}

void BossMonsterObj::Show(SDL_Renderer* des, bool is_pause /*false*/)
{
    if (is_alive_ == true)
    {
        m_Rect.x = x_pos_ - GameMap::GetInstance()->GetMap()->getStartX();
        m_Rect.y = y_pos_ - GameMap::GetInstance()->GetMap()->getStartY();

        if (is_pause == false)
        {
            if (SDL_GetTicks() - m_DelayTimeList[frame_] > m_PassTime)
            {
                    m_PassTime = SDL_GetTicks();
                    frame_++;
                    if (frame_ > BOSS_FRAME_NUM - 1)
                    {
                        frame_ = 0;
                    }
            }
        }

        SDL_Rect* currentClip = &m_FrameClip[frame_];
        BaseObj::Render(des, currentClip);
    }
}

void BossMonsterObj::DoAction(SDL_Renderer* des, bool is_pause  /*false*/)
{
    if (is_pause == false && is_alive_ == true)
   {
       FreeFalling();
       y_pos_ += y_val_;
   }
}

void BossMonsterObj::FreeFalling()
{

    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL || is_alive_ == false)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    y_val_ += GRAVITY_SPEED;
    if (y_val_ >= MAX_FALL_SPEED)
    {
        y_val_ = MAX_FALL_SPEED;
    }

    int width_frame = m_Rect.w;
    int height_frame = m_Rect.h;

    // Xác định 2 vị trí x1 và x2 để tìm 2 ô tile tương ứng với vị trí player đang rơi
    int curTileX1 = x_pos_ / TILE_SIZE;
    int curTileX2 = (x_pos_ + width_frame) / TILE_SIZE;
    int curTileMid = (x_pos_ + width_frame*0.5) / TILE_SIZE;

    // vì rơi nên sẽ tìm ô tile kế tiếp
    int yPosNext = y_pos_ + height_frame + y_val_;
    int nextTileY = yPosNext / TILE_SIZE;

    std::vector<BlockMap*> bList;
    BlockMap* pBlock1 = GetBlockMap(nextTileY, curTileX1);
    if (pBlock1 != NULL && pBlock1->GetTile() != NULL)
    {
        bList.push_back(pBlock1);
    }

    BlockMap* pBlock2 = GetBlockMap(nextTileY, curTileX2);
    if (pBlock2 != NULL && pBlock2->GetTile() != NULL)
    {
        bList.push_back(pBlock2);
    }

    BlockMap* pBlock3 = GetBlockMap(nextTileY, curTileMid);
    if (pBlock3 != NULL && pBlock3->GetTile() != NULL)
    {
        bList.push_back(pBlock3);
    }

    for (auto block : bList)
    {
        int ret = CheckBlock(block);
        if (ret == 1)
        {
            on_ground_ = true;
            y_pos_ = nextTileY*TILE_SIZE - height_frame;
            y_val_ = 0;
            break;

        }
    }
}

int BossMonsterObj::CheckBlock(BlockMap* block)
{
    GameMap* pMap = GameMap::GetInstance();

    std::string sTile = block->getType();
    bool bSkip = pMap->CheckSkipMap(sTile);
    int vProduct = pMap->GetValueProduct(sTile);

    if (bSkip == false && vProduct == 0)
    {
        return 1;
    }
    else if (vProduct > 0)
    {
        return -1;
    }

    return -1;
}

BlockMap* BossMonsterObj::GetBlockMap(int y, int x)
{
    BlockMap* pBlock = NULL;
    GameMap* pMap = GameMap::GetInstance();
    if (pMap != NULL && pMap->GetMap())
    {
        Map* data_map = pMap->GetMap();
        VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
        if (tile_list.empty() == false)
        {
            UINT yNum = tile_list.size();
            if (y >= 0 && y < yNum)
            {
                UINT xNum = tile_list[y].size();
                if (x >= 0 && x < xNum)
                {
                    pBlock = tile_list[y][x];
                }
            }
        }
    }

    return pBlock;
}


bool BossMonsterObj::CheckCol(const SDL_Rect& rect, SDL_Rect& exp_rect, bool bBullet /*= false*/)
{
    if (is_alive_ == false)
        return false;

    bool bRet = false;
    SDL_Rect boss_rect = GetRectFrame();
    boss_rect.x += 30;
    boss_rect.y += 30;
    bool bColRect = TBase::CheckCollision(rect, boss_rect);
    if (bColRect)
    {
        bRet = true;
        exp_rect = boss_rect;

        if (bBullet == true)
        {
            m_HitCount++;
        }
    }

    return bRet;
}

bool BossMonsterObj::CheckColBul(const SDL_Rect& rect_object)
{
    for (int bul = 0; bul < m_BulList.size(); bul++)
    {
        BulletObj* pBull = m_BulList[bul];
        if (pBull != NULL)
        {
            bool bCol = TBase::CheckCollision(rect_object, pBull->GetRect());
            if (bCol == true)
             {
                  pBull->set_is_move(false);
                  return true;
             }
        }
    }

    return false;
}