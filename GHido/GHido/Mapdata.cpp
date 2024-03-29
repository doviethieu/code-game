
#include "stdafx.h"
#include "MapData.h"
#include "TBase.h"

BlockMap::BlockMap()
{
    m_tile = NULL;
    m_type = "";

    x_index_ = 0;
    y_index_ = 0;
    xp_map_ = 0;
    yp_map_ = 0;
}

BlockMap::~BlockMap()
{
    if (m_tile != NULL)
    {
        delete m_tile;
        m_tile = NULL;
    }
}

void BlockMap::Update()
{
    m_tile->SetRect(xp_map_, yp_map_);
}

void BlockMap::Render(SDL_Renderer* screen)
{
    if (m_tile && m_type != "")
    {
        m_tile->Render(screen);
    }
}

void BlockMap::RemoveTile()
{
    m_type = "";
    if (m_tile != NULL)
    {
        m_tile->Free();
        m_tile = NULL;
    }
}

void BlockMap::UpdateImage(SDL_Renderer* screen, std::string path)
{
    std::string sFile = path + std::string("\\") +  m_type +  ".png";
    m_tile->LoadImg(sFile, screen);
}

////////////////////////////////////////////////////////////////////

Map::Map()
{

}

Map::~Map()
{
    RemoveList();
}

void Map::UpdateYMapInfo(int yp)
{
    // tuong tu nhu vay voi y
    if (max_y_ < SCREEN_HEIGHT)
    {
        // Todo
    }
    else
    {
        int sub = yp - start_y_;
        int d4 = (int)(SCREEN_HEIGHT*0.5);
        int d6 = (int)(SCREEN_HEIGHT*0.6);
        if (sub < d4)
        {
            start_y_ = start_y_ - (d4 - sub);
        }
        else if (sub > d6)
        {
            start_y_ = start_y_ + (sub - d6);
        }

        if (start_y_ + SCREEN_HEIGHT >= max_y_)
        {
            start_y_ = max_y_ - SCREEN_HEIGHT;
        }
        else if (start_y_ < 0)
        {
            start_y_ = 0;
        }
    }
}

void Map::UpdateXMapInfo(int xp)
{
    if (max_x_ < SCREEN_WIDTH)
    {
        //Todo
    }
    else
    {
        // Khi nhan vat di chuyen tu vi tri dau tien, den gan giua man hinh
        // thi man hinh chua can phai cuon chieu
        // Bat dau nhan vat vuot qua 1/2 man hinh, thi bat dau map dc cuon chieu
        // Ex: xp = 2 + screen_width/2, map bat dau cuon chieu 1 luong = 2
        int d5 = (int)SCREEN_WIDTH*0.5;

        int sub = xp - start_x_;
        if (sub > d5)
        {
            start_x_ = start_x_ + (sub - d5);
        }
        else if (sub < 64)
        {
            start_x_ = start_x_ - (TILE_SIZE - sub);
        }

        if (start_x_ + SCREEN_WIDTH >= max_x_)
        {
            // khi nhan vat di chuyen den gan cuoi ban do
            // cung la luc start_x se tang dan.
            // khi start_x + screen_width bat dau >= full map
            // thi luc nay map ko can cuon chieu nua
            // start_x dat gioi han = max_x - width man hinh
            start_x_ = max_x_ - SCREEN_WIDTH;
        }
        else if (start_x_ < 0)
        {
            start_x_ = 0;
        }
    }
}

void Map::RemoveList()
{
        for (int i = 0; i < m_BlockList.size(); i++)
        {
            VT(BlockMap*) temp = m_BlockList[i];
            VT(BlockMap*)::iterator it;
            for (it = temp.begin(); it != temp.end(); it++)
            {
                delete (*it);
                *it = NULL;
            }

            temp.clear();
        }

        m_BlockList.clear();
}

///////////////////////////////////////////////////////////////////
GameMap* GameMap::instance_ = NULL;

GameMap::GameMap()
{
    game_map_ = new Map();
    m_path = "image\\map";
    map_num_h = 0;
    map_num_w = 0;
}

GameMap::~GameMap()
{
    
}

void GameMap::DestroyInst()
{
    if (game_map_ != NULL)
    {
        delete game_map_;
        game_map_ = NULL;
    }

    if (instance_ != NULL)
    {
        delete instance_;
        instance_ = NULL;
    }
}

std::string GameMap::GetPathMapName()
{
    return m_path;
}

void GameMap::LoadMap()
{
    std::string mapPath = GetPathMapName() + "\\hido_map.tmp";
    ReadMap(mapPath);
}

