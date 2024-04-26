# Chiến Binh HiDo 
•	Thử nghiệm game:  link youtube 
https://youtu.be/10TfWaQGUz0

•	Chiến thắng game:  link yotube
https://youtu.be/Vd4k6Wq5KQc


Giới Thiệu Game:
Chiến binh HiDo là tựa game thuộc thể loại nhập vai. Hido là một chiến binh gan dạ và dũng cảm. Một ngày nọ nghe tin vương quốc của mình bị một con rồng Qủy Long tấn công và bắt đi công chúa, HiDo đã vội vã lên đường, tiến thằng đến sào huyệt của Qủy Long để giải cứu nàng.
1.	Thông tin game
a.	Tải bản chơi game (bản release)
b.	Tải project game (source code)

2.	Nội dung, Thành phần cơ bản của game.
3.	Cách chơi Game.
4.	Game Win và Game Over
5.	Thông tin về Project Game
6.	Thông tin về tài nguyên game



# 1.	Thông tin về game
a.	Click vào link sau để tải bản exe về để có thể chơi game
https://drive.google.com/file/d/10TeIVfx3R1635hsGdU7BjEZjWY6OhjrS/view?usp=sharing

b.Click vào link sau để tải source code game về chạy thử và nghiên cứu 
https://github.com/doviethieu/code-game/tree/main
+ Sau khi tải về hãy giải nén 
+ Yêu cầu máy tính phải cài visual studio phiên bản 2012 trở lên
+ Build project ở cấu hình 32 bit, mode debug hoặc release
+ Sau khi build thành công thì hãy copy toàn bộ nội dung folder dưới đây  (res_game) vào thư mục debug hoặc release

[Trong project, copy đống dll của thư viện sdl, copy folder image, sound, font ,  ra một folder bên ngoài ví dụ canh fodler lib_sdl, đặt tên là res_game,  sau đó add nó vào git và đây lên github, sau đó chụp ảnh project có folder res_game, và chỉ mũi tên vào đó]
Ví dụ :
 


+ F5 project trên visual để chạy, hoặc cũng có thể trực tiếp từ file exe được build ra.
 

# 2.	Nội dung về game
a.	Màn hình start game
[em chạy game, chụp ảnh màn hình start game vào paste vào đây nhé]

Khi bắt đầu chơi game, màn hình start game được hiển thị, gồm background và 2 button. Play – Exit
b.	Chương trình game
+ Nhân vật player có tên là chiến binh HiDo
  
+ Bối cảnh game: Bản đồ game thể loại tile map
 [chụp ảnh game đang chơi cho vào đây nhé]

+ Hoạt cảnh game: Như đồng tiền (coin)

 
 

+ Quái vật, Qụa vàng cản trở chiến binh Hido 
     

+ Boom, cản trở gây sát thương chiến binh Hido
 


+ BOSS Game: Là 1 con Qủy Long, to lớn, màu xanh và có thể phun lửa.

 
+ Công chúa: Mục tiêu giải cứu của chiến binh Hido
 

c.	Màn hình Pause Game:
Có 2 nut resume và exit
[em chụp màn hình pause game rồi cho vào đây nhé]

d.	Màn hình EndGame
Có 2 màn hình End Game là End Game Win và End Game Over
[em chụp 2 màn hình End Game rồi cho vào đây nhé]

# 3.	Cách chơi Game

Chạy game, và ấn vào nút start
Chương trình game bắt đầu hoạt động.
Di chuyển nhân vật game bằng các nut di chuyển trên bàn phím, trái phải lên xuống
Sử dụng chuột phải để nhảy. và chuột trái để bắn đạn

Hay di chuyển trên map để không bị rơi xuống biển, hoặc vực thẳm.
Mỗi lần rơi sẽ bị mất một mạng
Sử dụng súng để bắn đạn tiêu diệt kẻ địch.
Tránh va chạm với kẻ địch như con quạ vàng, quả bom hay quái vật
Tránh dính phải đạn bắn của con quái vật.

