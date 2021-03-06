#include "Application.h"
#include "resource.h"
#include <stdio.h>
//====================================================================================================
//====================================================================================================
#define PEN 0x01
#define ERASER 0x02
//====================================================================================================
//====================================================================================================
HCURSOR cursorPen = NULL;
HCURSOR cursorEraser = NULL;
HCURSOR cursorDef = NULL;

POINT tmpPos;

/*cursor_data*/
POINT posCursor;

HPEN hPen;
HPEN tmpPen;
HPEN oldPen = NULL;

COLORREF colorDT;

char flDraw = NULL;
char flCursor = NULL;
/*cursor_data*/

HDC tmpHDC;
HDC memHDC;

HBITMAP bufferMap;
HBITMAP oldBuffMap;

HWND btnPen;

Canvas paintList;
Canvas upMenu;
Canvas leftMenu;
Canvas bottomMenu;

Button penBtn;
Button eraseBtn;

Button fileBtn;
Button editBtn;
Button viewBtn;
Button imageBtn;
//====================================================================================================
//====================================================================================================
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//====================================================================================================
//====================================================================================================
// CANVAS
//====================================================================================================
//====================================================================================================
const wchar_t NAME_CANVAS[] = L"Canvas";
//====================================================================================================
//====================================================================================================
int Canvas::fillData(HINSTANCE hInstance) {
	WNDCLASS canvas = {};

	canvas.lpszClassName = NAME_CANVAS;
	canvas.lpfnWndProc = WndProc;
	canvas.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClass(&canvas);

	return NULL;
}

void Canvas::createWnd(HWND parent, HINSTANCE hInstance, int xPos, int yPos, int Height, int Width) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->Height = Height;
	this->Width = Width;

	wchar_t m_report[256];

	if ((hWnd = CreateWindow(
		NAME_CANVAS,
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,

		xPos, yPos, Width, Height,

		parent,
		NULL,
		hInstance,
		NULL
	)) == NULL){
		MessageBox(parent, L"Error in canvas", L"Error", MB_OK);
		PostQuitMessage(EXIT_FAILURE);
	}
}
//====================================================================================================
//====================================================================================================
// BUTTON
//====================================================================================================
//====================================================================================================
const wchar_t NAME_BUTTON[] = L"BUTTON";
//====================================================================================================
//====================================================================================================
void Button::createWnd(HWND parent, HINSTANCE hInstance, int xPos, int yPos, 
					   int Height, int Width , const wchar_t NAME[], int hMenu, int BITMAP_ID) {
	HBITMAP hBM;
	
	this->xPos = xPos;
	this->yPos = yPos;
	this->Height = Height;
	this->Width = Width;

	wchar_t m_report[256];

	if ((hWnd = CreateWindowExW(
		NULL,
		NAME_BUTTON,
		NAME,
		WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,

		xPos, yPos, Width, Height,

		parent,
		(HMENU) hMenu,
		hInstance,
		NULL
	)) == NULL) {
		MessageBox(parent, L"Error in canvas", L"Error", MB_OK);
		PostQuitMessage(EXIT_FAILURE);
	}

	hBM = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(BITMAP_ID), IMAGE_BITMAP, 0, 0, 0);
	SendMessage(hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBM);
}
//====================================================================================================
//====================================================================================================
// APPLICATION
//====================================================================================================
//====================================================================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	return appMainWnd.classWndProc(hwnd, uMsg, wParam, lParam);
}

int App::fillData(HINSTANCE &hInstance, const wchar_t clsName[], int nCmdShow) {
	this->clsName = clsName;
	this->hInstance = hInstance;
	this->nCmdShow = nCmdShow;

	wcMain.hInstance = hInstance;
	wcMain.lpfnWndProc = WndProc;
	wcMain.lpszClassName = clsName;
	wcMain.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(MAIN_ICON), IMAGE_ICON, 32, 32, 0);
	wcMain.style = CS_HREDRAW | CS_VREDRAW;
	wcMain.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);

	return NULL;
}

