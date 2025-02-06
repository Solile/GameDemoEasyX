#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>
#include <functional>
#include <graphics.h>

class Button
{
public:
	Button(const std::wstring& text, const RECT& rect)
	{
		this->text = text;
		this->rect = rect;
	}

	~Button() = default;

	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			is_hovered = msg.x >= rect.left && msg.x <= rect.right
				&& msg.y >= rect.top && msg.y <= rect.bottom;
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (is_hovered)
				is_pushed = true;
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			if (is_pushed && on_click)
				on_click();
			is_pushed = false;
			break;
		default:
			break;
		}
	}

	void on_render()
	{
		setlinecolor(RGB(20, 20, 20));
		if (is_pushed)
			setfillcolor(RGB(185, 185, 185));
		else if (is_hovered)
			setfillcolor(RGB(235, 235, 235));
		else
			setfillcolor(RGB(205, 205, 205));

		fillrectangle(rect.left, rect.top, rect.right, rect.bottom);

		setbkmode(TRANSPARENT);
		settextcolor(RGB(20, 20, 20));
		drawtext(text.c_str(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	void set_on_click(std::function<void()> on_click)
	{
		this->on_click = on_click;
	}

private:
	RECT rect;
	std::wstring text;
	bool is_pushed = false;
	bool is_hovered = false;
	std::function<void()> on_click;

};

#endif // !_BUTTON_H_