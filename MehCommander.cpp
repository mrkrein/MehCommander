#include <iostream>
#include <Windows.h>
#include "SoftwareDefinitions.h"
#include "resource.h"
#include <commctrl.h>
using namespace std;


int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nShowCmd) {
	WNDCLASSW SoftwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_MEHCOMMANDER)), L"MainWndClass", SoftwareMainProcedure);

	if (!RegisterClassW(&SoftwareMainClass)) { return -1; }
	MSG SoftwareMainMessage = { 0 };

	CreateWindowW(L"MainWndClass", L"MehCommander v1.0", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 350, 150, WinWidth, WinHeight, NULL, NULL, NULL, NULL);


	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessageW(&SoftwareMainMessage);
	}
	return 0;
}

WNDCLASSW NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASSW NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
		case OnMenuClicked:
			MessageBoxA(hWnd, "Menu was clicked!", "Menu Worked", MB_OK);
			break;
		default: break;
		}
		break;
	case WM_CREATE:
		MainWndAddMenus(hWnd);
		CommandToolbar(hWnd, ((LPCREATESTRUCT)lp)->hInstance);
		break;
	case WM_DESTROY:
		DestroyMenu(GetMenu(hWnd));
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void MainWndAddMenus(HWND hWnd) {
	HMENU RootMenu = CreateMenu();
	HMENU FilesMenu = CreateMenu();

	// �������� ����
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FilesMenu, L"�����");
	AppendMenu(RootMenu, MF_STRING, OnMenuClicked, L"����");
	AppendMenu(RootMenu, MF_STRING, OnMenuClicked, L"������");

	// ������� ��� ������ (�����)
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"���������.. \t Alt+F5");						// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�����������.. \t Alt+F9");					// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�������������� �����(�) \t Shift+Alt+F9");	// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� ����..");							// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"���������� �����..");							// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� ����������� �����..");				// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� ����������� �����..");				// ����� -> �����
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// �����������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� (Wipe) \t Alt+Del");					// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� \t F8");								// ����� -> �����
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// �����������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"����� \t Alt+x");								// ����� -> �����

	SetMenu(hWnd, RootMenu);
}

// ������ ������������
HWND CommandToolbar(HWND hWndParent, HINSTANCE hInst) {
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);
	
	if (hWndToolbar == NULL) {
		return NULL;
	}

	// ��������� ������� ������ � ����� ��
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}