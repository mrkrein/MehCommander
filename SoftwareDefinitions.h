#pragma once

// Константы
const int WinWidth = 1200;
const int WinHeight = 700;

// Дефайны для меню
#define OnMenuClicked 1

// Обработчики для окон
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

// Классы окон
WNDCLASSW NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

// Регистрация прототипов функций
HWND CommandToolbar(HWND hWndParent, HINSTANCE hInst);
HWND StatusBar(HWND hWndParent, HINSTANCE hInst);
HWND TextOutFunc(HWND hWndParent, HINSTANCE hInst);

// Добавление меню
void MainWndAddMenus(HWND hWnd);