#pragma once
#include "DefineGame.h"
#include "BaseObj.h"
#include "MapData.h"

#define PLAYER_SPEED        8    // tốc độ di chuyển
#define PLAYER_HIGHT_VAL    19   // tốc độ nhảy của player
#define PLAYER_FRAMES       8    // số lượng khung hình

class PlayerObj : public BaseObj
{
public:

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };

    PlayerObj(void);
    ~PlayerObj(void);

    int get_frame_width()  const { return width_frame_; }
    int get_frame_height() const { return height_frame_; }

    int get_x_pos()       const { return x_pos_; }
    int get_y_pos()        const { return y_pos_; }
    void Set_Pos(int xp, int yp) {x_pos_ = xp; y_pos_ = yp;}

    bool LoadImg(std::string path, SDL_Renderer* screen);
    SDL_Rect GetRectFrame();

    void SetClip();
    void Show(SDL_Renderer* des);

    void DoAction(SDL_Renderer* des);
    void CheckToMap(SDL_Renderer* des);
    void DoRight();
    void DoLeft();
    void DoJump();
    void FreeFalling();
    BlockMap* GetBlockMap(int y, int x);
    int CheckBlock(BlockMap* block);
    void UpdateImagePlayer();
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void ResetAlive();
    bool CheckMinusBlood() {return m_bMinusBlood;};
    void ResetFlagBlood() { m_bMinusBlood = false;}
private:
    SDL_Rect m_FrameClip[PLAYER_FRAMES];
    int status_;
    int frame_;
    Input input_type_;

    int width_frame_;
    int height_frame_;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    bool on_ground_;
    float y_val_jump_;

    bool m_bMinusBlood;

    int m_CoinCount;

    bool is_falling_;
    int alive_time_;
};
