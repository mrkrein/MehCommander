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
		StatusBar(hWnd, ((LPCREATESTRUCT)lp)->hInstance);
		TextOutFunc(hWnd, ((LPCREATESTRUCT)lp)->hInstance);
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
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� ���������� ������ \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� ������� ������ \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� ������� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� ����� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// �����������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�������� �������� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�������� ����� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"���������� ���������� ����� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�������� �� ����������� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� �������������� \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// �����������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"���������.. \t Alt+F5");						// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�����������.. \t Alt+F9");					// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"�������������� �����(�) \t Shift+Alt+F9");	// ����� -> �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� ����..");							// ����� -> ��������� �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"���������� �����..");							// ����� -> ���������� �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� ����������� �����..");				// ����� -> ��������� ��� �����
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"��������� ����������� �����..");				// ����� -> ��������� ��� �����
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// �����������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� (Wipe) \t Alt+Del");					// ����� -> �������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"������� \t F8");								// ����� -> �������
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// �����������
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"����� \t Alt+x");								// ����� -> �����

	SetMenu(hWnd, RootMenu);
}

// ������ ������������
HIMAGELIST g_hImageList = NULL;
HWND CommandToolbar(HWND hWndParent, HINSTANCE hInst) {

	// �������������� � ����������������� ��������� ��������
	const int ImageListID = 0;
	const int numButtons = 6;
	const int bitmapSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;
	const DWORD buttonSep = BTNS_SEP;

	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);
	
	if (hWndToolbar == NULL) {
		return NULL;
	}

	// �������� ������ ������.
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR32 | ILC_MASK, numButtons, 0);
	if (g_hImageList == NULL) {
		return NULL;
	}

	// ��������� ������ ������.
	SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);

	// �������� ����������� ������.
	SendMessage(hWndToolbar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);

	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_DELETE,  ImageListID), OnMenuClicked , TBSTATE_ENABLED, buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILENEW, ImageListID), OnMenuClicked , TBSTATE_ENABLED, buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , TBSTATE_ENABLED, buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , TBSTATE_ENABLED, buttonSep, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , TBSTATE_ENABLED, buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , 0, 0, {0}, 0}
	};

	// ���������� ������.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)& tbButtons);


	// ��������� ������� ������ � ����� ��
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);


	return hWndToolbar;
}


// �������� ������� ������� (�������� ����)




// ������ ��������� (�������)
HWND StatusBar(HWND hWndParent, HINSTANCE hInst) {
	HWND hStatusWnd = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);

	int widths[] = { 150, 300, 450, 600, 750, 900, 1050, 1200, }; // ? �� ������� ������� xD
	SendMessage(hStatusWnd, SB_SETPARTS, 8, (LPARAM)widths);

	SendMessage(hStatusWnd, SB_SETTEXT, 0, (LPARAM)L"\tF3 ��������"); 
	SendMessage(hStatusWnd, SB_SETTEXT, 1, (LPARAM)L"\tF4 ������");
	SendMessage(hStatusWnd, SB_SETTEXT, 2, (LPARAM)L"\tF5 �����������");
	SendMessage(hStatusWnd, SB_SETTEXT, 3, (LPARAM)L"\tF6 �����������");
	SendMessage(hStatusWnd, SB_SETTEXT, 4, (LPARAM)L"\tF7 �������");
	SendMessage(hStatusWnd, SB_SETTEXT, 5, (LPARAM)L"\tF8 ��������");
	SendMessage(hStatusWnd, SB_SETTEXT, 6, (LPARAM)L"\tF9 ��������");
	SendMessage(hStatusWnd, SB_SETTEXT, 7, (LPARAM)L"\tAlt+F4 �����");
	
	return hStatusWnd;
}
 

// ����� ������ �� �����
HWND TextOutFunc(HWND hWndParent, HINSTANCE hInst) {
	HWND hStaticText = CreateWindowEx(0, L"STATIC", L"���, ����� ���, body4 ������ ���.\n ��� ����� ������ ������ ����� ������ ������ � ������� � ������ ������ ����� ������ �� ����� � ������������ ����.",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		30, 30, 300, 200, hWndParent, NULL, hInst, NULL);

	// ��������� ������
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Calibri");
	if (hFont != NULL)
	{
		SendMessage(hStaticText, WM_SETFONT, (WPARAM)hFont, TRUE);
	}

	return hStaticText;
}

/*

	1.������� ������ � ������ ��� ������ ������������
	2.��������� �� � ����
	3.������� ������ ���������
	4.������� � ��� ��������� ������ (�������)

*/
