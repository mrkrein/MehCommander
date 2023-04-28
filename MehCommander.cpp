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
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Создать символьную ссылку \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Создать жесткую ссылку \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Создать каталог \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Создать ярлык \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// Разделитель
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Изменить атрибуты \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Свойства файла \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Подсчитать занимаемое место \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Сравнить по содержимому \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Групповое переименование \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// Разделитель
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Упаковать.. \t Alt+F5");						// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Распаковать.. \t Alt+F9");					// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Протестировать архив(ы) \t Shift+Alt+F9");	// Файлы -> Выход
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Разрезать файл..");							// Файлы -> Разрезать файлы
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Обьеденить файлы..");							// Файлы -> Обьеденить файлы
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Посчитать контрольные суммы..");				// Файлы -> Посчитать хеш суммы
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Проверить контрольные суммы..");				// Файлы -> Проверить хеш суммы
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// Разделитель
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Стереть (Wipe) \t Alt+Del");					// Файлы -> Стереть
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Удалить \t F8");								// Файлы -> Удалить
	AppendMenu(FilesMenu, MF_SEPARATOR, NULL, NULL);												// Разделитель
	AppendMenu(FilesMenu, MF_STRING, OnMenuClicked, L"Выход \t Alt+x");								// Файлы -> Выход

	SetMenu(hWnd, RootMenu);
}

// Панель инструментов
HIMAGELIST g_hImageList = NULL;
HWND CommandToolbar(HWND hWndParent, HINSTANCE hInst) {

	// Декларирование и инициализирование локальных констант
	const int ImageListID = 0;
	const int numButtons = 6;
	const int bitmapSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;
	const DWORD buttonSep = BTNS_BUTTON;

	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);
	
	if (hWndToolbar == NULL) {
		return NULL;
	}

	// Создание списка иконок
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16 | ILC_MASK, numButtons, 0);
	if (g_hImageList == NULL) {
		return NULL;
	}

	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)ImageListID,
		(LPARAM)g_hImageList);

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR,
		(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_FILENEW,  ImageListID), OnMenuClicked ,  TBSTATE_ENABLED, buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILEOPEN, ImageListID), OnMenuClicked , TBSTATE_ENABLED, buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , TBSTATE_ENABLED,               buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , TBSTATE_ENABLED,               buttonSep, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , TBSTATE_ENABLED,               buttonStyles, {0}, 0},
		{ MAKELONG(STD_FILESAVE, ImageListID), OnMenuClicked , 0,               buttonStyles, {0}, 0}
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)& tbButtons);


	// Изменение размера панели и вывод ее
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}


// Строка состояния (макросы)


/*

	1.Создать кнопки и иконки для панели инструментов
	2.Загрузить их в меню
	3.Создать строку состояния
	4.Вывести в нее имеющиеся хоткеи (текстом)

*/