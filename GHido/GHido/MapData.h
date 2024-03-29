
#ifndef MAP_DATA_H_
#define MAP_DATA_H_

#include "DefineGame.h"
#include "BaseObj.h"

class BlockMap
{
public:
    BlockMap();
    ~BlockMap();

    void Update();
    void Render(SDL_Renderer* screen);
    void UpdateImage(SDL_Renderer* screen, std::string path);
    void RemoveTile();

    std::string getType() { return m_type; }
    BaseObj* GetTile() const { return m_tile; }

    int getXIdx() { return x_index_; }
    int getYIdx() { return y_index_; }
    int getXpMap() { return xp_map_; }
    int getYpMap() { return yp_map_; }

    void setTile(BaseObj* pTile) { m_tile = pTile; };
    void setXIdx(int x) { x_index_ = x; }
    void setYIdx(int y) { y_index_ = y; }
    void setType(std::string type) { m_type = type; }
    void setXpMap(int xp) { xp_map_ = xp; }
    void setYpMap(int yp) { yp_map_ = yp; }

private:
    std::string m_type; // 1 ô map sẽ có mã tile đọc từ file, ví dụ G1, TR1, BRICK1, COIN
    BaseObj* m_tile; // dữ liệu ảnh của ô map đó
    int x_index_;       // index của ô map, ví dụ là ô ở dòng 20, cột 10
    int y_index_;
    int xp_map_; // vị trí của ô map
    int yp_map_;
};


///////////////////////////////////////////////////////////////////
class Map
{
public:
    Map();
    ~Map();

    int getStartX() const { return start_x_; }
    int getStartY() const { return start_y_; }
    int getMaxX() const { return max_x_; }
    int getMaxY() const { return max_y_; }

    VT(VT(BlockMap*)) GetTile1() { return m_BlockList; }

    void SetStartX(int xs) { start_x_ = xs; }
    void SetStartY(int ys) { start_y_ = ys; }
    void SetMaxX(int mx) { max_x_ = mx; }
    void SetMaxY(int my) { max_y_ = my; }
    void UpdateXMapInfo(int xp);
    void UpdateYMapInfo(int yp);
    void SetFileMap(std::string filemap) { file_name_ = filemap; }
    void AddList(VT(BlockMap*) list) { m_BlockList.push_back(list); }
    void RemoveList();
private:
    int start_x_;  // vị trí start_x của map
    int start_y_;
    int max_x_;  // tổng chiều dài map
    int max_y_;
    std::string file_name_;
    VT(VT(BlockMap*)) m_BlockList;
};


////////////////////////////////////////////////////////

static const char TL_DATA[] = { "data" };

class GameMap
{
public:
  GameMap();
  ~GameMap();

  static GameMap* GetInstance()
  {
      if (instance_ == NULL)
          instance_ = new GameMap();
      return instance_;
  }

  void DestroyInst(); // giải phóng đối tượng map
  void SetPath(std::string path) { m_path = path; }

  // hàm loadMap
  void LoadMap();
  void ReadMap(std::string file); // dọc dữ liệu từ file map theo layer

  void DrawMap(SDL_Renderer* des);  // vẽ toàn bộ bản đồ map
  void LoadMapTiles(SDL_Renderer* screen); // đọc các file ảnh cho từng ô tile, dữ liệu đó sẽ dùng trong hàm DraMap

  void SetMap(Map* gMap) {game_map_ = gMap;}
  void ResetMap(SDL_Renderer* screen); // xóa dữ liệu máp

  Map* GetMap() const { return game_map_; }
  std::string GetPathMapName();

  int GetHNum() { return map_num_h; }
  int GetWNum() { return  map_num_w; }

  bool CheckSkipMap(const std::string& tile);  // kiểm tra ô tile có phải đối tượng bỏ qua hay ko
  int GetValueProduct(const std::string& tile); // kiểm tra ô tile có phải coin, kim cương hay ko và trả về giá trị của nó
public:
  Map* game_map_;   // đối đại diện cho danh sách các khối map game
private:
    static GameMap* instance_;
    int map_num_h;  // số lượng ô tile map chiều dọc
    int map_num_w;  // số lượng ô til map chiều ngang
    std::string m_path;  // tên bản đồ map
};

#endif