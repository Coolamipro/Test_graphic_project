// GUI_Application.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GUI_Application.h"
#include "TestDll.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

testdll::cScene scene = testdll::cScene();

using namespace testdll;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUIAPPLICATION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIAPPLICATION));

	MSG msg;




	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIAPPLICATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUIAPPLICATION);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		scene.AddLSys(cPoint(0.0, 0.0, 0.0), cPoint(50.0, 0.0, 0.0), cPoint(0.0, 50.0, 0.0));
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		scene.SetScreenRest(ps.rcPaint.right, ps.rcPaint.bottom);
		// Draw all geometry 
		// in current state
		scene.DrawGeometry(hdc);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_UP:
			scene.MoveCamera(cPoint(0, 0, 10));
			break;
		case VK_DOWN:
			scene.MoveCamera(cPoint(0, 0, -10));
			break;
		case VK_LEFT:
			scene.MoveCamera(cPoint(0, -10, 0));
			break;
		case VK_RIGHT:
			scene.MoveCamera(cPoint(0, 10, 0));
			break;
		case 'Q':
			scene.MoveCamera(cPoint(10, 0, 0));
			break;
		case 'A':
			scene.MoveCamera(cPoint(-10, 0, 0));
			break;
		case 'W':
			scene.RotateCamera(cPoint(1, 0, 0));
			break;
		case 'S':
			scene.RotateCamera(cPoint(-1, 0, 0));
			break;
		case 'Z':
			scene.RotateCamera(cPoint(0, 1, 0));
			break;
		case 'X':
			scene.RotateCamera(cPoint(0, -1, 0));
			break;
		case 'E':
			scene.RotateCamera(cPoint(0, 0, 1));
			break;
		case 'D':
			scene.RotateCamera(cPoint(0, 0, -1));
			break;
		case 'R':
			scene.MoveCurrent(cPoint(0, 0, 1));
			break;
		case 'F':
			scene.MoveCurrent(cPoint(0, 0, -1));
			break;
		case 'V':
			scene.MoveCurrent(cPoint(0, -1, 0));
			break;
		case 'B':
			scene.MoveCurrent(cPoint(0, 1, 0));
			break;
		case 'T':
			scene.MoveCurrent(cPoint(-1, 0, 0));
			break;
		case 'G':
			scene.MoveCurrent(cPoint(1, 0, 0));
			break;
		case 'Y':
			scene.RotateCurrent(cPoint(1, 0, 0));
			break;
		case 'H':
			scene.RotateCurrent(cPoint(-1, 0, 0));
			break;
		case 'U':
			scene.RotateCurrent(cPoint(0, 1, 0));
			break;
		case 'J':
			scene.RotateCurrent(cPoint(0, -1, 0));
			break;
		case 'N':
			scene.RotateCurrent(cPoint(0, 0, 1));
			break;
		case 'M':
			scene.RotateCurrent(cPoint(0, 0, -1));
			break;
		case 'I':
			scene.RotateAround(cVector(0, 0, 1, 1), cPoint(100, 100, 0), 1);
			break;
		case 'K':
			scene.RotateAround(cVector(0, 0, 1, 1), cPoint(100, 100, 0), -1);
			break;
		case 'L':
			scene.AddLSys(cPoint(0, 0, 0), cPoint(10, 0, 0), cPoint(0, 10, 0));
			break;
		case 'P':
			scene.AddBox(cPoint(50, 50, 0), cPoint(200, 200, 0), cPoint(200, 200, 100));
			break;
		case 'O':
			scene.AddSphere(cPoint(-150, -150, 0), 100);
			break;
		case VK_TAB:
			scene.SetNext();
			break;
		case VK_DELETE:
			scene.DeleteCurrent();
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
