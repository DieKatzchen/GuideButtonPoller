/*
 * GuideButtonPoller.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: James
 */
//#include "stdafx.h"  //Uncomment this line for Visual Studio
#include <windows.h>
#include <iostream>

int main(int argc, _TCHAR* argv[])
{
	struct ControllerStruct
	{
		unsigned long eventCount; //event counter, increases with every controller event,
					  //but for some reason not by one.
		unsigned short up:1, down:1, left:1, right:1, start:1, back:1, l3:1, r3:1,
                       lButton:1, rButton:1, guideButton:1, unknown:1, aButton:1,
                       bButton:1, xButton:1, yButton:1; // button state bitfield
		unsigned char lTrigger;  //Left Trigger
		unsigned char rTrigger;  //Right Trigger
		short lJoyY;  //Left Joystick Y
		short lJoyx;  //Left Joystick X
		short rJoyY;  //Right Joystick Y 
		short rJoyX;  //Right Joystick X
	};
	//First create an HINSTANCE of the xinput1_3.dll.  Probably should use system variables to find it
	//but whatever.
	HINSTANCE hGetProcIDDLL = LoadLibrary("C:/Windows/System32/xinput1_3.dll");  //In Visual Studio replace this
	//HINSTANCE hGetProcIDDLL = LoadLibrary(L"C:/Windows/System32/xinput1_3.dll");  //With this

	//Get the address of ordinal 100.
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), (LPCSTR)100);

	//typedef the function. It takes an int and a pointer to a ControllerStruct and returns an error code
	//as an int.  it's 0 for no error and 1167 for "controller not present".  presumably there are others
	//but I never saw them.  It won't cause a crash on error, it just won't update the data.
	typedef int(__stdcall * pICFUNC)(int, ControllerStruct &);

	//Assign it to getControllerData for easier use
	pICFUNC getControllerData;
	getControllerData = pICFUNC(lpfnGetProcessID);
	
	//Create in an instance of the ControllerStruct
	ControllerStruct buttons;
	while(1) //Infinite polling loop
	{
		getControllerData(0, buttons);  //call the function with the controller number(zero based) and
						//the pointer to the ControllerStruct.
		std::cout << buttons.guideButton;  //simply access the variable like normal.  Easy peasy.
	}
	//in a real program you should release the dll by calling FreeLibrary(hGetProcIDDLL) to prevent memory
	//leaks, but since there's no way of cleanly exiting this program, I'm not sure where to put it
	
	return 0;
}



