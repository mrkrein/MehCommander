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
	const DWORD buttonSep = BTNS_SEP;

	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);
	
	if (hWndToolbar == NULL) {
		return NULL;
	}

	// Создание списка иконок.
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR32 | ILC_MASK, numButtons, 0);
	if (g_hImageList == NULL) {
		return NULL;
	}

	// Установка списка иконок.
	SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);

	// Загрузка изображений кнопок.
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

	// Добавление кнопок.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)& tbButtons);


	// Изменение размера панели и вывод ее
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);


	return hWndToolbar;
}


// Создание рабочей области (Основное окно)




// Строка состояния (макросы)
HWND StatusBar(HWND hWndParent, HINSTANCE hInst) {
	HWND hStatusWnd = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWndParent, NULL, hInst, NULL);

	int widths[] = { 150, 300, 450, 600, 750, 900, 1050, 1200, }; // ? мб функцию бахнуть xD
	SendMessage(hStatusWnd, SB_SETPARTS, 8, (LPARAM)widths);

	SendMessage(hStatusWnd, SB_SETTEXT, 0, (LPARAM)L"\tF3 Просмотр"); 
	SendMessage(hStatusWnd, SB_SETTEXT, 1, (LPARAM)L"\tF4 Правка");
	SendMessage(hStatusWnd, SB_SETTEXT, 2, (LPARAM)L"\tF5 Копирование");
	SendMessage(hStatusWnd, SB_SETTEXT, 3, (LPARAM)L"\tF6 Перемещение");
	SendMessage(hStatusWnd, SB_SETTEXT, 4, (LPARAM)L"\tF7 Каталог");
	SendMessage(hStatusWnd, SB_SETTEXT, 5, (LPARAM)L"\tF8 Удаление");
	SendMessage(hStatusWnd, SB_SETTEXT, 6, (LPARAM)L"\tF9 Терминал");
	SendMessage(hStatusWnd, SB_SETTEXT, 7, (LPARAM)L"\tAlt+F4 Выход");
	
	return hStatusWnd;
}
 

// Вывод текста на экран
HWND TextOutFunc(HWND hWndParent, HINSTANCE hInst) {
	HWND hStaticText = CreateWindowEx(0, L"STATIC", L"Йоу, хелло мир, body4 крутит мир.\n Мне стало впадлу делать вывод списка файлов и поэтому я просто сделал вывод текста на экран в родительское окно.",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		30, 30, 300, 200, hWndParent, NULL, hInst, NULL);

	// Изменение шрифта
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

	1.Создать кнопки и иконки для панели инструментов
	2.Загрузить их в меню
	3.Создать строку состояния
	4.Вывести в нее имеющиеся хоткеи (текстом)

*/
