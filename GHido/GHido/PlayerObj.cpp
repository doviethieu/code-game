#include "stdafx.h"
#include "PlayerObj.h"
#include "TBase.h"
#include "Music.h"

PlayerObj::PlayerObj(void)
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    status_ = WALK_RIGHT;

    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;

    x_val_ = 0;
    y_val_ = 0;

    on_ground_ = false;

    y_val_jump_ = 0;

    m_bMinusBlood = false;

    is_falling_ = false;
    alive_time_ = 0;

    m_CoinCount = 0;
}


PlayerObj::~PlayerObj(void)
{
}



bool PlayerObj::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = m_Rect.w/PLAYER_FRAMES;
        height_frame_ = m_Rect.h;
        SetClip();
    }

    return ret;
}

SDL_Rect PlayerObj::GetRectFrame()
{
  SDL_Rect rect;
  rect.x = m_Rect.x;
  rect.y = m_Rect.y;
  rect.w = m_Rect.w/ PLAYER_FRAMES;
  rect.h = m_Rect.h;
  return rect;
}

void PlayerObj::SetClip()
{
  if (width_frame_ > 0 && height_frame_ > 0)
  {
      for (int i = 0; i < PLAYER_FRAMES; i++)
      {
          m_FrameClip[i].x = width_frame_*i;
          m_FrameClip[i].y = 0;
          m_FrameClip[i].w = width_frame_;
          m_FrameClip[i].h = height_frame_;
      }
  }
}

void PlayerObj::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (is_falling_ == true || alive_time_ > 0)
    {
        return;
    }

     if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT: // di phải
        {
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            input_type_.left_ = 0;
            UpdateImagePlayer();
            break;
        }
        case SDLK_LEFT: // đi trái
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0;
            UpdateImagePlayer();
            break;
        }
        case SDLK_RETURN:
        {
            break;
        }
        default:
            break;
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        // khi bấm key up, thì đưa các trạng thái di chuyển ở trên về = 0
        // nhân vật đứng im
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            input_type_.right_ = 0;
            break;
        case SDLK_LEFT:
            input_type_.left_ = 0;
            break;
        default:
            break;
        }
    }
      else if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        // khi click chuột trái thì bắn đạn
        if (events.button.button == SDL_BUTTON_LEFT)
        {
           CreateBullet(screen);
        }
        else if (events.button.button == SDL_BUTTON_RIGHT)
        {
            // kích chuột phải thì nhảy lên
            if (on_ground_ == true)
            {
                on_ground_ = false;
                input_type_.jump_ = 1;
                input_type_.down_ = 0;
            }
        }
    }
    else if (events.type == SDL_MOUSEBUTTONUP)
    {
        if (events.button.button == SDL_BUTTON_RIGHT)
        {
            input_type_.jump_ = 0;
        }
    }
}

void PlayerObj::CreateBullet(SDL_Renderer* screen)
{
        BulletObj* p_bullet = new BulletObj();
        bool bRet = p_bullet->Init(kImgBullet, screen);
        if (bRet == true)
        {
            Music::GetInstance()->PlaySoundGame(Music::BULLET_SOUND);
            INT xBul = x_pos_;
            if (status_ == WALK_LEFT)
            {
                // quay bên trái thì bắn sang bên trái
                p_bullet->set_dir_bullet(BulletObj::BD_LEFT);
                xBul -= p_bullet->GetRect().w;
                xBul -= x_val_;
            }
            else
            {
                // quay bên phải bắn sang ben phải
                p_bullet->set_dir_bullet(BulletObj::BD_RIGHT);
                xBul += width_frame_;
                xBul += x_val_;
            }

            INT yBul = y_pos_ + height_frame_*0.58;

            p_bullet->set_xy_pos(xBul, yBul);
            // tốc độ đạn
            p_bullet->set_x_val(15);
            m_BulList.push_back(p_bullet);
        }
}

