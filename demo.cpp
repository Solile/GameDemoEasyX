#include <graphics.h>
#include <string>
#include <vector>

const double PI = 3.1415926535;

int idx_current_pikaQ = 0;

const int PLAYER_pikaQ_NUM = 6;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

#pragma comment(lib,"Winmm.lib")	// music lib
#pragma comment(lib,"MSIMG32.LIB")

bool running = true;
bool is_game_started = false;

inline void putimage_alpha(int x, int y, IMAGE* img);

// 动画所用图集
class Atlas
{
public:
	Atlas(LPCTSTR path, int num)
	{
		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path, i);
			IMAGE* frame = new IMAGE();
			loadimage(frame, path_file);
			frame_list.push_back(frame);
		}
	}
	~Atlas()
	{
		for (size_t i = 0; i < frame_list.size(); i++)
		{
			delete frame_list[i];
		}
	}

public:
	std::vector<IMAGE*> frame_list;
};

Atlas* atlas_palyer_left;
Atlas* atlas_palyer_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;

class pikaQ
{
public:
	pikaQ(Atlas* atlas, int interval)
	{
		anim_atlas = atlas;
		interval_ms = interval;
	}
	~pikaQ() = default;	// 不可析构函数

	void play(int x, int y, int delta)
	{
		timer += delta;
		if (timer >= interval_ms)
		{
			idx_frame = (idx_frame + 1) % anim_atlas->frame_list.size();
			timer = 0;
		}
		putimage_alpha(x, y, anim_atlas->frame_list[idx_frame]);
	}
private:
	int timer = 0;		// 动画计时器
	int idx_frame = 0;	// 动画帧索引
	int interval_ms = 0;

private:
	Atlas* anim_atlas;
};

class Player
{
public:
	const int FRAME_WIDTH = 80;
	const int FRAME_HEIGHT = 80;

public:
	Player()
	{
		loadimage(&img_shadow, _T("img/shadow_player.png"));
		pika_left = new pikaQ(atlas_palyer_left, 45);
		pika_right = new pikaQ(atlas_palyer_right, 45);
	}
	~Player()
	{
		delete pika_left;
		delete pika_right;
	}
	const POINT& GetPosition() const
	{
		return position;
	}
	void ProcessEvent(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case VK_UP:
				is_move_up = true;
				break;
			case VK_DOWN:
				is_move_down = true;
				break;
			case VK_LEFT:
				is_move_left = true;
				break;
			case VK_RIGHT:
				is_move_right = true;
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case VK_UP:
				is_move_up = false;
				break;
			case VK_DOWN:
				is_move_down = false;
				break;
			case VK_LEFT:
				is_move_left = false;
				break;
			case VK_RIGHT:
				is_move_right = false;
				break;
			}
			break;
		}
	}
	void Move()
	{
		int dir_x = is_move_right - is_move_left;
		int dir_y = is_move_down - is_move_up;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0)
		{
			double normalized_x = dir_x / len_dir;
			double normalized_y = dir_y / len_dir;
			position.x += (int)(SPEED * normalized_x);
			position.y += (int)(SPEED * normalized_y);
		}

		if (position.x < 0) position.x = 0;
		if (position.y < 0) position.y = 0;
		if (position.x + FRAME_WIDTH > WINDOW_WIDTH) position.x = WINDOW_WIDTH - FRAME_WIDTH;
		if (position.y + FRAME_HEIGHT > WINDOW_HEIGHT) position.y = WINDOW_HEIGHT - FRAME_HEIGHT;
	}
	void Draw(int delta)
	{
		int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);
		int pos_shadow_y = position.y + FRAME_HEIGHT - 8;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

		static bool facing_left = true;
		int dir_x = is_move_right - is_move_left;
		if (dir_x < 0)
			facing_left = true;
		else if (dir_x > 0)
			facing_left = false;

		if (facing_left)
			pika_left->play(position.x, position.y, delta);
		else
			pika_right->play(position.x, position.y, delta);
	}

private:
	const int SPEED = 3;
	const int SHADOW_WIDTH = 32;

private:
	IMAGE img_shadow;
	pikaQ* pika_left;
	pikaQ* pika_right;
	POINT position = { 600,400 };
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

};

class Bullet
{
public:
	POINT position = { 0,0 };
public:
	Bullet() = default;
	~Bullet() = default;
	void Draw() const
	{
		setlinecolor(RGB(255, 155, 50));
		setfillcolor(RGB(200, 75, 10));
		fillcircle(position.x, position.y, RADIUS);
	}
private:
	const int RADIUS = 10;
};

