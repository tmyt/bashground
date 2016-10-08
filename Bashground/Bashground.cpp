// Bashground.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Bashground.h"

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100

// {77D1E74E-A377-4E03-943F-B686C9057A63}
static const GUID NotifyIconGUID =
{ 0x77d1e74e, 0xa377, 0x4e03,{ 0x94, 0x3f, 0xb6, 0x86, 0xc9, 0x5, 0x7a, 0x63 } };


// Global Variables:
HMENU hMenu;
DWORD dwTaskbarCreated;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE LaunchBash();
void RegisterIcon(HWND hWnd);
void DestroyIcon(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BASHGROUND, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	HWND hWnd = InitInstance(hInstance, nCmdShow);
	if (!hWnd)
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BASHGROUND));

	MSG msg;
	HANDLE hProcess = LaunchBash();
	if (!hProcess) {
		MessageBox(nullptr, _T("failed to launch bash.exe"), nullptr, 0);
		return 1;
	}
	RegisterIcon(hWnd);
	dwTaskbarCreated = RegisterWindowMessage(_T("TaskbarCreated "));

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DestroyIcon(hWnd);
	TerminateProcess(hProcess, 0);

	return (int)msg.wParam;
}

HANDLE LaunchBash() {
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	TCHAR* cmdline = _tcsdup(_T("bash.exe ~"));
	CreateProcess(nullptr, cmdline, nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
	return pi.hProcess;
}

void RegisterIcon(HWND hWnd) {
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WM_APP;
	nid.uVersion = NOTIFYICON_VERSION_4;
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), szTitle);
	LoadIconMetric(GetModuleHandle(0), MAKEINTRESOURCE(IDI_SMALL), LIM_SMALL, &(nid.hIcon));
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void DestroyIcon(HWND hWnd) {
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_GUID;
	nid.guidItem = NotifyIconGUID;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szWindowClass;

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hMenu = CreatePopupMenu();
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_ID | MIIM_STRING;
	mi.wID = IDM_EXIT;
	mi.dwTypeData = _T("&Exit");
	mi.cch = (UINT)_tcslen(mi.dwTypeData);
	InsertMenuItem(hMenu, 0, TRUE, &mi);
	return CreateWindowW(szWindowClass, szTitle, 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	if (message == dwTaskbarCreated) {
		// taskbar re-ceated, re-register taskbar icon
		RegisterIcon(hWnd);
		return 0;
	}
	switch (message)
	{
	case WM_APP:
		if (LOWORD(lParam) == WM_LBUTTONUP)
		{
			SetActiveWindow(hWnd);
			GetCursorPos(&pt);
			TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, hWnd, nullptr);
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