void PlayerObj::HandleBullet(SDL_Renderer* des, bool is_pause /* = false*/)
{

    int xBoder1 = 0;
    int xBoder2 = 0;

    xBoder1 = x_pos_ - 300;
    xBoder2 = x_pos_ + width_frame_ + 300;

    for (int i = 0; i < m_BulList.size(); i++)
    {
        BulletObj* bullet = m_BulList.at(i);
        if (bullet != NULL)
        {
            if (bullet->get_is_move())
            {
                bullet->Show(des);
                if (is_pause == false) // nếu dang pause thì ko di chuyển đạn
                {
                    bullet->HandleMove(xBoder1, xBoder2);

                    // kiểm tra dạn va chạm với bản đồ
                    bool ret = bullet->CheckToMap();
                    if (ret == true)
                    {
                        bullet->set_is_move(false);

                        ExplosionObj* pExp = new ExplosionObj(4);
                        bool ret = pExp->LoadImg(sBoomName, des);
                        if (ret)
                        {
                            SDL_Rect rc_pos = bullet->GetExpPos();
                            pExp->SetXP(rc_pos);
                            ExpList::GetInstance()->Add(pExp);
                        }
                        continue;
                    }
                }
            }
            else
            {
                // nếu đạn đi quá phạm vi thì sẽ xóa đạn
                RemoveBullet(i);
            }
        }
    }
}


void PlayerObj::RemoveBullet(const int& idx)
{
    if (m_BulList.size() > 0 && idx < m_BulList.size())
    {
        BulletObj* bullet = m_BulList.at(idx);
        m_BulList.erase(m_BulList.begin() + idx);

        if (bullet)
        {
            bullet->Free();
            bullet = NULL;
        }
    }
}


void PlayerObj::DoAction(SDL_Renderer* des)
{
    Map* data_map = GameMap::GetInstance()->GetMap();

    if (is_falling_ == false && alive_time_ == 0)
    {
        if (input_type_.left_ == 1)
        {
                // giá trị x_val bị giảm 1 lương speed
                x_val_ = -PLAYER_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
                // với bên phải thì tăng 1 lượng speed
                x_val_ = PLAYER_SPEED;
        }
       else if (input_type_.left_ == 0 && input_type_.right_ == 0)
       {
                // khi ko di chyển, thì giá trị tăng = 0
                x_val_ = 0;
       }

        if (input_type_.jump_ == 1)
        {
            input_type_.jump_ = 0;
            y_val_jump_ = -PLAYER_HIGHT_VAL;

        }

        if (y_val_jump_ < 0)
        {
            DoJump();
        }
        else
        {
            FreeFalling();
        }

        CheckToMap(des);

        data_map->UpdateXMapInfo(x_pos_);
        data_map->UpdateYMapInfo(y_pos_);
    }
    else
    {
         // nếu nhân vật bị chết, thì
        // thời gian hồi sinh được thiết lập, ví dụ = 60
        if (alive_time_ > 0)
        {
            // bắt đầu giảm thời gian hồi sinh
            alive_time_ -= 1;
            if (alive_time_ <= 0)
            {
                // khi thời gian chời hồi sinh = 0
                alive_time_ = 0;
                x_val_ = 0;
                y_val_ = 0;
                // bắt đầu hồi sinh player
                is_falling_ = false;
                frame_ = 0;
                ResetAlive();
                //LoadImg(sPlayerMove, des);
            }
        }
    }
}


void PlayerObj::ResetAlive()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL) return;

    // xóa các hành vi trước đó
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;

    // tính toán lại vị trí hồi sinh
    // lùi sau vị trí chết 4 tile
    x_pos_ = x_pos_ - TILE_SIZE * 4;
    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }

    y_pos_ = 200;

    // tính toán lại vị trí vẽ player trên màn hình
    Map* data_map = GameMap::GetInstance()->GetMap();

    int mxStart = data_map->getStartX();
    int myStart = data_map->getStartY();

    m_Rect.x = x_pos_ - mxStart;
    m_Rect.y = y_pos_ - myStart;
}