void GameMap::ReadMap(std::string file)
{
    const char *wName = file.c_str();
    std::fstream fsFile;
    fsFile.open(wName, std::ios::in);

    int rIndex = 0;
    int cIndex = 0;

    // File map là file dạng txt, với 400 cột và 10 hàng
    // thực hiện đọc dữ liệu file để tìm ra các mã tile.
    if (fsFile.is_open() == true)
    {
        bool bStartData = false;
        while (!fsFile.eof())
        {
            std::string strData;
            std::getline(fsFile, strData);
            if (bStartData == false)
            {
                int index = strData.find(TL_DATA);
                if (index >= 0)
                {
                    bStartData = true;
                    continue;
                }
            }
            else
            {
                if (strData.empty() == false)
                {
                    // mỗi dòng line tron file text sẽ có 400 mã tile
                    // dùng hàm SplitBySpace để tách ra từng mã
                    std::vector<std::string> tile_list = TBase::SplitBySpace(strData);
                    VT(BlockMap*) xTemp;
                    for (int i = 0; i < tile_list.size(); ++i)
                    {
                        // Với mỗi mã tìm được, sẽ tạo một đối tượng blockmap
                        // dại diện cho 1 ô tile map
                        std::string strTile = tile_list.at(i);
                        BlockMap* pBlock = new BlockMap();
                        int cx = rIndex*TILE_SIZE;
                        int cy = cIndex*TILE_SIZE;

                        // block map có tên layer, loại tile, vị trí hiển thị
                        pBlock->setType(strTile);
                        pBlock->setYIdx(cy);
                        pBlock->setXIdx(cx);
                        xTemp.push_back(pBlock);
                        rIndex++;
                    }


                    game_map_->AddList(xTemp);

                    map_num_w = rIndex;
                    rIndex = 0;
                    cIndex++;
                }
            }
        }
    }

    map_num_h = cIndex;


    // sau khi load toàn bộ số lượng tile map trong file map
    // lưu giá trị chiều dài, rộng toàn bộ bản đồ game
    game_map_->SetMaxX(map_num_w*TILE_SIZE);
    game_map_->SetMaxY(map_num_h*TILE_SIZE);

    // vi trí ban đầu của map với màn hình là 0, 0
    // khi nhân vật chạy, màn hình giữ nguyên, máp bị cuốn chiếu
    // giá trị start_x, start_y sẽ được tăng dần.
    game_map_->SetStartX(0);
    game_map_->SetStartY(0);
    fsFile.close();
    game_map_->SetFileMap(file);
}

//////////////////////////////////////////////////////////////////////////
// Sau khi đã đọc được mã tile của từng ô trong bản đồ map
// thì sử dụng mã đó tìm file ảnh có tên tương ứng với mã để load dữ liệu
// Hàm này thực hiện nhiệm vụ đó
//////////////////////////////////////////////////////////////////////////
void GameMap::LoadMapTiles(SDL_Renderer* screen)
{
    char* filename = NULL;
    VT(VT(BlockMap*)) dataMapList;
    dataMapList = game_map_->GetTile1();

    std::string path_img = GetPathMapName() + std::string("\\");
    int nRow = dataMapList.size();

    // Đọc theo từng hàng
    for (int y = 0; y < nRow; ++y)
    {

        VT(BlockMap*) dataMaps = dataMapList[y];
        int nCol = dataMaps.size();

        // đọc theo từng cột
        for (int x = 0; x < nCol; ++x)
        {
            BlockMap* pBlock = dataMaps[x];
            if (pBlock != NULL)
            {
                // tìm được mã tile
                std::string type = pBlock->getType();
                //  file ảnh  = mã tile + đuôi ảnh (png, bitmap..)
                std::string tile_path = path_img + type + ".png";
                filename = const_cast<char*>(tile_path.c_str());

                // mỗi tile ảnh là một baseobject
                BaseObj* pTile = new BaseObj();
                // load ảnh và lưu dữ liệu ảnh vào blockMap
                bool ret = pTile->LoadImg(filename, screen);
                if (ret)
                {
                    pBlock->setTile(pTile);
                }
            }
        }
    }
}

void GameMap::ResetMap(SDL_Renderer* screen)
{
    game_map_->RemoveList();
    LoadMap();
    LoadMapTiles(screen);
}

