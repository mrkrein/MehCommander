#pragma once

const int WinWidth = 1200;
const int WinHeight = 700;

#define OnMenuClicked 1

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASSW NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
HWND CommandToolbar(HWND hWndParent, HINSTANCE hInst);
HWND StatusBar(HWND hWndParent, HINSTANCE hInst);

void MainWndAddMenus(HWND hWnd);