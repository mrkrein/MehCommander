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

	// Основное меню
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FilesMenu, L"Файлы");
	AppendMenu(RootMenu, MF_STRING, OnMenuClicked, L"Сеть");
	AppendMenu(RootMenu, MF_STRING, OnMenuClicked, L"Помощь");

	// Подменю для пункта (Файлы)
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Упаковать.. \t Alt+F5");						// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Распаковать.. \t Alt+F9");					// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Протестировать архив(ы) \t Shift+Alt+F9");	// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Разрезать файл..");							// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Обьеденить файлы..");							// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Посчитать контрольные суммы..");				// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Проверить контрольные суммы..");				// Файлы -> Выход
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// Разделитель
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Стереть (Wipe) \t Alt+Del");					// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Удалить \t F8");								// Файлы -> Выход
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// Разделитель
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Выход \t Alt+x");								// Файлы -> Выход

	SetMenu(hWnd, RootMenu);
}

// Панель инструментов
HWND CommandToolbar(HWND hWndParent, HINSTANCE hInst) {
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);
	
	if (hWndToolbar == NULL) {
		return NULL;
	}

	// Изменение размера панели и вывод ее
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}