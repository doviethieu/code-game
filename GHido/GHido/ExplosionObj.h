#pragma once

#include "BaseObj.h"
#include "DefineGame.h"
#include "Music.h"

class ExplosionObj : public BaseObj
{
public:
    ExplosionObj(void);
    ~ExplosionObj(void);
    ExplosionObj(int frameNum);

    virtual bool LoadImg(std::string path, SDL_Renderer* screen);

    int get_frame() const { return frame_; }
    int get_frame_width() const { return frame_width_; }
    int get_frame_height() const { return frame_height_; }

    void set_clips();
    void set_frame(const int& fr) { frame_ = fr; }
    void SetXP(SDL_Rect rect_pos);
    void ImpRender(SDL_Renderer* screen, bool is_pause = false);
    void SetActive(bool bActive) { m_isActive = bActive; }
    bool GetActive() const { return m_isActive; }
private:
    int m_Frame;  // số lượng frame cho vụ nổ (4)
    VT(SDL_Rect) frame_clip_;  // mảng chứa thông tin frame
    int frame_width_;   // chiều rộng 1 frame
    int frame_height_;  // chiều cao 1 frame
    unsigned int frame_; // frame hiện tại khi xử lý 
    bool m_isActive;     // frame hoạt động hay ngưng
};

//////////////////////////////////////////////
class ExpList
{
public:
    ExpList();
    ~ExpList();

    static ExpList* GetInstance()
    {
        if (instance_ == NULL)
            instance_ = new ExpList();
        return instance_;
    }

    void Render(SDL_Renderer* screen, bool is_pause = false);
    void Add(ExplosionObj* pObj);
    bool IsEmpty();
    void ResetExp() { m_ExpList.clear(); }
private:
    std::vector<ExplosionObj*> m_ExpList;
    static ExpList* instance_;
};