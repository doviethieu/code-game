#include "stdafx.h"
#include "BulletObj.h"


BulletObj::BulletObj(void)
{
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    is_move_ = true;
    bullet_dir_ = BD_RIGHT;

    x_scope_ = 0;
}


BulletObj::~BulletObj(void)
{

}

BulletObj* BulletObj::Clone()
{
    BulletObj* pCLone = new BulletObj();
    pCLone->bullet_dir_ = this->bullet_dir_;
    pCLone->is_move_ = this->is_move_;
    pCLone->x_val_ = this->x_val_;
    pCLone->y_val_ = this->y_val_;
    pCLone->x_pos_ = this->x_pos_;
    pCLone->y_pos_ = this->y_pos_;
    pCLone->x_scope_ = this->x_scope_;

    return pCLone;
}

bool BulletObj::Init(std::string path, SDL_Renderer* screen)
{
    bool bRet = LoadImg(path, screen);
    return bRet;
}

void BulletObj::HandleMove(const int& xb1, const int& xb2)
{
    if (bullet_dir_ == BD_RIGHT)
    {
        x_pos_ += x_val_;
        if (x_pos_ > xb2)
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == BD_LEFT)
    {
        x_pos_ -= x_val_;
        if (x_pos_ < xb1)
        {
            is_move_ = false;
        }
    }
}

bool BulletObj::CheckToMap()
{
    Map* map_data = GameMap::GetInstance()->GetMap();
    SDL_Rect blRect = GetRect();
    blRect.x = x_pos_ - map_data->getStartX();
    blRect.y = y_pos_ - map_data->getStartY();

    int x = (x_pos_) / TILE_SIZE;
    int y = (y_pos_) / TILE_SIZE;

    bool IsInside = true;
    IsInside = TBase::CheckInsideMapX(x, x);
    IsInside &= TBase::CheckInsideMapY(y, y);

    if (IsInside)
    {
        BlockMap* pBlock = map_data->GetTile1().at(y).at(x);
        if (pBlock->GetTile() != NULL)
        {
            std::string val = pBlock->getType();
            bool bSkip = GameMap::GetInstance()->CheckSkipMap(val);
            if (bSkip == false)
            {
                if (bullet_dir_ == BD_LEFT)
                {
                    x_pos_ = (x + 1) * TILE_SIZE;
                }
                else if (bullet_dir_ == BD_RIGHT)
                {
                    x_pos_ = x * TILE_SIZE;
                }

                is_move_ = false;
                m_Rect.x = x_pos_ - map_data->getStartX();
                return true;
            }
        }
    }

    return false;
}

void BulletObj::Show(SDL_Renderer* des)
{
    Map* map_data = GameMap::GetInstance()->GetMap();
    m_Rect.x = x_pos_ - map_data->getStartX();
    m_Rect.y = y_pos_ - map_data->getStartY();
    BaseObj::Render(des);
}


SDL_Rect BulletObj::GetExpPos()
{
    SDL_Rect rectExp = m_Rect;
    rectExp.y += m_Rect.h*0.5;

    return rectExp;
}