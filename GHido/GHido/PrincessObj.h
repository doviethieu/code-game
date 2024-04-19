#pragma once

#include "DefineGame.h"
#include "BaseObj.h"
#include "MapData.h"

class PrincessObj : public BaseObj
{
public:
    PrincessObj(void);
    ~PrincessObj(void);
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void DoAction(SDL_Renderer* des);
    void FreeFalling();
    BlockMap* GetBlockMap(int y, int x);
    int CheckBlock(BlockMap* block);
    void Show(SDL_Renderer* des);
    void Set_Pos(int xp, int yp) {x_pos_ = xp; y_pos_ = yp;}
private:
    
    float x_pos_;
    float y_pos_;
    float y_val_;;
    bool on_ground_;
};

