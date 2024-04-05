#pragma once
#include "DefineGame.h"
#include "BaseObj.h"
#include "Geometric.h"
#include "TextObj.h"

class MenuGame
{
public:
    MenuGame(bool is_bkgn = true);
    ~MenuGame(void);

    enum MenuTyle
	{
		MENU_START = 200,
		MENU_PAUSE,
		MENU_END,
	};

	virtual void Render(SDL_Renderer* screen);
	virtual void MenuAction(SDL_Event events, SDL_Renderer* screen);
	virtual void LoadBkgn(const char* file, SDL_Renderer* screen);

	void Free();
	bool OnRect(int x, int y, SDL_Rect& rect);
	void SetIsBkgn(bool is_bkgn) { m_isDrawBkgn = is_bkgn; }
	void SetIsSelect(int idx) { m_Select = idx; };
	int GetSelect() { return m_Select; }

protected:
    int m_Type;
	BaseObj m_Background;
	bool m_isDrawBkgn;
	int m_Select;
};

///////////////////////////////////////////////////////////
class MenuGameStart : public MenuGame
{
public:
    MenuGameStart();
    ~MenuGameStart();
    void SetImgOptionList(const VT(BaseObj*)& img_list);
    void FreeData();
    void Render(SDL_Renderer* screen);
    void MenuAction(SDL_Event events, SDL_Renderer* screen);
    void InitOption(SDL_Renderer* screen);
private:
    VT(BaseObj*) m_ImgOptionList;
};

//////////////////////////////////////////////

class MenuGamePause : public MenuGame
{
public:
	MenuGamePause();
	~MenuGamePause();
	void SetImgOptionList(const VT(BaseObj*)& img_list);
	void FreeData();
	void Render(SDL_Renderer* screen);
	void MenuAction(SDL_Event events, SDL_Renderer* screen);
	void InitFrameGeo();
private:
	VT(BaseObj*) m_ImgOptionList;
	GeoObj m_Rect;
	GeoObj m_RectOutLine;
};

////////////////////////////////////////////////////////////

class MenuGameEnd : public MenuGame
{
public:

	enum EndLessonState
	{
		GA_NONE = 200,
		GA_WIN,
		GA_LOSE,
		GA_END_GAME
	};

	MenuGameEnd();
	~MenuGameEnd();

	void SetImgOptionList(const VT(BaseObj*)& img_list);
	void FreeData();
	void Render(SDL_Renderer* screen);
	void MenuAction(SDL_Event events, SDL_Renderer* screen);
	void SetTextContent(std::string sData, int idx);
	void AddTextObj(TextObj* obj) { m_TextList.push_back(obj); }
    void RemoveTextList();
	void SetFont(TTF_Font* font) { m_Font = font; }
	void SetGameState(Uint16 state) { m_State = state; }
	int GetGameState() {return m_State;}
private:
	VT(BaseObj*) m_ImgOptionList;
	VT(TextObj*) m_TextList;
	TTF_Font* m_Font;
	Uint16 m_State;
};