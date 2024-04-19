#include "stdafx.h"
#include "PlayerBlood.h"


PlayerBlood::PlayerBlood(void)
{
    m_Count = 0;
    m_path = "";
}


PlayerBlood::~PlayerBlood(void)
{
    FreeData();
}

void PlayerBlood::Init(SDL_Renderer* screen)
{
    m_path = "image\\player_alive.png";
    m_Count = 3;

    int xp = 0;
    int yp = 5;

    for (int i = 0; i < m_Count; i++)
    {
        BaseObj* pObjAlive = new BaseObj();
        bool ret = pObjAlive->LoadImg(m_path, screen);
        if (ret == true)
        {
            if (i == 0)
            {
                xp = 5;
            }
            else
            {
                xp = xp + pObjAlive->GetRect().w + 5;
            }
            pObjAlive->SetRect(xp, yp);
            m_BloodList.push_back(pObjAlive);
        }
    }
}


void PlayerBlood::Show(SDL_Renderer* screen)
{
	for (auto b : m_BloodList)
	{
		b->Render(screen);
	}
}

// giảm 1 mang song
void PlayerBlood::MinusUpdate()
{
	int size = m_BloodList.size();
	if (size > 0)
	{
		BaseObj* pObj = m_BloodList.at(size - 1);
		pObj->Free();
		m_BloodList.pop_back();
        m_Count--;
        if (m_Count < 0)
        {
            m_Count = 0;
        }
	}
}

void PlayerBlood::PlusUpdate(SDL_Renderer* screen)
{
    int size = m_BloodList.size();
    SDL_Rect last_rect;
    if (size > 0)
    {
        BaseObj* pObj = m_BloodList.at(size - 1);
        last_rect = pObj->GetRect();

        int xp = last_rect.x + pObj->GetRect().w + 2;
        int yp = last_rect.y;

        BaseObj* pObjNew = new BaseObj();
        bool ret = pObjNew->LoadImg(m_path, screen);
        pObjNew->SetRect(xp, yp);
        m_BloodList.push_back(pObjNew);
        m_Count++;
    }
}

bool PlayerBlood::IsEmpty()
{
    if (m_Count > 0)
    {
        return false;
    }
    return true;
}

void PlayerBlood::FreeData()
{
	if (IsEmpty() == false)
	{
		for (auto x : m_BloodList)
		{
			x->Free();
		}

		m_BloodList.clear();
	}
}


void PlayerBlood::ReStart(SDL_Renderer* screen)
{
    m_Count = 3;
    FreeData();
    Init(screen);
}

//////////////////////////////////////
BossBlood::BossBlood(void)
{
    m_Count = 0;
    m_path = "";
}


BossBlood::~BossBlood(void)
{
    FreeData();
}

void BossBlood::Init(SDL_Renderer* screen)
{
    m_path = "image\\Boss_blood.png";
    m_Count = 10;

    int xp = SCREEN_WIDTH - 400;
    int yp = 5;

    for (int i = 0; i < m_Count; i++)
    {
        BaseObj* pObjAlive = new BaseObj();
        bool ret = pObjAlive->LoadImg(m_path, screen);
        if (ret == true)
        {
            if (i != 0)
            {
                xp = xp + pObjAlive->GetRect().w + 5;
            }
            pObjAlive->SetRect(xp, yp);
            m_BloodList.push_back(pObjAlive);
        }
    }
}


void BossBlood::Show(SDL_Renderer* screen)
{
	for (auto b : m_BloodList)
	{
		b->Render(screen);
	}
}

// giảm 1 mang song
void BossBlood::MinusUpdate()
{
	int size = m_BloodList.size();
	if (size > 0)
	{
		BaseObj* pObj = m_BloodList.at(size - 1);
		pObj->Free();
		m_BloodList.pop_back();
        m_Count--;
	}

    if (m_Count < 0)
    {
        m_Count = 0;
    }
}


bool BossBlood::IsEmpty()
{
    if (m_Count > 0)
    {
        return false;
    }
    return true;
}

void BossBlood::FreeData()
{
	if (IsEmpty() == false)
	{
		for (auto x : m_BloodList)
		{
			x->Free();
		}

		m_BloodList.clear();
	}
}
