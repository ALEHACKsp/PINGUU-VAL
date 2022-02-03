#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>
#include <iostream>
#include <string>
#include "SerialPort.h"

class Mouse
{
private:
	bool suspendThreads = false;
	int threads = 0;
	SerialPort* arduino;

public:
	Mouse(SerialPort* pArduino);

	void Send(int x, int y, int a, int b, int c, int d);
	void terminateThreads();

};

#endif // !MOUSE_H