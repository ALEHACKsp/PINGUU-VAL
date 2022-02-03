#ifndef __INPUT_H__
#define __INPUT_H__

#define DIRECTINPUT_VERSION	0x800

#include <d3d9.h>
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:
	void Init(HWND game);
	void Update();

	bool IsKeyUp(int key) { return !(MouseState.rgbButtons[key] & 0x80); }
	bool IsKeyDown(int key) { return (MouseState.rgbButtons[key] & 0x80); }
	bool IsScrollUp() { return MouseState.lZ > 0; }
	bool IsScrollDown() { return MouseState.lZ < 0; }

private:
	LPDIRECTINPUTDEVICE8	lpDIMouseDevice;
	LPDIRECTINPUT8			lpDIObject;

	DIMOUSESTATE2			MouseState;
};

extern Input input;

#endif //__INPUT_H__