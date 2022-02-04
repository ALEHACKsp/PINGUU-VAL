#define _CRT_SECURE_NO_WARNINGS
#include "draw.h"
#include "input.h"

#include "lazyimporter.hh"

#include "xor.hpp"
#include <time.h>
#include <dwmapi.h>
#include "menu.h"
#include <mutex>


#include <iostream>

#include <d3d11.h>
#include "include/MinHook.h"
#pragma comment(lib, "d3d11.lib")

#include "offsets.h"

#pragma comment(lib, "dwmapi")

#include "Pointer.h"
#include <process.h>
#include <vector>
#include "defs.h"
#include "vectors.h"
#include "Drawing.h"

bool(__stdcall* LineOfSight)(uintptr_t, uintptr_t, Vector3*, bool) = nullptr;

#include "player.h"

vector<Enemy> entityList;

#include "AimAnglesShit.h"
#include <algorithm>

uintptr_t DiscordPresentDataPtr;
void* DiscordPresent, * DrawIndexedOriginal;

#define Player 0
#define Bot 1
#define Abs 2
#define sSpike 3
#define All 4
#define DropedSpike 5

#define red4 D3DCOLOR_RGBA(255, 37, 59, 255)
#define ared4 D3DCOLOR_RGBA(255, 37, 59, 50)
#define glassblack D3DCOLOR_RGBA(0, 0, 0, 180)
#define aglassblack D3DCOLOR_RGBA(0, 0, 0, 50)


HWND MyWnd = NULL;
MSG Message = { NULL };
const MARGINS Margin = { -1 ,-1, -1, -1 };
RECT GameRect = { NULL };
HWND GameWnd = NULL;