Hãy ăn các đồng tiền, để đạt được 100 đồng, thì sẽ được thêm 1 mạng
Khi về đến gần cuối bài sẽ va chạm với con Qủy Long.
Qủy Long rất mạnh, có thể phun lửa với tốc độ rất nhanh và có đến 10 cột máu
[em thich bổ sung hình ảnh gì thì cho vào đây nhé]
Phải bắn trúng quỷ long 4 viên đạn, thì mới làm cho quỷ Long mất đi 1 giọt máu.
Khi nào Qủy Long gần chết, sẽ có dấu hiệu đó là, từ màu xanh, Qủy Long sẽ hóa dần sang màu đỏ cam.
Hãy chú ý thời gian đang đếm ngược, hết thời gian mà HiDo chưa hạ được Qủy Long và đến gặp công chúa, Game sẽ coi là thua.

# 4.	Game Win và Game Over
a.	Game Win: 
Khi bạn vượt qua được được Qủy Long, và đến được sát gần công chúa, thì được coi là bạn đã chiến thắng.
b.	Game Over:
Khi bạn hết mạng, hoặc khi thời gian kết thúc mà bạn chưa đến được vị trí công chúa thì coi như bạn đã thua.
# 5.	Thông tin về project Game
Ngôn ngữ lập trình: C++
Thư viện mã nguồn mở: SDL 2.0
Công cụ lập trình (IDE và trình biên dịch)  Visual studio 2012
Kỹ thuật lập trình: Sử dụng kỹ thuật lập trình hướng đối tượng.
Mode Build: Debug-win32 và Release –win 32

Thành phần source:
a.	File main.cpp
Là file viết hàm chính của ngôn ngữ lập trình c++ 
Hàm int main()
Hàm này sẽ gọi đến toàn bộ các hoạt động của game thông qua lớp GameMain.

b.	GameMain.h/cpp là một lớp đối tượng đại diện cho toàn bộ hoạt động của 1 chương trình game.
Vì chương trình game chỉ có 1, nên GameMain chỉ cần tạo một đối tượng duy nhất xuyên suốt toàn bộ quá trình.
Do đó sử dụng kỹ thuật design pattern: SingleTon để tạo ra một object duy nhất đại diện cho một chương trình game
GameMain có 4 hoạt động chính.
+ Hàm Init() : Khởi tạo môi trường thư viện SDL và tạo window, màn hình game, và các thuộc tính khác như, hiển thị văn bản, âm thanh
+ Hàm InitData(): Khởi tạo dữ liệu cho các đối tượng game
+ Hàm Loop: Thực hiện hiển thị các hình ảnh game, và xử lý toàn bộ các hoạt động của đối tượng game
.	Hiển thị bản đồ
.	Hiển thị player, monster, coin, boss….
.	Hiển thị text
.	Xử lý va chạm, xử lý bắn đạn, xử lý di chuyển…
+ Hàm Close():  Giải phóng vùng nhớ cho toàn bộ các đối tượng game, khi game kết thúc
 

c.	Sơ đồ mô tả kiến trúc chương trình Game như sau

 

d.	Các đối tượng game
Từ sơ đồ trên, cho thấy một chương trình game có rất nhiều đối tượng game.
Và do đó mỗi đối tượng game sẽ được mô hình hóa bởi một lớp dữ liệu.
Nhóm 1: Đối tượng game có hình ảnh, do đó sẽ có 1 lớp cha gọi là BaseObject.
Các lớp con sẽ là: 
+ Lớp xử lý Map (MapData, BlockMap, GameMap)
+ Lớp PlayerObj
+ Lớp TObj (ThreatsObj), trong code đặt là MonsterObj
+ Lớp BulletObj (đạn bắn)
+ Lớp BossObj
+ Lớp ExpObj (Vụ nổ)
+ Lớp Support (PlayerCoin, PlayerBlood..)
+ Lớp MenuGame.

Nhóm 2: Nhóm ko liên quan đến hình ảnh.
Lớp Music, Lớp TextObj, Geometric…
Lớp xử lý thời gian cân bằng game (ImpTime, kỹ thuật fps)

# 6.	Thông tin về tài nguyên game
Mọi hình ảnh game đều được lấy từ 2 nguồn sau
+ https://www.freepik.com/
+ https://craftpix.net/

Sử dụng kỹ thuật thiết kế hình ảnh game bằng Photoshop

Nguồn audio và font chữ:
Kiếm từ các trang web trên mạng

                                     END