void PlayerObj::DoJump()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;


    // Tại vị trí player đang thực hiện nhảy, x_pos, y_pos
    int curTileX1 = (x_pos_) / TILE_SIZE;  // tìm ra chỉ số của ô tile map thứ nhất

    // lấy giá trị x_pos + thêm chiều rộng đề tìm ra ô tile máp thứ 2
    int xPosCur = (x_pos_ + width_frame_ - EPXILON);
    int curTileX2 = xPosCur / TILE_SIZE;

    // Nếu curTileX1, và curTileX2 bằng nhau, nghĩa là player đứng nguyên trong 1 ô tile map
    // Nhưng nếu nó khác nhau, nghĩa là player đang đứng ở vị trí, 1 nửa ô tile này và 1 nửa ô tile kia.
    // Do đó, phải check 2 ô tile với chiều X

    // Khi nhảy lên, thì giá trị y_pos bị giảm đi 1 lượng (Phép công, nhưng chú ý :y_val_jum_ có giá trị âm)
    // cần tìm ổ tile theo chiều y sau khi giảm.
    // Mục đích là xem ô tile đó có phải tile va chạm hay ko, nếu có va chạm, thì player ko thể nhảy qua tile đó được.
    int prevTileY = (y_pos_ + y_val_jump_) / TILE_SIZE;

    std::vector<BlockMap*> bList;
    // Lấy dữ liệu ô tile map tại vị trí 1
    BlockMap* pBlock1 = GetBlockMap(prevTileY, curTileX1);
    if (pBlock1 != NULL && pBlock1->GetTile() != NULL)
    {
        bList.push_back(pBlock1);
    }

    BlockMap* pBlock2 = GetBlockMap(prevTileY, curTileX2);
    if (pBlock2 != NULL && pBlock2->GetTile() != NULL)
    {
        bList.push_back(pBlock2);
    }

    for (auto block : bList)
    {
        int ret = CheckBlock(block);
        if (ret == 1)
        {
            y_pos_ = (prevTileY + 1)*TILE_SIZE;
            y_val_jump_ = 0;
            break;
        }
    }

    // Nếu ko xảy ra bất cứ va chạm nào, thì 
    // y_pos sẽ giảm 1 lượng giá trị y_val_jump
    if (y_val_jump_ < 0)
    {
        y_pos_ += y_val_jump_;
        y_val_jump_++;
    }
    else
    {
        y_val_jump_ = 0;
    }
}

void PlayerObj::FreeFalling()
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

    // Xác định 2 vị trí x1 và x2 để tìm 2 ô tile tương ứng với vị trí player đang rơi
    int curTileX1 = x_pos_ / TILE_SIZE;
    int curTileX2 = (x_pos_ + width_frame_) / TILE_SIZE;
    int curTileMid = (x_pos_ + width_frame_*0.5) / TILE_SIZE;

    // vì rơi nên sẽ tìm ô tile kế tiếp
    int yPosNext = y_pos_ + height_frame_ + y_val_;
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
            y_pos_ = nextTileY*TILE_SIZE - height_frame_;
            y_val_ = 0;
            break;
        }
    }
}


void PlayerObj::CheckToMap(SDL_Renderer* des)
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();

    if (input_type_.right_ == 1) // MOVE RIGHT
    {
        DoRight();
    }
    else if (input_type_.left_ == 1) // MOVE LEFT
    {
        DoLeft();
    }

    // Nếu xử lý trải, phải không va chạm với tile
    // giá trị x_val_ sẽ khác 0
    // và x_pos, y_pos được thay đổi vị trí
    // và như vậy player di chuyển tiếp
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    // nếu di chuyển trái, và khi di mãi về đầu bản đồ
    if (x_pos_ < 0)
    {
        //thì sẽ chặn lại ko di chuyển được nữa
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ >= data_map->getMaxX())
    {
        // Hoặc di chuyển tới cuối bản đồ
        x_pos_ = data_map->getMaxX() - width_frame_ - EPXILON;
    }

    // giải sử khi bạn đứng ở tile rất cao, và nhảy cái vượt quá màn hình theo chiều y
    // thì cũng chặn lại, ko cho vượt quá
    if (y_pos_ < 0)
    {
        y_pos_ = 0;
    }
    else if (y_pos_ + height_frame_ >= data_map->getMaxY() + 2*TILE_SIZE)
    {
        // Khi y_pos vượt quá chiều cao của map theo phương y
        // nghĩa là player bị rơi xuống vực
        // coi như player chết, máu bị giảm đi 1 đơn vị
        m_bMinusBlood = true;
        is_falling_ = true; // thiết lập trạng thái rơi = true
        alive_time_ = 100;  // thiết lập thời gian chờ hồi sinh là 100
        //Music::GetInstance()->PlaySoundGame(Music::BLOOD_SOUND);
    }
}