string tSelect[] = { xorstr_("Enemy"), xorstr_("All"), xorstr_("Bot") };
string lineStyle[] = { ("Off"), xorstr_("Top"), xorstr_("Middle"),xorstr_("Bottom") };
string boxStyle[] = { ("Off"), xorstr_("Corner"),xorstr_("2D"), xorstr_("3D") };
string onOff[] = { ("Off"), "On" };
string aimBone[] = { xorstr_("Head"),xorstr_("Neck"), xorstr_("Body") };
string aimFov[] = { "5", "10", "15", "20", "25", "30", "45", "60", "90", "120", "180" };
string multiplier[] = { "Off", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10" };
string aimTime[] = { ("0"), "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" };
string keys[] = { ("Capslock"), ("Alt"), ("LShift"), ("Fire"), "E", "F", "T", "V", "C", "X", "Right Click", "Mouse 5", "Mouse 6" };


static int enemy_size = 3;
D3DXVECTOR4 Rect;
uintptr_t entityx = 0;
bool bumbum = true;
bool trav = true;
IDirect3D9Ex* d3d;

#include "ESPLoop.h"

unsigned __stdcall cache(LPVOID lp)
{
	HideThread(GetCurrentThread());

	while (true)
	{
		pointer::ulevel = read<uintptr_t>(pointer::uworld + Offsets::oLevel);
		if (pointer::ulevel == NULL || pointer::game_instance == NULL || pointer::local_player == NULL || pointer::local_player_controller == NULL)
			pointer::uworld = ReadWorld();

		pointer::game_instance = read<uintptr_t>(pointer::uworld + Offsets::oGameInstance);
		if (pointer::game_instance == 0) continue;

		pointer::local_player = read<uintptr_t>(read<uintptr_t>(pointer::game_instance + Offsets::oLocalPlayers));
		if (pointer::local_player == 0) continue;

		pointer::local_player_controller = read<uintptr_t>(pointer::local_player + Offsets::oPlayerController);
		if (pointer::local_player_controller == 0) continue;

		pointer::camera_cache = read<uint64_t>(pointer::local_player_controller + Offsets::oCameraManager);
		if (pointer::camera_cache == 0) continue;

		pointer::local_player_pawn = read<uintptr_t>(pointer::local_player_controller + Offsets::oLocalPawn);
		if (pointer::local_player_pawn != pointer::old_player_pawn)
		{
			if (!entityList.empty())
			{
				entityList.clear();
				findEnemyID = -1;
			}

			pointer::old_player_pawn = pointer::local_player_pawn;
		}

		pointer::local_damage_handler = read<uintptr_t>(pointer::local_player_pawn + Offsets::DamageHandler);
		if (pointer::local_damage_handler == 0) continue;

		pointer::local_player_state = read<uintptr_t>(pointer::local_player_pawn + Offsets::PlayerState);
		if (pointer::local_player_state == 0)
		{
			if (!entityList.empty())
			{
				entityList.clear();
			}
			continue;
		}

		auto ActorArray = read<uintptr_t>(pointer::ulevel + 0xA0);
		if (ActorArray == 0) continue;

		pointer::local_team_component = read<uintptr_t>(pointer::local_player_state + Offsets::TeamComponent);
		if (pointer::local_team_component == 0) continue;

		pointer::local_team_id = read<int>(pointer::local_team_component + Offsets::Team);

		pointer::local_root_comp = read<uint64_t>(pointer::local_player_pawn + Offsets::RootComp);

		if (pointer::local_player_pawn)
		{
			auto Inventory = read<intptr_t>(pointer::local_player_pawn + Offsets::Inventory);
			if (Inventory)
			{
				intptr_t CurrentEquip = read<intptr_t>(Inventory + Offsets::CurrentWeapon);
				if (CurrentEquip)
				{
					int objID = read<int>(CurrentEquip + Offsets::ObjID);
					pointer::current_weapon = GetWeaponName(objID);
				}
				else
				{
					pointer::current_weapon = "Error";
				}
			}
		}

		int ActorCount = read<int>(pointer::ulevel + 0xA8);
		for (int i = 0; i < ActorCount; i++)
		{
			uintptr_t Actor = read<uintptr_t>(ActorArray + (i * 0x8));
			if (Actor)
			{
				if (Actor == pointer::local_player_pawn)
					continue;

				int UniqueId = read<int>(Actor + Offsets::UniqueID);

				uintptr_t Mesh = read<uintptr_t>(Actor + Offsets::Mesh);
				if (!Mesh)
					continue;

				uintptr_t DmgHandler = read<uintptr_t>(Actor + Offsets::DamageHandler);
				uintptr_t EnemyPlayerState = read<uintptr_t>(Actor + Offsets::PlayerState);
				uintptr_t EnemyRootComp = read<uint64_t>(Actor + Offsets::RootComp);

				int obj_id = read<int>(Actor + Offsets::ObjID);
				int enemy_team = read<int>(read<uintptr_t>(EnemyPlayerState + Offsets::TeamComponent) + Offsets::Team);				
				int bone_count = read<int>(Mesh + Offsets::BoneCount);

				Enemy valEntity{ };

				if (UniqueId == 18743553)
				{							
					if (bone_count == 103) // Bot
					{						
						valEntity.actor_ptr = Actor;
						valEntity.actor_type = Bot;
						valEntity.mesh_ptr = Mesh;
						valEntity.bone_count = bone_count;
						valEntity.damage_handler_ptr = DmgHandler;
						valEntity.player_state_ptr = EnemyPlayerState;
						valEntity.object_id = obj_id;
						valEntity.team_id = enemy_team;
						valEntity.root_component_ptr = EnemyRootComp;

						if (!entityList.empty())
						{
							auto it = std::find(entityList.begin(), entityList.end(), valEntity);
							if (it == entityList.end())
							{
								entityList.push_back(valEntity);
							}
						}
						else
						{
							entityList.push_back(valEntity);
						}
					}

					if ((bone_count == 102 || bone_count == 99) && enemy_team != pointer::local_team_id && EnemyPlayerState) // Spieler
					{						
						valEntity.actor_ptr = Actor;
						valEntity.actor_type = Player;
						valEntity.mesh_ptr = Mesh;
						valEntity.bone_count = bone_count;
						valEntity.damage_handler_ptr = DmgHandler;
						valEntity.player_state_ptr = EnemyPlayerState;
						valEntity.object_id = obj_id;
						valEntity.team_id = enemy_team;
						valEntity.root_component_ptr = EnemyRootComp;

						if (!entityList.empty())
						{
							auto it = std::find(entityList.begin(), entityList.end(), valEntity);
							if (it == entityList.end())
							{
								entityList.push_back(valEntity);
							}
						}
						else
						{
							entityList.push_back(valEntity);
						}
					}
				}
				else
				{
					if (obj_id == 12922391 && UniqueId == 17694977) // Spike
					{						
						valEntity.actor_ptr = Actor;
						valEntity.actor_type = sSpike;
						valEntity.mesh_ptr = Mesh;
						valEntity.bone_count = bone_count;
						valEntity.damage_handler_ptr = DmgHandler;
						valEntity.player_state_ptr = EnemyPlayerState;
						valEntity.object_id = obj_id;
						valEntity.team_id = enemy_team;
						valEntity.root_component_ptr = EnemyRootComp;

						if (!entityList.empty())
						{
							auto it = std::find(entityList.begin(), entityList.end(), valEntity);
							if (it == entityList.end())
							{
								entityList.push_back(valEntity);
							}
						}
						else
						{
							entityList.push_back(valEntity);
						}
					}	
					else if(obj_id == 12971480 && UniqueId == 17694977)
					{
						valEntity.actor_ptr = Actor;
						valEntity.actor_type = DropedSpike;
						valEntity.mesh_ptr = Mesh;
						valEntity.bone_count = bone_count;
						valEntity.damage_handler_ptr = DmgHandler;
						valEntity.player_state_ptr = EnemyPlayerState;
						valEntity.object_id = obj_id;
						valEntity.team_id = enemy_team;
						valEntity.root_component_ptr = EnemyRootComp;

						if (!entityList.empty())
						{
							auto it = std::find(entityList.begin(), entityList.end(), valEntity);
							if (it == entityList.end())
							{
								entityList.push_back(valEntity);
							}
						}
						else
						{
							entityList.push_back(valEntity);
						}
					}
					/*else
					{
						valEntity.actor_ptr = Actor;
						valEntity.actor_type = All;
						valEntity.mesh_ptr = Mesh;
						valEntity.bone_count = bone_count;
						valEntity.damage_handler_ptr = DmgHandler;
						valEntity.player_state_ptr = EnemyPlayerState;
						valEntity.object_id = obj_id;
						valEntity.team_id = enemy_team;
						valEntity.root_component_ptr = EnemyRootComp;

						if (!entityList.empty())
						{
							auto it = std::find(entityList.begin(), entityList.end(), valEntity);
							if (it == entityList.end())
							{
								entityList.push_back(valEntity);
							}
						}
						else
						{
							entityList.push_back(valEntity);
						}
					}*/
				}
			}
		}
		Sleep(1);
	}
}

void InitializeMenu()
{
	menu.Header(xorstr_("Vanguard"));

	menu.AddTab(xorstr_("All"));
	menu.AddItem(xorstr_("Team Select"), tSelect, 1, menu.items[0].value);
	menu.AddItem(xorstr_("Box ESP"), onOff, 2, menu.items[1].value);
	menu.AddItem(xorstr_("Line ESP"), lineStyle, 4, menu.items[2].value);
	menu.AddItem(xorstr_("Head ESP"), onOff, 2, menu.items[3].value);
	menu.AddItem(xorstr_("Health ESP"), onOff, 2, menu.items[4].value);
	menu.AddItem(xorstr_("Skeleton ESP"), onOff, 2, menu.items[5].value);
	menu.AddItem(xorstr_("Character ESP"), onOff, 2, menu.items[6].value);
	menu.AddItem(xorstr_("Dropped Spike (Buggy)"), onOff, 2, menu.items[7].value);
	menu.AddItem(xorstr_("Spike ESP"), onOff, 2, menu.items[8].value);
	menu.AddItem(xorstr_("Draw Radar"), onOff, 2, menu.items[9].value);

	menu.AddItem(xorstr_("Aimbot"), onOff, 2, menu.items[10].value);
	menu.AddItem(xorstr_("Aim Key"), keys, 13, menu.items[11].value);
	menu.AddItem(xorstr_("Aim Bone"), aimBone, 3, menu.items[12].value);
	menu.AddItem(xorstr_("Aim Fov"), aimFov, 11, menu.items[13].value);
	menu.AddItem(xorstr_("Draw Fov"), onOff, 2, menu.items[14].value);
	menu.AddItem(xorstr_("Smooth Aim"), aimTime, 21, menu.items[15].value);
	menu.AddItem(xorstr_("Control Recoil "), onOff, 2, menu.items[16].value);

	menu.items[16].value = 1;
	menu.items[17].value = 1;

	menu.AddItem(xorstr_("Dark Mode"), onOff, 2, menu.items[17].value);
}

void render()
{
	draw.GetDevice()->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, NULL);
	draw.GetDevice()->BeginScene();

	// input.Update();

	GameWnd = FindWindow(xorstr_("UnrealWindow"), 0);
	// RtlSecureZeroMemory(&wnd_str, sizeof(wnd_str));

	if (!GameWnd)
		ExitProcess(0);

	if (GameWnd == GetForegroundWindow() || GetActiveWindow() == GetForegroundWindow())
	{
		draw.GetDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		menu.Render(GameWnd);


		EspLoop();
		findNearestEnemy();

		auto Inventory = read<intptr_t>(pointer::local_player_pawn + Offsets::Inventory);
		if (Inventory)
		{	
			char dist_[64];
			sprintf(dist_, xorstr_("My Weapon: %s"), pointer::current_weapon);
			DrawString(50, 50, Red, dist_);
		}


		// Der tolle andere shit
		// { "5", "10", "15", "20", "25", "30", "45", "60", "90", "120", "180" };

		if (menu.items[13].value == 0)
			AimFOV = 10.f;
		else if (menu.items[13].value == 1)
			AimFOV = 20.f;
		else if (menu.items[13].value == 2)
			AimFOV = 30.f;
		else if (menu.items[13].value == 3)
			AimFOV = 40.f;
		else if (menu.items[13].value == 4)
			AimFOV = 50.f;
		else if (menu.items[13].value == 5)
			AimFOV = 60.f;
		else if (menu.items[13].value == 6)
			AimFOV = 90.f;
		else if (menu.items[13].value == 7)
			AimFOV = 120.f;
		else if (menu.items[13].value == 8)
			AimFOV = 180.f;
		else if (menu.items[13].value == 9)
			AimFOV = 240.f;
		else if (menu.items[13].value == 10)
			AimFOV = 360.f;

		if (menu.items[11].value == 0)
			Aimkey = VK_CAPITAL;
		else if (menu.items[11].value == 1)
			Aimkey = VK_MENU;
		else if (menu.items[11].value == 2)
			Aimkey = VK_LSHIFT;
		else if (menu.items[11].value == 3)
			Aimkey = VK_LBUTTON;
		else if (menu.items[11].value == 4)
			Aimkey = 'E';
		else if (menu.items[11].value == 5)
			Aimkey = 'F';
		else if (menu.items[11].value == 6)
			Aimkey = 'T';
		else if (menu.items[11].value == 7)
			Aimkey = 'V';
		else if (menu.items[11].value == 8)
			Aimkey = 'C';
		else if (menu.items[11].value == 9)
			Aimkey = 'X';
		else if (menu.items[11].value == 10)
			Aimkey = VK_RBUTTON;
		else if (menu.items[11].value == 11)
			Aimkey = 0x05;
		else if (menu.items[11].value == 11)
			Aimkey = 0x06;

		if (menu.items[15].value == 0)
			Smooth = 0.f;
		else if (menu.items[15].value == 1)
			Smooth = 3.5f;
		else  if (menu.items[15].value == 2)
			Smooth = 4.f;
		else  if (menu.items[15].value == 3)
			Smooth = 5.f;
		else  if (menu.items[15].value == 4)
			Smooth = 6.f;
		else  if (menu.items[15].value == 5)
			Smooth = 7.f;
		else if (menu.items[15].value == 6)
			Smooth = 8.f;
		else if (menu.items[15].value == 7)
			Smooth = 9.f;
		else if (menu.items[15].value == 8)
			Smooth = 10.f;
		else if (menu.items[15].value == 9)
			Smooth = 11.f;
		else if (menu.items[15].value == 10)
			Smooth = 12.f;
		else if (menu.items[15].value == 11)
			Smooth = 13.f;
		else if (menu.items[15].value == 12)
			Smooth = 14.f;
		else if (menu.items[15].value == 13)
			Smooth = 15.f;
		else if (menu.items[15].value == 14)
			Smooth = 16.f;
		else if (menu.items[15].value == 15)
			Smooth = 17.f;
		else if (menu.items[15].value == 16)
			Smooth = 18.f;
		else if (menu.items[15].value == 17)
			Smooth = 19.f;
		else if (menu.items[15].value == 18)
			Smooth = 20.f;
		else if (menu.items[15].value == 19)
			Smooth = 21.f;
		else if (menu.items[15].value == 20)
			Smooth = 22.f;

		if (menu.items[10].value != 0)
		{
			if (GetAsyncKeyState(Aimkey))
				AimbotHandling();
		
		}
	}

	draw.GetDevice()->EndScene();
	draw.GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CleanuoD3D()
{
	if (draw.GetDevice() != NULL)
	{
		draw.GetDevice()->EndScene();
		draw.GetDevice()->Release();
	}
	if (d3d != NULL)
	{
		d3d->Release();
	}
}

unsigned __stdcall CheckLoop(LPVOID lp)
{
	HideThread(GetCurrentThread());

	while (true)
	{
		if (!entityList.empty() && pointer::game_instance && pointer::local_player_pawn && pointer::ulevel)
		{
			for (int i = 0; i < entityList.size(); i++)
			{
				auto enemy = entityList[i];

				if (enemy.actor_ptr == (uint64_t)nullptr)
				{
					// Delete enemy
					auto it = std::find(entityList.begin(), entityList.end(), enemy);
					if (it != entityList.end())
					{
						entityList.erase(it);
					}
				}

				int ActorCount = read<int>(pointer::ulevel + 0xA8);
				auto ActorArray = read<uintptr_t>(pointer::ulevel + 0xA0);
				if (ActorArray == 0) continue;

				bool exist = false;
				for (int i = 0; i < ActorCount; i++)
				{
					uintptr_t Actor = read<uintptr_t>(ActorArray + (i * 0x8));
					if (enemy.actor_ptr == enemy.actor_ptr)
					{
						exist = true;
					}
				}

				if (exist == false)
				{
					auto it = std::find(entityList.begin(), entityList.end(), enemy);
					if (it != entityList.end())
					{
						entityList.erase(it);
					}
				}
			}
		}

		Sleep(2);
	}
}

unsigned __stdcall MainLoop(LPVOID lp)
{
	//  Lala(xorstr_("qwerwefv"));
	HideThread(GetCurrentThread());
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));
	InitializeMenu();
	while (Message.message != WM_QUIT)
	{
		if (!draw.IsInitialized())
		{
			D3DXCreateFontA(draw.GetDevice(), 16, 7, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY,    // default Quality
				DEFAULT_PITCH | FF_DONTCARE, xorstr_("Calibri"), &draw.font);
			D3DXCreateLine(draw.GetDevice(), &xwwwwwwqq);

			//  input.Init(MyWnd);
			draw.Init();
		}
		render();
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	// Lala(xorstr_("qwerwefv"));
	DestroyWindow(MyWnd);
	CleanuoD3D();
	ExitProcess(0);
	return Message.wParam;
}

