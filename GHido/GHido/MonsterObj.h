#pragma once

#include "DefineGame.h"
#include "TBase.h"
#include "BaseObj.h"
#include "BulletObj.h"

static char sMBird [] = {"image//m_bird.png"};
static char sMShot [] = {"image//m_shot.png"};
static char sMStand [] = {"image//m_stand.png"};
static char kImgBulletCir[] = { "image//bul_cir.png" };

class MonsterObj : public BaseObj
{
public:
    MonsterObj(int clip_num);
    ~MonsterObj(void);

    enum MObjType
    {
        M_NONE = 1000,
        M_MOVING_SHOT,
        M_STAND_ATTACK,
        M_BIRD,
    };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void DoAction();
    void CheckToMap();
    void DoLeft();
    void DoRight();
    void DoDown();
    void Update();
    void Show(SDL_Renderer* des, bool is_pause = false);

    void set_type(int type) {type_ = type;}
    void set_x_val(int xp) { x_val_ = xp; }
    void set_y_val(int yp) { y_val_ = yp; }
    void set_xpos(const int& xps) { x_pos_ = xps; }
    void set_ypos(const int& yps) { y_pos_ = yps; }
    void set_clips();
    void set_is_alive(bool is_alive) { is_alive_ = is_alive; }
    void SetClipNum(int clip_num) { m_ClipNum = clip_num; }

    int get_x_pos() { return x_pos_; }
    int get_y_pos() { return y_pos_; }
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }
    bool get_is_alive() { return is_alive_; }
    int GetType() const { return type_; }
    SDL_Rect GetRectFrame();
    void ResetBird();
    VT(BulletObj*) GetBuls() {return m_BulletList;}

    void AddBullet(SDL_Renderer* screen);
    void DoBullet(SDL_Renderer* screen, bool is_pause = false);
    void InitStartPos(int& xBul, int& yBul);
protected:
    Map* pMap_;

    VT(SDL_Rect) m_FrameClipList;

    bool is_alive_;

    int on_ground_;
    int width_frame_;
    int height_frame_;
    int frame_;
    int v_dir_;
    int type_;

    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;
    float x_plus_;

    unsigned long m_PassTime;
    VT(int) m_DelayTimeList;
    int m_ClipNum;
    VT(BulletObj*) m_BulletList;  // đạn cho moster bắn đạn
};

////////////////////////////////////////////////

static INT gMonsterShot[] = { 12, 22, 38, 76, };
static INT gMonsterStand[] = { 32, 43, 52, 63, 79};

class MList
{

public:
    MList();
    ~MList();
    static MList* GetInstance()
    {
        if (instance_ == NULL)
            instance_ = new MList();
        return instance_;
    }

    
    void BuildMonster(SDL_Renderer* screen);
    void Render(SDL_Renderer* screen, bool is_pause = false);
    void Free();
    void DestroyInstance();

    void MakeMShot(SDL_Renderer* screen);
    void MakeMStand(SDL_Renderer* screen);
    void MakeMBird(SDL_Renderer* screen);

    bool CheckCol(const SDL_Rect& rect, SDL_Rect& exp_rect, bool bDel = true);
    bool CheckColBul(const SDL_Rect& rect);
private:
    static MList* instance_;
    std::vector <MonsterObj*> m_MonsterList;

};