#include "Arduino.h"
#include <vector>

char* intToBytes(int paramInt)
{
	std::vector<char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte.data();
}

Mouse::Mouse(SerialPort* pArduino)
{
	arduino = pArduino;
}

void Mouse::Send(int x, int y, int a, int b, int c, int d)
{
	arduino->writeSerialPort(&x, 1);
	arduino->writeSerialPort(&y, 1);
	arduino->writeSerialPort(&a, 1);
	arduino->writeSerialPort(&b, 1);
	arduino->writeSerialPort(&c, 1);
	arduino->writeSerialPort(&d, 1);
}

void Mouse::terminateThreads() {
	suspendThreads = (threads > 0) ? true : false;
}