bool initD3D(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d)))
		exit(3);
	// d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = 0;
	d3dpp.BackBufferHeight = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(d3d->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, 0, &draw.device)))
	{
		d3d->Release();
		exit(4);
	}
	// d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &draw.device);

	 //D3DXCreateLine(draw.device, &xwwwwwwqq);
	d3d->Release();//boşalt

	// ImGUi

	

	return true;

}

string gen_random(const int len) {
	string tmp_s;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	srand((unsigned)time(NULL) * _getpid());
	tmp_s.reserve(len);
	for (int i = 0; i < len; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	return tmp_s;
}

void SetWindowToTarget()
{
	const char* wnd_str = xorstr_("UnrealWindow");
	GameWnd = FindWindow(wnd_str, 0);
	RtlSecureZeroMemory(&wnd_str, sizeof(wnd_str));
	ZeroMemory(&GameRect, sizeof(GameRect));
	GetWindowRect(GameWnd, &GameRect);
	Width = GameRect.right - GameRect.left;
	Height = GameRect.bottom - GameRect.top;
	DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
	if (dwStyle & WS_BORDER)
	{
		GameRect.top += 32;
		Height -= 39;
	}
	ScreenCenterX = Width / 2;
	ScreenCenterY = Height / 2;
}


void ASCDAVSDFASCXD()
{
	//AllocConsole();
	//freopen(xorstr_("con"), xorstr_("w"), stdout);

	//printf(xorstr_("[] -> Init Internal\n"));

	while (!GetModuleHandleA(xorstr_("d3d9.dll"))) {
		Sleep(1);
	}

	std::string yey = gen_random(12);
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(RGB(0, 0, 0));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = yey.c_str();
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
		exit(1);
	SetWindowToTarget();
	MyWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		yey.c_str(),
		gen_random(8).c_str(),//""
		WS_POPUP | WS_VISIBLE,
		GameRect.left, GameRect.top, Width, Height,
		NULL, NULL, 0, NULL);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(MyWnd, &Margin);
	//Lala(xorstr_("qwerwefv"));
	_beginthreadex(0, 0, cache, 0, 0, 0);
	// Lala(xorstr_("qwerwefv"));
	if (initD3D(MyWnd))
	{
		LineOfSight = reinterpret_cast<decltype(LineOfSight)>(valBase + 0x4B39410);

		//  Lala(xorstr_("qwerwefv"));
		_beginthreadex(0, 0, MainLoop, 0, 0, 0);
		_beginthreadex(0, 0, CheckLoop, 0, 0, 0);
	}
	// Lala(xorstr_("qwerwefv"));
}

bool __stdcall DllMain(HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
	HideThread(hModule);
	if (dwAttached == DLL_PROCESS_ATTACH) {
		ASCDAVSDFASCXD();

		//DiscordPresentDataPtr = (uintptr_t)GetModuleHandleA("DiscordHook64.dll") + 0x1B3080;
		//DiscordPresent = InterlockedExchangePointer((volatile PVOID*)DiscordPresentDataPtr, PresentHook);

		CloseHandle(hModule);
	}
	return 1;
}