int App::Init() {
	RECT rw;
	int centerWidth;
	int centerHeight;

	RegisterClassW(&wcMain);

	hMainWnd = CreateWindow(
		wcMain.lpszClassName,
		wcMain.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	// window update
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	// create paintlist
	paintList.fillData(hInstance);

	// calc window size and center
	GetClientRect(hMainWnd, &rw);

	// set default size
	paintList.setHeight(800);
	paintList.setWidth(800);

	// calc center
	centerWidth = (rw.right - rw.left) / 2;
	centerHeight = (rw.bottom - rw.top) / 2;

	/*CANVAS*/
	// create paintList
	paintList.createWnd(hMainWnd, hInstance, centerWidth, centerHeight, 400, 400);
	// create menu
	upMenu.createWnd(hMainWnd, hInstance, NULL, NULL, 40, rw.right - rw.left);
	// create left menu
	leftMenu.createWnd(hMainWnd, hInstance, NULL, 70, 200, 70);
	// create rgb menu
	bottomMenu.createWnd(hMainWnd, hInstance, NULL, 340, 150, 150);

	/*BUTTON*/
	// create pen button
	penBtn.createWnd(leftMenu.getHWND(), hInstance, 2, 2, 30, 30, L"", 10, IDB_BITMAP1);
	
	eraseBtn.createWnd(leftMenu.getHWND(), hInstance, 34, 2, 30, 30, L"", 20, IDB_BITMAP2);
	
	// create file btn
	fileBtn.createWnd(upMenu.getHWND(), hInstance, 3, 3, 30, 60, L"File", 2, NULL);
	// create edit btn
	editBtn.createWnd(upMenu.getHWND(), hInstance, 66, 3, 30, 60, L"Edit", 3, NULL);
	// create view btn
	viewBtn.createWnd(upMenu.getHWND(), hInstance, 129, 3, 30, 60, L"View", 4, NULL);
	// create image btn
	imageBtn.createWnd(upMenu.getHWND(), hInstance, 192, 3, 30, 60, L"Image", 5, NULL);

	cursorPen = LoadCursor( hInstance, MAKEINTRESOURCE(IDC_CURSOR2));
	// cursorEraser = LoadCursor( hInstance, MAKEINTRESOURCE(IDC_ICON2));
	cursorDef = LoadCursor(NULL, IDC_ARROW);

	return NULL;
}
//====================================================================================================
//====================================================================================================
int App::Run() {
	MSG uMsg = {};
	LPARAM lParam;
	RECT rw;

	// reaupdate function
	GetClientRect(hMainWnd, &rw);
 
	WORD nWidth = rw.right - rw.left; // LOWORD(lParam);   width of client area 
	WORD nHeight = rw.bottom - rw.top; //HIWORD(lParam);  height of client area
	lParam = nWidth | (nHeight << 16);

	// update window
	reUpFunc(lParam);

	// start recieving messages
	while (GetMessageW( &uMsg, hMainWnd, NULL, NULL) > 0) {
		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}

	return NULL;
}
//====================================================================================================
//====================================================================================================
void App::reUpFunc(LPARAM lParam) {
	/* positions */
	int nWidth;
	int nHeight;

	int centerWidth;
	int centerHeight;
	/* positions */

	// error code
	int Error;
	// error report
	wchar_t errReport[256];

	// get new size
	nWidth = LOWORD(lParam);  // width of client area
	nHeight = HIWORD(lParam); // height of client area 

	// calc center
	centerWidth = (nWidth - paintList.getWidth()) / 2;
	centerHeight = (nHeight - paintList.getHeight()) / 2;

	// move paintlist
	if (paintList.getHWND() != NULL) {
		if (centerHeight < 70)
			centerHeight = 70;
		if (centerWidth < 200)
			centerWidth = 200;

		// SetWindowPos(upMenu.getHWND(), NULL, NULL, NULL, , upMenu.getHeight(), SWP_NOZORDER);
		// MoveWindow(upMenu.getHWND(), NULL, NULL, nWidth, upMenu.getHeight(), true);
		
		if(nHeight - bottomMenu.getHeight() > 280)
			MoveWindow(bottomMenu.getHWND(), NULL, nHeight - bottomMenu.getHeight(), 
					   bottomMenu.getWidth(), bottomMenu.getHeight(), true);

		if (!MoveWindow(paintList.getHWND(), centerWidth, centerHeight, 400, 400, true)) {
			// get error code
			Error = GetLastError();

			// convert to wchar_t
			swprintf_s(errReport, L"%d", Error);
			MessageBox(hMainWnd, errReport, L"Error", MB_OK);
		}
	}
}
//====================================================================================================
//====================================================================================================
LRESULT CALLBACK App::classWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC paintListHDC;
	RECT rw;

	HWND tmp = NULL;

	RECT rect;
	GetClientRect(paintList.getHWND(), &rect);

	// check position
	if (flDraw) {
		GetCursorPos(&tmpPos);
		tmp = WindowFromPoint(tmpPos);
	}

	// start paint
	if (hwnd == paintList.getHWND() || tmp == paintList.getHWND())
		WndPaintCanvasProc(paintList.getHWND(), uMsg, wParam, lParam);
	else
		PostAppMessage(paintList.getHWND(), WM_LBUTTONUP, NULL, NULL);

	switch (uMsg){
	case WM_CREATE: {
			if (paintList.getHWND() != NULL && memHDC == NULL) {
				tmpHDC = GetDC(paintList.getHWND());

				memHDC = CreateCompatibleDC(tmpHDC);

				RECT rect;
				GetClientRect(paintList.getHWND(), &rect);
				bufferMap = CreateCompatibleBitmap(tmpHDC, rect.right, rect.bottom);

				oldBuffMap = (HBITMAP)SelectObject(memHDC, bufferMap);

				FillRect( memHDC, &rect, WHITE_BRUSH);

				ReleaseDC(paintList.getHWND(), tmpHDC);
			}
		}
		break;
	case WM_COMMAND:{
		switch (LOWORD(wParam)) {
			// create pen and set flCursor
			case 10:
				if (flCursor != PEN) {
					hPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 0));
					flCursor = PEN;
					tmpPen = hPen;

					// send message about changing cursor
					PostMessage(paintList.getHWND(), WM_SETCURSOR, NULL, NULL);
				}
				else {
					DeleteObject(hPen);
					flCursor = NULL;
					hPen = NULL;
				}
				break;

			// create eraser
			case 20:
				if (flCursor != ERASER) {
					hPen = CreatePen(PS_SOLID, 30, RGB(255, 255, 255));
					flCursor = ERASER;
					tmpPen = hPen;

					PostMessage(paintList.getHWND(), WM_SETCURSOR, NULL, NULL);
				}
				else {
					DeleteObject(hPen);
					flCursor = NULL;
					hPen = NULL;
				}
				break;
			}
		}
		break;

	// reaction on resizing wnd
	case WM_SIZE:
		reUpFunc(lParam);
		break;

	// paint in wnd
	case WM_PAINT:
		// start paint
		paintListHDC = BeginPaint(paintList.getHWND(), &ps);

		BitBlt( paintListHDC, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			memHDC, rect.left, rect.top, SRCCOPY);

		EndPaint(paintList.getHWND(), &ps);
		break;

	// destroy wnd
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		DeleteDC( memHDC);
		// send quit message
		PostQuitMessage(EXIT_SUCCESS);
		break;
	}

	// default reaction
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//====================================================================================================
//====================================================================================================
// FRIENDS
//====================================================================================================
//====================================================================================================
void WndPaintCanvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RECT rect;

	GetClientRect(paintList.getHWND(), &rect);

	switch (uMsg)
	{
	case WM_SETCURSOR:
		if (flCursor == PEN && cursorPen != NULL)
			SetCursor(cursorPen);
		else if(flCursor == ERASER && cursorEraser != NULL)
			SetCursor(cursorEraser);
		else
			SetCursor(cursorDef);
		break;
	case WM_LBUTTONDOWN:
		// get cursor pos
		posCursor.x = LOWORD(lParam);
		posCursor.y = HIWORD(lParam);

		flDraw = true;

		// start draw
		if (flDraw == true && hPen != NULL) {
			oldPen = (HPEN) SelectObject(memHDC, hPen);

			MoveToEx(memHDC, posCursor.x, posCursor.y, NULL);
			LineTo(memHDC, posCursor.x, posCursor.y);
		}
		else
			flDraw = false;
		break;
	case WM_LBUTTONUP:
		if (flDraw == true)
			flDraw = false;

		SelectObject(memHDC, oldPen);

		break;
	case WM_MOUSEMOVE:
		// get cursor pos
		if (flDraw == true){
			MoveToEx(memHDC, posCursor.x, posCursor.y, NULL);
			LineTo(memHDC, posCursor.x = LOWORD(lParam), posCursor.y = HIWORD(lParam));

			InvalidateRect(paintList.getHWND(), NULL, FALSE);
		}
		break;
	}
}