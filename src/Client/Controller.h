#pragma once
// ----------------------------------------------------------------------------------------------
//static HHOOK KeyboardHook = NULL;
//static HHOOK MouseHook = NULL; 
class Controller
{
public:
	bool	Load();
	// ----
	static	LRESULT WINAPI Mouse(int Code, WPARAM wParam, LPARAM lParam);
	static	LRESULT WINAPI Keyboard(int Code, WPARAM wParam, LPARAM lParam);
	static	LRESULT WINAPI Window(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam);
	// ----
	HINSTANCE	Instance;
	// ----
	WPARAM wMouse;
	WPARAM wKeyBord;
private:
	HHOOK		MouseHook;
	HHOOK		KeyboardHook;
	// ----
}; extern Controller gController;

//BOOL KeyboardSetHook(BOOL set_or_remove);
//BOOL MouseSetHook(BOOL set_or_remove);
//LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
extern WPARAM mParam;

#define AUTOCLICK_LEFT		70001
#define AUTOCLICK_RIGHT		70002


extern bool         AutoClick[3];
// ----------------------------------------------------------------------------------------------