#ifndef __MENU_H__
#define __MENU_H__

#include <Windows.h>
#include <string>
#include "Draw.h"
#include "input.h"

using namespace std;

#define GetSizeOf(a)			sizeof(a)/sizeof(*a)


class Menu
{
public:
	Menu();


	struct Cheats
	{
		Cheats() { }

		struct Esp_s
		{
			int Box, Line, Char;
		} Esp;

		struct Aimbot_s
		{
			int on, FovOn, Trigger, TriggerSniper, TrigFov, spawn;
		} Aimbot;

		struct Misc_s
		{
			int Recoil;
		} Misc;

		struct MenuSettings_s
		{
			int darkMode;
		} MenuSettings;
	};
	void AddTab(string name);
	void AddItem(string name, string* display, int max = 2, int value = 0);
	void Header(string title) { menuInfo.title = title; }
	void Render(HWND gwnd);
	bool IsShown() { return menuInfo.isShown; }

	struct MenuItems {
		string name;
		string* display;
		int value; 
		int max;
	} items[100];

private:
	void MouseSetup();
	void MouseNavigation(int itemIndex);
	void MouseScrollNavigation(float x, float y, float w, float h);
	void KeyboardNavigation();

	void DrawSelection(int x, int y, int w);
	void DrawTabs(int x, int y, int w, int h);
	void DrawItems(int x, int y, int w);
	void DrawScrollBar(int x, int y, int h);
	void DrawColors(int x, int y, int w, int h);
	void DragMenu(int x, int y, int w, int h);
	
	struct CursorInfo { POINT pos; POINT offset; bool isLClicked; bool isRClicked; bool isHolding; } c;
	struct MenuInfo { string title; int x; int y; int w; D3DCOLOR color; bool isDragging; bool isShown; } menuInfo;
	struct ItemInfo { int num; int index; } itemInfo, tabInfo, scrollInfo, colorInfo;

	struct MenuTab { string name; int pos; } tabs[8];

	// atualiza �ndice do item e scroll
	void Update() { itemInfo.index = 0; scrollInfo.index = 0; }

	// itens e abas
	int FirstTabIndex() { return tabs[tabInfo.index].pos; }	// primeiro �ndice da aba
	int LastTabIndex() { if (tabInfo.num > tabInfo.index + 1) return tabs[tabInfo.index + 1].pos - 1; else return itemInfo.num - 1; }	// �ltimo �ndice da aba
	int NumTabItems() { int num = 0; for (int i = this->FirstTabIndex(); i <= this->LastTabIndex(); i++) num++; return num; } // quantidade de itens na aba

	// scroll
	bool HasScroll() { return (this->NumTabItems() > scrollInfo.num); }	// aba atual possui scroll
	int NumScrollItems() { return (this->NumTabItems() - scrollInfo.num); }	// quantidade m�xima de itens num scroll
	int NumScrollHideItems() { return (this->HasScroll() ? (this->NumScrollItems() - scrollInfo.index) : 0); }	// quantidade de itens n�o vis�veis
	int FirstScrollIndex() { return (this->FirstTabIndex() + scrollInfo.index); }	// �ndice inicial com base no �ndice do scroll
	int LastScrollIndex() { return (this->LastTabIndex() - this->NumScrollHideItems()); }	// �ndice final com base nos itens n�o vis�veis

	// cursor
	bool IsLClicked() { return this->c.isLClicked; }
	bool IsRClicked() { return this->c.isRClicked; }
	bool IsHolding() { return this->c.isHolding; }

	// verifica se o cursor est� sobre a regi�o determinada
	bool IsMouseOver(float x, float y, float w, float h)
	{
		return (c.pos.x > x && c.pos.x < x + w && c.pos.y > y && c.pos.y < y + h);
	}
};

extern Menu menu;
extern Menu::Cheats Cheat;
#endif //__MENU_H__