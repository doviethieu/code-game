#pragma once

#include "DefineGame.h"
#include "BaseObj.h"

class PlayerBlood
{
public:
    PlayerBlood(void);
    ~PlayerBlood(void);

    int GetCount() {return m_Count;}
    void Init(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void FreeData();
    void ReStart(SDL_Renderer* screen);
    bool IsEmpty();
    void PlusUpdate(SDL_Renderer* screen);
    void MinusUpdate(); 

private:
    bool m_Empty;
    int m_Count;
    VT(BaseObj*) m_BloodList;
    std::string m_path;
};

////////////////////////////////////////////////////////////////////


class BossBlood
{
public:
    BossBlood(void);
    ~BossBlood(void);

    int GetCount() {return m_Count;}
    void Init(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void FreeData();
    void ReStart(SDL_Renderer* screen);
    bool IsEmpty();
    void PlusUpdate(SDL_Renderer* screen);
    void MinusUpdate(); 

private:
    bool m_Empty;
    int m_Count;
    VT(BaseObj*) m_BloodList;
    std::string m_path;
};