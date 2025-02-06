#include "button.h"

#include <graphics.h>

#pragma comment(lib,"WINMM.lib")
#pragma comment(lib,"MSIMG32.lib")


IMAGE img_menu;
IMAGE img_mole;
IMAGE img_empty;
IMAGE img_hammer_idle;
IMAGE img_hammer_down;

bool is_quit = false;
bool is_start = false;

const POINT offset_map = { 32,75 };	// ��ͼƫ������

bool map[4][4] =
{
	{false,false,false,false},
	{false,false,false,false},
	{false,false,false,false},
	{false,false,false,false},
};									// ��Ϸ��ͼ
int idx_x, idx_y;					// ��ǰ��������

bool is_hammer_down = false;
POINT pos_hammer = { 0,0 };

int score = 0;

Button btn_play(L"�� ʼ �� Ϸ", { 300,450,500,500 });
Button btn_info(L"�� Ϸ �� ��", { 300,550,500,600 });
Button btn_quit(L"�� �� �� Ϸ", { 300,650,500,700 });

void my_putimage(int x,int y,IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void load_resources()
{
	loadimage(&img_menu, _T("resources/menu.jpg"));
	loadimage(&img_mole, _T("resources/mole.jpg"));
	loadimage(&img_empty, _T("resources/empty.jpg"));
	loadimage(&img_hammer_idle, _T("resources/hammer_idle.png"));
	loadimage(&img_hammer_down, _T("resources/hammer_down.png"));

	mciSendString(_T("open resources/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open resources/hit.mp3 alias hit"), NULL, 0, NULL);
}

void input_menu_scene(const ExMessage& msg)
{
	btn_play.on_input(msg);
	btn_info.on_input(msg);
	btn_quit.on_input(msg);
}

void input_game_scene(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_MOUSEMOVE:
		pos_hammer.x = msg.x;
		pos_hammer.y = msg.y;
		break;
	case WM_LBUTTONDOWN:
	{
		is_hammer_down = true;
		static const int width_img_mole = img_mole.getwidth();
		static const int height_img_mole = img_mole.getheight();
		if (map[idx_x][idx_y] &&
			(pos_hammer.x >= idx_x * width_img_mole + offset_map.x
				&& pos_hammer.x <= (idx_x + 1) * width_img_mole + offset_map.x
				&& pos_hammer.y >= idx_y * height_img_mole + offset_map.y
				&& pos_hammer.y <= (idx_y + 1) * height_img_mole + offset_map.y))
		{
			score += 10;
			map[idx_x][idx_y] = false;
			mciSendString(_T("play hit from 0"), NULL, 0, NULL);
		}
		else
		{
			score -= 10;
		}
	}
		break;
	case WM_LBUTTONUP:
		is_hammer_down = false;
		break;
	default:
		break;
	}
}

void render_menu_scene()
{
	putimage(0, 0, &img_menu);

	btn_play.on_render();
	btn_info.on_render();
	btn_quit.on_render();
}

void render_game_scene()
{
	// ������Ϸ��ͼ
	static const int width_img_mole = img_mole.getwidth();
	static const int height_img_mole = img_mole.getheight();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// �����ǰλ���е��� �������ͼ
			if (map[i][j])
				putimage(i * width_img_mole + offset_map.x, j * height_img_mole + offset_map.y, &img_mole);
			else
				putimage(i * width_img_mole + offset_map.x, j * height_img_mole + offset_map.y, &img_empty);
		}
	}

	// ���ƴ���
	static const int width_img_hammer_idle = img_hammer_idle.getwidth();
	static const int height_img_hammer_idle = img_hammer_idle.getheight();
	static const int width_img_hammer_down = img_hammer_down.getwidth();
	static const int height_img_hammer_down = img_hammer_down.getheight();
	if (!is_hammer_down)
		my_putimage(pos_hammer.x - width_img_hammer_idle / 2, 
			pos_hammer.y - height_img_hammer_idle / 2, &img_hammer_idle);
	else
		my_putimage(pos_hammer.x - width_img_hammer_down / 2, 
			pos_hammer.y - height_img_hammer_down / 2, &img_hammer_down);

	// ���Ƶ÷�
	std::wstring str_score = L"��ҵ÷֣�" + std::to_wstring(score);
	settextstyle(25, 0, _T("����"));
	settextcolor(RGB(255, 15, 105));
	outtextxy(25, 25, str_score.c_str());
}

void update_game_scene()
{
	static int timer = 0;

	// ���Ӷ�ʱ��ʱ��
	timer = ++timer % 100;

	// �������ʱ�䣬ˢ�µ�ͼ�ϵ���λ��
	if (timer == 0)
	{
		map[idx_x][idx_y] = false;
		idx_x = rand() % 4, idx_y = rand() % 4;
		map[idx_x][idx_y] = true;
	}
}

int main(int argc, char** argv)
{
	HWND hwnd = initgraph(800, 950);
	SetWindowText(hwnd, _T("AAA - EasyX ����� "));

	load_resources();

	btn_play.set_on_click([&]()
		{
			is_start = true;
			mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
		});

	btn_info.set_on_click([&]()
		{
			MessageBox(hwnd, _T("��ͼ�л������ˢ�µĵ���\n����������û�����÷�"), _T("�淨����"), MB_OK);
		});

	btn_quit.set_on_click([&]()
		{
			is_quit = true;
		});

	BeginBatchDraw();

	while (!is_quit)
	{
		ExMessage msg;
		while (peekmessage(&msg))
		{
			if (!is_start)
				input_menu_scene(msg);
			else
				input_game_scene(msg);
		}
		if (is_start)
			update_game_scene();

		setbkcolor(RGB(251,251,251));
		cleardevice();

		if (!is_start)
			render_menu_scene();
		else
			render_game_scene();

		FlushBatchDraw();

		Sleep(5);
	}

	EndBatchDraw();

	return 0;
}