class Enemy
{
public:
	Enemy()
	{
		loadimage(&img_shadow, _T("img/shadow_enemy.png"));
		enemy_left = new pikaQ(atlas_enemy_left, 45);
		enemy_right = new pikaQ(atlas_enemy_right, 45);

		// 敌人生成边界
		enum class SpawnEdge
		{
			Up=0,
			Down,
			Left,
			Right
		};

		// 将敌人放置在地图边界的随机位置
		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		switch (edge)
		{
		case SpawnEdge::Up:
			position.x = rand() % WINDOW_WIDTH;
			position.y = -FRAME_HEIGHT;
			break;
		case SpawnEdge::Down:
			position.x = rand() % WINDOW_WIDTH;
			position.y = WINDOW_HEIGHT;
			break;
		case SpawnEdge::Left:
			position.x = -FRAME_WIDTH;
			position.y = rand() % WINDOW_HEIGHT;
			break;
		case SpawnEdge::Right:
			position.x = WINDOW_WIDTH;
			position.y = rand() % WINDOW_HEIGHT;
			break;
		default:
			break;

		}
	}
	~Enemy()
	{
		delete enemy_left;
		delete enemy_right;
	}
	bool checkBulletCollsion(const Bullet& bullet)
	{
		bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + FRAME_WIDTH;
		bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + FRAME_HEIGHT;
		return is_overlap_x && is_overlap_y;
	}
	bool checkPlayerCollsion(const Player& player)
	{
		POINT check_position = { position.x + FRAME_WIDTH / 2, position.y + FRAME_HEIGHT / 2 };
		const POINT& player_postion = player.GetPosition();
		bool is_overlap_x = check_position.x >= player_postion.x && check_position.x <= player_postion.x + player.FRAME_WIDTH;
		bool is_overlap_y = check_position.y >= player_postion.y && check_position.y <= player_postion.y + player.FRAME_HEIGHT;
		return is_overlap_x && is_overlap_y;
	}
	void Move(const Player& player)
	{
		const POINT& player_postion = player.GetPosition();
		int dir_x = player_postion.x - position.x;
		int dir_y = player_postion.y - position.y;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0)
		{
			double normalized_x = dir_x / len_dir;
			double normalized_y = dir_y / len_dir;
			position.x += (int)(SPEED * normalized_x);
			position.y += (int)(SPEED * normalized_y);
		}
		if (dir_x < 0)
			facing_left = true;
		else if (dir_x > 0)
			facing_left = false;
	}
	void Draw(int delta)
	{
		int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);
		int pos_shadow_y = position.y + FRAME_HEIGHT - 25;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

		if (facing_left)
			enemy_left->play(position.x, position.y, delta);
		else
			enemy_right->play(position.x, position.y, delta);
	}
	void Hurt()
	{
		alive = false;
	}
	bool CheckAlive()
	{
		return alive;
	}
private:
	const int SPEED = 2;
	const int FRAME_WIDTH = 80;
	const int FRAME_HEIGHT = 80;
	const int SHADOW_WIDTH = 48;

private:
	IMAGE img_shadow;
	pikaQ* enemy_left;
	pikaQ* enemy_right;
	POINT position = { 0,0 };
	bool facing_left = false;
	bool alive = true;
};

class Button
{
public:
	Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_horered, LPCTSTR path_img_pushed)
	{
		region = rect;
		loadimage(&img_idle, path_img_idle);
		loadimage(&img_horered, path_img_horered);
		loadimage(&img_pushed, path_img_pushed);
	}
	~Button() = default;
	
	void ProcessEvent(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))
				status = Status::Horered;
			else if(status == Status::Horered && !CheckCursorHit(msg.x, msg.y))
				status = Status::Idle;
			break;
		case WM_LBUTTONDOWN:
			if(CheckCursorHit(msg.x, msg.y))
				status = Status::Pushed;
			break;
		case WM_LBUTTONUP:
			if (status == Status::Pushed)
				OnClick();
			break;
		default:
			break;
		}
	}

	void Draw()
	{
		switch (status)
		{
		case Status::Idle:
			putimage(region.left, region.top, &img_idle);
			break;
		case Status::Horered:
			putimage(region.left, region.top, &img_horered);
			break;
		case Status::Pushed:
			putimage(region.left, region.top, &img_pushed);
			break;
		}
	}
protected:
	virtual void OnClick() = 0;
private:
	enum Status
	{
		Idle = 0,
		Horered,
		Pushed
	};

private:
	RECT region;
	IMAGE img_idle;
	IMAGE img_horered;
	IMAGE img_pushed;
	Status status = Status::Idle;

private:
	// 检测鼠标点击
	bool CheckCursorHit(int x, int y)
	{
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}
};

class StartGameButton :public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_horered, LPCTSTR path_img_pushed) :
		Button(rect, path_img_idle, path_img_horered, path_img_pushed) {}
	~StartGameButton() = default;
protected:
	void OnClick()
	{
		is_game_started = true;
		mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
	}
};

