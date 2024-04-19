#pragma once

#include "DefineGame.h"
#include "BaseObj.h"
#include "MapData.h"
#include "BulletObj.h"

#define BOSS_FRAME_NUM 32


static char sBossFire[] = {"image//boss_fire.png"};

class BossMonsterObj : public BaseObj
{
public:
    BossMonsterObj(void);
    ~BossMonsterObj(void);

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void DoAction(SDL_Renderer* des, bool is_pause = false);
    void FreeFalling();
    BlockMap* GetBlockMap(int y, int x);
    int CheckBlock(BlockMap* block);
    void Set_Pos(int xp, int yp) {x_pos_ = xp; y_pos_ = yp;}
    void SetClip();
    SDL_Rect GetRectFrame();
    void Show(SDL_Renderer* des, bool is_pause = false);

    void AddBullet(SDL_Renderer* screen);
    void InitStartPos(int& xBul, int& yBul);
    void DoBullet(SDL_Renderer* screen, bool is_pause = false);
    bool CheckCol(const SDL_Rect& rect, SDL_Rect& exp_rect, bool bBullet = false);
    int GetHit() {return m_HitCount;};
    void SetHit(int nHit) {m_HitCount = nHit;};
    void SetDeath() {is_alive_ = false;}
    void UpdateImg(std::string path, SDL_Renderer* screen);
    bool isAlive() {return is_alive_;};
    bool CheckColBul(const SDL_Rect& rect_object);
private:
    float x_pos_;
    float y_pos_;
    float y_val_;;
    bool on_ground_;
    bool is_alive_;
    // Clip 
    SDL_Rect m_FrameClip[BOSS_FRAME_NUM];
    int frame_;
    
    int width_frame_;
    int height_frame_;

    unsigned long m_PassTime;
    VT(int) m_DelayTimeList;
    VT(BulletObj*) m_BulList;

    int m_HitCount;
};

