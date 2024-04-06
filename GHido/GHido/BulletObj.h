#pragma once
#include "DefineGame.h"
#include "BaseObj.h"
#include "MapData.h"
#include "TBase.h"

class BulletObj : public BaseObj
{
public:
    BulletObj(void);
    ~BulletObj(void);

    enum BulletDir
    {
        BD_RIGHT = 1000, 
        BD_LEFT,
    };

    bool Init(std::string path, SDL_Renderer* screen);
    void HandleMove(const int& xb1, const int& xb2);
    bool CheckToMap();
    void Show(SDL_Renderer* screen);
    SDL_Rect GetExpPos();

    BulletObj* Clone();
public:
    void set_x_Scope(const int& xScope) { x_scope_ = xScope; }
    void set_x_val(const int& x_val) { x_val_ = x_val; }
    void set_y_val(const int& y_val) { y_val_ = y_val; }
    void set_xy_pos(const int& xp, const int& yp) { x_pos_ = xp, y_pos_ = yp; }
    int get_x_pos() const { return x_pos_; }
    int get_y_pos() const { return y_pos_; }
    int get_x_val() const { return x_val_; }
    int get_y_val() const { return y_val_; }

    void set_is_move(const bool& is_move) { is_move_ = is_move; }
    bool get_is_move()const { return is_move_; }
    void set_dir_bullet(const int& dir_type) { bullet_dir_ = dir_type; }
    int get_dir_bullet() { return bullet_dir_; }
private:
    int x_pos_;
    int y_pos_;
    int x_val_;
    int y_val_;

    bool is_move_;
    int bullet_dir_;
    int x_scope_;
};

