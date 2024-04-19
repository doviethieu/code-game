#include "stdafx.h"
#include "PrincessObj.h"


PrincessObj::PrincessObj(void)
{
    y_val_ = 0.0;
    x_pos_ = 0;
    y_pos_ = 0;
    on_ground_ = false;
}


PrincessObj::~PrincessObj(void)
{
}

bool PrincessObj::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    return ret;
}


void PrincessObj::Show(SDL_Renderer* des)
{
     Map* data_map = GameMap::GetInstance()->GetMap();
     m_Rect.x = x_pos_ - data_map->getStartX();
     m_Rect.y = y_pos_ - data_map->getStartY();
     BaseObj::Render(des);
}

void PrincessObj::DoAction(SDL_Renderer* des)
{
   FreeFalling();
   y_pos_ += y_val_;
}

void PrincessObj::FreeFalling()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
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

int PrincessObj::CheckBlock(BlockMap* block)
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

BlockMap* PrincessObj::GetBlockMap(int y, int x)
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

