// DongHo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DongHo.h"
#include <time.h>
#include <ctime>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND lb_gio, lb_phut, lb_giay;
HWND edt_gio, edt_phut, edt_giay;
HWND btn_start, btn_stop, btn_reset;
HWND htron;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
	LoadStringW(hInstance, IDC_DONGHO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DONGHO));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DONGHO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DONGHO);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int gio, phut, giay, tongTime;
	static TCHAR chuoi[1000];
	static HDC hdc;
	static int xLeft = 0, moveToRight = 1;  // xLeft của hinhtron, hướng đi: sang phải = 1, sang trái = 0;
	switch (message)
	{
	case WM_CREATE:
		lb_gio = CreateWindow(L"static", L"Giờ:", WS_CHILD | WS_VISIBLE, 20, 20, 50, 30, hWnd, NULL, NULL, NULL);
		edt_gio = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 75, 20, 80, 30, hWnd, (HMENU)(1), NULL, NULL);

		lb_phut = CreateWindow(L"static", L"Phút:", WS_CHILD | WS_VISIBLE, 160, 20, 50, 30, hWnd, NULL, NULL, NULL);
		edt_phut = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 215, 20, 80, 30, hWnd, (HMENU)2, NULL, NULL);

		lb_giay = CreateWindow(L"static", L"Giây:", WS_CHILD | WS_VISIBLE, 300, 20, 50, 30, hWnd, NULL, NULL, NULL);
		edt_giay = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 355, 20, 80, 30, hWnd, (HMENU)3, NULL, NULL);

		btn_start = CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 55, 135, 30, hWnd, (HMENU)4, NULL, NULL);
		btn_stop = CreateWindow(L"button", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 160, 55, 135, 30, hWnd, (HMENU)5, NULL, NULL);
		btn_reset = CreateWindow(L"button", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 55, 135, 30, hWnd, (HMENU)6, NULL, NULL);
		break;
	case WM_TIMER:
		// Điều khiển đồng hồ
		gio = tongTime / 3600;
		wsprintf(chuoi, L"%d", gio);
		SetWindowText(edt_gio, chuoi);
		phut = (tongTime % 3600) / 60;
		wsprintf(chuoi, L"%d", phut);
		SetWindowText(edt_phut, chuoi);
		giay = (tongTime % 3600) % 60;
		wsprintf(chuoi, L"%d", giay);
		SetWindowText(edt_giay, chuoi);
		if (tongTime == 0)
		{
			KillTimer(hWnd, 100);   // Hủy Timer
			break;
		}
		tongTime--;
		// Điều khiển htron chạy 0->850
		if (xLeft + 250 >= 850) {
			moveToRight = 0;
		}
		if (xLeft <= 0)
		{
			moveToRight = 1;
		}
		if (moveToRight == 1)
		{
			xLeft += 30;
		}
		if (moveToRight == 0)
		{
			xLeft -= 30;
		}
		MoveWindow(htron, xLeft, 100, 250, 250, TRUE);
		
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case 4:
			if (GetWindowText(edt_gio, chuoi, 20) == 0)
				break;
			gio = _tstoi(chuoi);	// ÉP kiểu TCHAR -> int bằng _tstoi()
			if (GetWindowText(edt_phut, chuoi, 20) == 0)
				break;
			phut = _tstoi(chuoi);
			if (GetWindowText(edt_giay, chuoi, 20) == 0)
				break;
			giay = _tstoi(chuoi);

			if (giay > 60)
			{
				MessageBox(hWnd, L"Nhập số giây quá 60!", L"Toanggg", MB_OK | MB_ICONERROR);
				SendMessage(btn_reset, BM_CLICK, 1, 0);
				break;
			}
			if (phut > 60)
			{
				MessageBox(hWnd, L"Nhập số phút quá 60!", L"Toanggg", MB_OK | MB_ICONERROR);
				SendMessage(btn_reset, BM_CLICK, 1, 0);
				break;
			}
			tongTime = gio * 3600 + phut * 60 + giay;
			SetTimer(hWnd, 100, 1000, NULL);
			htron = CreateWindow(L"button", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, xLeft, 100, 250, 250, hWnd, (HMENU)7, NULL, NULL);
			hdc = GetDC(htron);
			Ellipse(hdc, 10, 10, 230, 230);
			ReleaseDC(htron, hdc);
			break;
		case 5: // Stop
			KillTimer(hWnd, 100);   // Hủy Timer để dừng đồng hồ
			break;
		case 6: // Reset
			KillTimer(hWnd, 100);   // Hủy Timer để dừng đồng hồ
			wsprintf(chuoi, L"");
			SetWindowText(edt_gio, chuoi);
			SetWindowText(edt_phut, chuoi);
			SetWindowText(edt_giay, chuoi);
			break;
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
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
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