class QuitGameButton :public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_horered, LPCTSTR path_img_pushed) :
		Button(rect, path_img_idle, path_img_horered, path_img_pushed) {}
	~QuitGameButton() = default;
protected:
	void OnClick()
	{
		running = false;
	}
};

// 加载透明度
void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
// 生成敌人
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list)
{
	const int INTERBAL = 100;
	static int counter = 0;
	if (++counter % INTERBAL == 0) {
		enemy_list.push_back(new Enemy());
	}
}
// 实时更新子弹
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player, int score)
{
	const double RADIAL_SPEED = 0.0045;						// 径向波动速度
	const double TANGENT_SPEED = 0.0055;					// 切向波动速度
	double radian_interval = 2 * PI / bullet_list.size();	// 子弹之间的弧度间隔
	const POINT& player_postion = player.GetPosition();
	double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
	
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;	//当前子弹所在的弧度值
		bullet_list[i].position.x = player_postion.x + player.FRAME_WIDTH / 2 + (int)(radius * sin(radian));
		bullet_list[i].position.y = player_postion.y + player.FRAME_HEIGHT / 2 + (int)(radius * cos(radian));
	}
}
// 绘制玩家得分
void DrawPlayerScore(int score)
{
	static TCHAR text[64];
	_stprintf_s(text, _T("当前玩家得分：%d"), score);

	//setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(10, 10, text);
}
// 显示玩家当前位置
void DrawPlayerPosition(const Player& player)
{
	const POINT& player_postion = player.GetPosition();
	static TCHAR text[64];
	_stprintf_s(text, _T("当前玩家位置：(%d,%d)"), player_postion.x, player_postion.y);

	settextcolor(RGB(0, 0, 0));
	outtextxy(10, 40, text);
}
// 主动显示向右的图片
void addRightpic()
{
	IMAGE img_player_left[6];
	IMAGE img_player_right[6];
	for (int i = 0; i < 6; i++)
	{
		int width = img_player_left[i].getwidth();
		int height = img_player_left[i].getheight();
		Resize(&img_player_right[i], width, height);

		DWORD* color_buffer_left_img = GetImageBuffer(&img_player_left[i]);
		DWORD* color_buffer_right_img = GetImageBuffer(&img_player_right[i]);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int idx_left_img = y * width + x;
				int idx_right_img = y * width + (width - x - 1);
				color_buffer_right_img[idx_right_img] = color_buffer_left_img[idx_left_img];
			}
		}
	}
}
// 白色闪光特效
void WhiteLeftSketch()
{
	IMAGE img_player_left[6];
	IMAGE img_player_left_sketch[6];
	for (int i = 0; i < 6; i++)
	{
		int width = img_player_left[i].getwidth();
		int height = img_player_left[i].getheight();
		Resize(&img_player_left_sketch[i], width, height);

		DWORD* color_buffer_raw_img = GetImageBuffer(&img_player_left[i]);
		DWORD* color_buffer_sketch_img = GetImageBuffer(&img_player_left_sketch[i]);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int idx = y * width + x;
				if ((color_buffer_raw_img[idx] & 0xFF000000) >> 24)
				{
					color_buffer_sketch_img[idx] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);
				}
			}
		}
	}
}
// 冰冻特效
void RenderFrozenPlayer()
{
	IMAGE img_player_left[6];
	IMAGE img_shadow;
	IMAGE img_ice;

	static const POINT position = { 1075,345 };

	static int counter = 0;				// 动画帧索引
	static int anim_timer = 0;			// 动画计时器
	static int frozen_timer = 0;		// 冷冻状态计时器
	static const int THICKNESS = 5;		// 扫描线宽度
	static int highlight_pose_y = 0;	// 扫描线竖直坐标
	static bool is_frozen = false;		// 当前是否在冰冻

	if ((!is_frozen) && (++anim_timer % 3 == 0))
		counter = (counter + 1) % 6;
	if (++frozen_timer % 100 == 0)
	{
		is_frozen = !is_frozen;
		highlight_pose_y = -THICKNESS;
	}
	putimage_alpha(position.x + (80 - 32) / 2, position.y + 80, &img_shadow);
	if (is_frozen)
	{
		IMAGE img_current_frame(img_player_left[counter]);
		int width = img_current_frame.getwidth();
		int height = img_current_frame.getheight();

		highlight_pose_y = (highlight_pose_y + 2) % height;

		DWORD* color_buffer_ice_img = GetImageBuffer(&img_ice);
		DWORD* color_buffer_frame_img = GetImageBuffer(&img_current_frame);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int idx = y * width + x;
				static const float RATIO = 0.25f;
				static const float THRESHOLD = 0.84f;
				DWORD color_ice_img = color_buffer_ice_img[idx];
				DWORD color_frame_img = color_buffer_frame_img[idx];
				if ((color_frame_img & 0xFF000000) >> 24)
				{
					BYTE r = (BYTE)(GetBValue(color_frame_img) * RATIO + GetBValue(color_ice_img) * (1 - RATIO));
					BYTE g = (BYTE)(GetGValue(color_frame_img) * RATIO + GetGValue(color_ice_img) * (1 - RATIO));
					BYTE b = (BYTE)(GetRValue(color_frame_img) * RATIO + GetRValue(color_ice_img) * (1 - RATIO));
					if ((y > highlight_pose_y && y < highlight_pose_y + THICKNESS) &&
						((r / 255.0f) * 0.2126f + (g / 255.0f) * 0.7152f + (b / 255.0f) * 0.0722 >= THRESHOLD))
					{
						color_buffer_frame_img[idx] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);
						continue;
					}
					color_buffer_frame_img[idx] = BGR(RGB(r, g, b)) | (((DWORD)(BYTE)(255)) << 24);
				}
			}
		}
		putimage_alpha(position.x, position.y, &img_current_frame);
	}
	else
		putimage_alpha(position.x, position.y, &img_player_left[counter]);

}

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);

	atlas_palyer_left = new Atlas(_T("img/player_left_%d.png"), 6);
	atlas_palyer_right = new Atlas(_T("img/player_right_%d.png"), 6);
	atlas_enemy_left = new Atlas(_T("img/enemy_left_%d.png"), 6);
	atlas_enemy_right = new Atlas(_T("img/enemy_right_%d.png"), 6);

	// 加载音乐
	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);
	//mciSendString(_T("setaudio bgm volume to 10"), NULL, 0, NULL);

	int score = 0;
	Player player;
	ExMessage msg;
	IMAGE img_menu;
	IMAGE img_background;
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);

	RECT region_btn_start_game, region_btn_quit_game;

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	StartGameButton btn_start_game = StartGameButton(region_btn_start_game,
		_T("img/ui_start_idle.png"), _T("img/ui_start_hovered.png"), _T("img/ui_start_pushed.png"));
	QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game,
		_T("img/ui_quit_idle.png"), _T("img/ui_quit_hovered.png"), _T("img/ui_quit_pushed.png"));

	loadimage(&img_menu, _T("img/menu.png"));
	loadimage(&img_background, _T("castleInAir.jpg"));

	setbkcolor(RGB(245,245,245));

	BeginBatchDraw();

	while (running)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			if (is_game_started) 
				player.ProcessEvent(msg);
			else
			{
				btn_start_game.ProcessEvent(msg);
				btn_quit_game.ProcessEvent(msg);
			}
		}
		
		if (is_game_started)
		{
			TryGenerateEnemy(enemy_list);
			player.Move();
			if (score >= 80)
				bullet_list.resize(7);
			else if (score >= 40)
				bullet_list.resize(6);
			else if (score >= 20)
				bullet_list.resize(5);
			else if (score >= 10)
				bullet_list.resize(4);

			UpdateBullets(bullet_list, player, score);

			for (Enemy* enemy : enemy_list)
				enemy->Move(player);

			// 检测敌人与玩家的碰撞
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->checkPlayerCollsion(player))
				{
					static TCHAR text[128];
					_stprintf_s(text, _T("最终得分：%d"), score);
					MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);

					running = false;
					break;
				}
			}
			// 检测子弹与敌人的碰撞
			for (Enemy* enemy : enemy_list)
			{
				for (const Bullet& bullet : bullet_list)
				{
					if (enemy->checkBulletCollsion(bullet))
					{
						mciSendString(_T("play hit from 0"), NULL, 0, NULL);
						enemy->Hurt();
						score++;
					}
				}
			}
			// 移除生命值归零的敌人
			for (size_t i = 0; i < enemy_list.size(); i++)
			{
				Enemy* enemy = enemy_list[i];
				if (!enemy->CheckAlive())
				{
					std::swap(enemy_list[i], enemy_list.back());
					enemy_list.pop_back();
					delete enemy;
				}
			}
		}
		//static int counter = 0;
		//if (++counter % 5 == 0)
		//	idx_current_pikaQ++;

		//// 使动画循环播放
		//idx_current_pikaQ = idx_current_pikaQ % PLAYER_pikaQ_NUM;

		cleardevice();

		if (is_game_started)
		{
			putimage(0, 0, &img_background);
			player.Draw(1000 / 144);
			for (Enemy* enemy : enemy_list)
				enemy->Draw(1000 / 144);
			for (const Bullet& bullet : bullet_list)
				bullet.Draw();
			DrawPlayerScore(score);
			DrawPlayerPosition(player);
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game.Draw();
			btn_quit_game.Draw();
		}

		FlushBatchDraw();

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / 144)
		{
			Sleep(1000 / 144 - delta_time);
		}
	}

	delete atlas_palyer_left;
	delete atlas_palyer_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;

	EndBatchDraw();
	return 0;
}

