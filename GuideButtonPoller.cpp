/*
 * GuideButtonPoller.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: James
 */
//#include "stdafx.h"  //Uncomment this line for Visual Studio
#include <windows.h>
#include <iostream>

//Clear Screen function I found in an article on cplusplus.com.  I considered
//using NCurses but decided that was overkill
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR) ' ',
			cellCount,
			homeCoords,
			&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition( hStdOut, homeCoords );
}

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
		ClearScreen();  //clear the screen
		std::cout << buttons.guideButton;  //simply access the variable like normal.  Easy peasy.
		Sleep(50);  //pause briefly so as not to spam the console
	}
	//in a real program you should release the dll by calling FreeLibrary(hGetProcIDDLL) to prevent memory
	//leaks, but since there's no way of cleanly exiting this program, I'm not sure where to put it
	
	return 0;
}



