#include "stdafx.h"
#include "PlayerObj.h"


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
}

void PlayerObj::DoAction(SDL_Renderer* des)
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

    x_pos_ += x_val_;
    y_pos_ += y_val_;
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
    m_Rect.x = x_pos_;
    m_Rect.y = y_pos_;


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