/*
 * GuideButtonPoller.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: James
 */

#include <windows.h>
#include <iostream>

int main(int argc, _TCHAR* argv[])
{
	struct ControllerStruct
	{
		unsigned long eventCount; //event counter
		unsigned short up:1, down:1, left:1, right:1, start:1, back:1, l3:1, r3:1,
                       lButton:1, rButton:1, guideButton:1, unknown:1, aButton:1,
                       bButton:1, xButton:1, yButton:1; // button state bitfield
		unsigned char lTrigger;
		unsigned char rTrigger;
		short lJoyY;
		short lJoyx;
		short rJoyY;
		short rJoyX;
	};

	HINSTANCE hGetProcIDDLL = LoadLibrary("C:/Windows/System32/xinput1_3.dll");

	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), (LPCSTR)100);

	typedef int(__stdcall * pICFUNC)(int, ControllerStruct &);

	pICFUNC getControllerData;
	getControllerData = pICFUNC(lpfnGetProcessID);
	ControllerStruct buttons;
	while(1)
	{
		getControllerData(0, buttons);
		std::cout << buttons.guideButton;
	}
	return 0;
}