//////////////////////////////////////////////////////////////////////////
// xử lý va chạm với map khi di chuyển trái
// Khác với nhảy và rơi, left right chỉ có 1 chiều.
// và khi va chạm với ô tile a, thì player cũng phải đứng trọn vẹn trong  1 layer
// (Trừ khi chúng ta cố tình vẽ tile a nằm 1 nửa tile x, 1 nửa tile y)
//////////////////////////////////////////////////////////////////////////
void PlayerObj::DoLeft()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int yPos = (y_pos_ + height_frame_ - 1);
    int xPosPrev = (x_pos_ + x_val_);

    // tìm ô tile sẽ di chuyển đến (bên trái) với curTileY và prevTileX
    int curTileY = yPos / TILE_SIZE;
    int prevTileX = xPosPrev / TILE_SIZE;

    bool IsInside = TBase::CheckInsideMap(prevTileX, curTileY);
    if (IsInside) 
    {
        // đảm bảo bên trong map
        BlockMap* pBlock = GetBlockMap(curTileY, prevTileX);
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                int ret = CheckBlock(pBlock);
                if (ret == 1)
                {
                    // player sẽ bị cản lại tại ô tile bên trái
                    x_pos_ = prevTileX*TILE_SIZE + width_frame_;
                    // ko cho di chuyển trái tiếp
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

//////////////////////////////////////////////////////////////////////////
// Hàm xử lý right cũng tương tự như hàm xử lý trái
//////////////////////////////////////////////////////////////////////////
void PlayerObj::DoRight()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int xPosNext = (x_pos_ + width_frame_ + x_val_);
    int yPosEnd = (y_pos_ + height_frame_ - 1);

    int nextTileX = xPosNext / TILE_SIZE;
    int curTileY = yPosEnd / TILE_SIZE;

    bool IsInside = TBase::CheckInsideMap(nextTileX, curTileY);
    if (IsInside)
    {
        BlockMap* pBlock = GetBlockMap(curTileY, nextTileX);
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                int ret = CheckBlock(pBlock);
                if (ret == 1)
                {
                    x_pos_ = nextTileX*TILE_SIZE - width_frame_;
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

int PlayerObj::CheckBlock(BlockMap* block)
{
    GameMap* pMap = GameMap::GetInstance();

    std::string sTile = block->getType();

    if (sTile == "WB1")
    {
        int a = 5;
    }
    bool bSkip = pMap->CheckSkipMap(sTile);
    int vProduct = pMap->GetValueProduct(sTile);

    /*if (sTile == TILE_GUN)
    {
        m_Shot_Type = SHOT_LVL_1;
        block->setTile(0);
        return 0;
    }*/
    if (bSkip == false && vProduct == 0)
    {
        return 1;
    }
    else if (vProduct > 0)
    {
        Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
        block->setTile(0);
        m_CoinCount += vProduct;
        return 2;
    }

    return -1;
}

BlockMap* PlayerObj::GetBlockMap(int y, int x)
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

void PlayerObj::UpdateImagePlayer()
{
    if (status_ == WALK_LEFT)
    {
        m_Flip = true;
    }
    else if (status_ == WALK_RIGHT)
    {
        m_Flip = false;
    }
}

void PlayerObj::Show(SDL_Renderer* des)
{
     Map* data_map = GameMap::GetInstance()->GetMap();
     m_Rect.x = x_pos_ - data_map->getStartX();
     m_Rect.y = y_pos_ - data_map->getStartY();

     if (is_falling_ == false && alive_time_ == 0)
     {
        if (input_type_.left_ == 0 && input_type_.right_ == 0 /*|| on_ground_ == false*/)
        {
             frame_ = 0;
        }
        else
        {
             ++frame_;
             if (frame_ >= PLAYER_FRAMES)
             {
                 frame_ = 0;
             }
        }

        SDL_Rect* currentClip = &m_FrameClip[frame_];
        BaseObj::Render(des, currentClip);
     }
}

void PlayerObj::ReStart()
{
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    x_pos_ = 100;
    y_pos_ = 100;
    m_CoinCount = 0;


    on_ground_ = false;
    is_falling_ = false;

    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE;
    alive_time_ = 0;
    m_bMinusBlood = false;
    y_val_jump_ = 0;
}

bool PlayerObj::IsDeath()
{
    if (alive_time_ == 0)
    {
        return false;
    }

    return true;
}