//////////////////////////////////////////////////////////////////////////
// Hàm vẽ map
// Khi dữ liệu lưu ảnh đã đầy đủ (400 cột, 10 hàng)
// mã tile đã có, dữ liệu load ảnh tile cũng đã xong
// thì bắt đầu vẽ map đó lên màn hình
//////////////////////////////////////////////////////////////////////////
void GameMap::DrawMap(SDL_Renderer* des)
{
    VT(VT(BlockMap*)) blockList;
    blockList = game_map_->GetTile1();
    if (blockList.empty() == true && map_num_w > 0 && map_num_h > 0)
    {
        return;
    }

    // Do bản đồ game kéo dài từ 0 đến 400*TILE_SIZE (25600)
    // Trong khi màn hình chỉ có chiều dài = 1280
    // Do đó chỉ số lượng ảnh tile có vị trí phù hợp trong phạm vi màn hình mới được hiển thị
    // Vi trí màn hình sẽ bắt dầu từ: start_x đến start_x + 1280
    // Do chiều cao = 10*64 = 640, vẫn nhỏ hơn chiều cao màn hình nên có thể hiện thị ful về chiều cao
    // Khi mới khởi động start_x = 0, do đó hiển thị từ 0 đến 1280
    // Nhưng khi nhân vật di chuyển đến giữa bản đồ, thì các tile đầu bản đồ ko thể hiển thị được nữa.
    // do đó cần giá trị start_x để biết được bản đồ đã bị cuốn chiều là bao nhiêu
    // ví dụ start_x = 256, nghĩa là, bản đồ đã bị kéo 256.
    // 256/ 64 = 4, như vậy có nghĩa là bản đồ phải hiển thị từ các tile ở cột 4 trở đi.
    // xử lý cả x,y cho tổng quát(vì có thể làm map game kiểu dọc)

    int start_x = game_map_->getStartX();  // tìm ra trị start_x, start_y
    int start_y = game_map_->getStartY();

    int map_x = start_x / TILE_SIZE;  // tìm ra số tile bắt đầu hiển thị
    int map_y = start_y / TILE_SIZE;

    // ko phải lúc nào start_x chia cho tile_size cũng ra tròn số
    // do đó map_x, map_y là 
    int x1 = (start_x % TILE_SIZE);
    int y1 = (start_y % TILE_SIZE);

    // tìm min giữa scren_width mà chiều dài bản đồ game
    // vì có thể loại game chiều dài game chưa hết màn hình

    int limitX = min(SCREEN_WIDTH, map_num_w*TILE_SIZE);
    int limitY = max(SCREEN_HEIGHT, map_num_h*TILE_SIZE);

    // Tọa độ vị trí của các ô tile map sẽ chạy theo giá trị từ 0 đến chiều rộng màn hình 
    for (int y_pos = 0; y_pos < limitY; y_pos += TILE_SIZE)
    {
        if (map_y >= 0 && map_y < blockList.size())
        {
            for (int x_pos = 0; x_pos <= limitX; x_pos += TILE_SIZE)
            {
                // với mỗi vòng lặp, x_pos đều tằng 1 lượng tile_size
                // do đó map_x sẽ được tăng lên 1
                if (map_x >= 0 && map_x <  blockList.at(map_y).size())
                {
                    // với mỗi giá trị map_x, map_y sẽ tìm tile tương ứng
                    BlockMap* pBlock = blockList.at(map_y).at(map_x);
                    // nếu block map tại vi trí đó có tile khác NULL (có hình ảnh)
                    if (pBlock != NULL && pBlock->GetTile() != NULL)
                    {
                        // thì thực hiện vẽ map tại vị trí x_pos, y_pos và trừ đi lượng số dư tính toán ở trên
                        pBlock->setXpMap(x_pos - x1);
                        pBlock->setYpMap(y_pos - y1);
                        pBlock->Update();
                        pBlock->Render(des);
                    }
                    map_x++;
                }
            }
        }
        
        // sau mỗi hàng, thì map_x lại được tính lại ban đầu
        map_x = game_map_->getStartX() / TILE_SIZE;

        // hàng chuyển sang kế tiếp
        map_y++;
    }
}

//////////////////////////////////////////////////////////////////////////
// kiểm tra tile map có phải đối tượng ngoại cảnh ko
//
//////////////////////////////////////////////////////////////////////////
bool GameMap::CheckSkipMap(const std::string& tile)
{
    bool bRet = false;
    //int nSize = sizeof(SkipMap) / sizeof(SkipMap[0]);
    //for (int i = 0; i < nSize; ++i)
    //{
    //    std::string sType = SkipMap[i];
    //    std::string sInput(tile);
    //    if (sInput == sType)
    //    {
    //        bRet = true;
    //        break;
    //    }
    //}

    return bRet;
}

// kiểm tra tile có phải tiền, kim cương hay đồ ăn mà bạn tự định nghĩa ra
int GameMap::GetValueProduct(const std::string& tile)
{
    int val = 0;
    if (tile == "CO1")
    {
        val = 1;
    }
    return val;
}