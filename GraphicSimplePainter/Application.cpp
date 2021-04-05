#include "Application.h"
#include "resource.h"
#include <stdio.h>
//====================================================================================================
//====================================================================================================
#define PEN 0x01
#define ERASER 0x02
#define MOVE 0x03
#define ROTATE 0x04
#define SCALE 0x05
#define LINE 0x06
#define WND_LINE 0x07
#define TRIANGLE 0x10

// lines
Line wuLine;
Line wndLine;

// figures
// 1. triangle
Triangle trFigure;
LPARAM tmp_lParam;
HANDLE threadTr = NULL;
POINT tmpDt_thread;

char resFlag = false;
char flThread = false;
int numPnt = 0;
//====================================================================================================
//====================================================================================================
HCURSOR cursorPen = NULL;
HCURSOR cursorEraser = NULL;
HCURSOR cursorDef = NULL;

POINT tmpPos;

/*cursor_data*/
POINT posCursor;
POINT centerTmp;

HPEN hPen;
HPEN tmpPen;
HPEN oldPen = NULL;

COLORREF colorDT;

char flDraw = NULL;
char flLine = NULL;
char flCursor = NULL;
char flFigure = NULL;
/*cursor_data*/

	// points for line
POINT str = { 0, 0 };
POINT end = { 0, 0 };
POINT tmpDT = { 0, 0 };
// set default color line
COLORREF colorLine = RGB(0, 0, 0);


// device context for prog
HDC tmpHDC;
HDC memHDC;

// buffer data
HBITMAP bufferMap;
HBITMAP oldBuffMap;

HWND btnPen;

// GUI
Canvas paintList;
Canvas upMenu;
Canvas leftMenu;
Canvas bottomMenu;

Button penBtn;
Button eraseBtn;
Button moveBtn;
Button rotateBtn;
Button scaleBtn;
Button lineBtn;
Button wndLineBtn;
Button triangleBtn;

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

	// register main wnd
	RegisterClassW(&wcMain);

	// craete window
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

	HWND tmpMenu = leftMenu.getHWND();

	/*BUTTON*/
	// create pen button
	penBtn.createWnd( tmpMenu, hInstance, 2, 2, 30, 30, L"", 10, IDB_BITMAP1);
	// create erase button
	eraseBtn.createWnd( tmpMenu, hInstance, 34, 2, 30, 30, L"", 20, IDB_BITMAP2);
	// create move btn
	moveBtn.createWnd( tmpMenu, hInstance, 2, 34, 30, 30, L"", 30, IDB_BITMAP3);
	// create rotate btn
	rotateBtn.createWnd( tmpMenu, hInstance, 34, 34, 30, 30, L"Rot", 40, NULL);
	// create scale btn
	scaleBtn.createWnd( tmpMenu, hInstance, 2, 66, 30, 30, L"Scl", 50, NULL);
	// create line btn
	lineBtn.createWnd( tmpMenu, hInstance, 2, 98, 30, 62, L"WuLine", 60, NULL);
	// craete windows line btn
	wndLineBtn.createWnd( tmpMenu, hInstance, 2, 130, 30, 62, L"WndLine", 70, NULL);
	// create triangle btn
	triangleBtn.createWnd( tmpMenu, hInstance, 2, 162, 30, 30, L"/\\", 80, NULL);

	tmpMenu = upMenu.getHWND();
	
	/* MENU_BUTTON */
	// create file btn
	fileBtn.createWnd( tmpMenu, hInstance, 3, 3, 30, 60, L"File", 1, NULL);
	// create edit btn
	editBtn.createWnd( tmpMenu, hInstance, 66, 3, 30, 60, L"Edit", 2, NULL);
	// create view btn
	viewBtn.createWnd( tmpMenu, hInstance, 129, 3, 30, 60, L"View", 3, NULL);
	// create image btn
	imageBtn.createWnd( tmpMenu, hInstance, 192, 3, 30, 60, L"Image", 4, NULL);

	/*CURSOR_ZONE*/
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
		
		// check position and move if that needed
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
	if (flDraw || flCursor == LINE) {
		GetCursorPos(&tmpPos);
		tmp = WindowFromPoint(tmpPos);

		// set flags on false
		if (tmp != paintList.getHWND()) {
			flDraw = false;
			
			if (flLine == true) {
				flLine = false;

				str = { 0, 0 };
				end = { 0, 0 };
			}
		}
	}
	// start paint
	if (hwnd == paintList.getHWND())
		WndPaintCanvasProc(paintList.getHWND(), uMsg, wParam, lParam);

	// send message WM_PAINT
	InvalidateRect(paintList.getHWND(), NULL, FALSE);

	switch (uMsg){
	case WM_CREATE: {
			// create memHDC for drawing and "double buffer"
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
				break;

			// create eraser
			case 20:
				if (flCursor != ERASER) {
					hPen = CreatePen(PS_SOLID, 30, RGB(255, 255, 255));
					flCursor = ERASER;
					tmpPen = hPen;

					PostMessage(paintList.getHWND(), WM_SETCURSOR, NULL, NULL);
				}
				break;
			// move
			case 30:
				if (flCursor != MOVE)
					flCursor = MOVE;
				else
					flCursor &= ~MOVE;

				// set data in coordinate matrix
				posPnt.matrix[2][0] = 1.0;

				break;
			// rotate
			case 40:
				if (flCursor != ROTATE)
					flCursor = ROTATE;
				else
					flCursor &= ~ROTATE;

				// set data in coordinate matrix
				posPnt.matrix[2][0] = 1.0;

				break;
			// scale
			case 50:
				if (flCursor != SCALE)
					flCursor = SCALE;
				else
					flCursor = ~SCALE;

				// set data in coordinate matrix
				posPnt.matrix[2][0] = 1.0;

				break;
			// draw wu line
			case 60:
				if (flCursor != LINE) {
					flCursor = LINE;

					flFigure = NULL;
				}
				else
					flCursor = NULL;
				break;
			// draw windows line
			case 70:
				if (flCursor != WND_LINE) {
					flCursor = WND_LINE;
					flFigure = NULL;
				
					hPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 0));
				}
				break;
			case 80:
				if ((flFigure & TRIANGLE) != true) {
					flFigure = TRIANGLE;

					hPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 0));
				}
				else 
					flCursor = NULL;
				break;
			default:
				if (hPen != NULL) {
					DeleteObject(hPen);
					flCursor = NULL;
					hPen = NULL;
				}
			}

		}
		break;

	// reaction on resizing wnd
	case WM_SIZE:
		// update interface
		reUpFunc(lParam);
		break;

	// paint in wnd
	case WM_PAINT:
		// start paint
		paintListHDC = BeginPaint(paintList.getHWND(), &ps);

		// copy memory into main hdc
		BitBlt( paintListHDC, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			memHDC, rect.left, rect.top, SRCCOPY);

		// end paint
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

	// get client size in rectangle
	GetClientRect(paintList.getHWND(), &rect);

	switch (uMsg)
	{
	case WM_SETCURSOR:
		// set new pen
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

		if(flDraw == false && (flCursor == PEN || flCursor == ERASER || flCursor == WND_LINE))
			flDraw = true;

		// start draw
		if (flDraw == true && (flCursor == PEN || flCursor == ERASER)) {
			// set new pen and save old pen
			oldPen = (HPEN) SelectObject(memHDC, hPen);

			// update pixel and draw
			MoveToEx(memHDC, posCursor.x, posCursor.y, NULL);
			LineTo(memHDC, posCursor.x, posCursor.y);

			// send message WM_PAINT
			InvalidateRect(paintList.getHWND(), NULL, FALSE);
			break;
		}

		// set flag line as true
		flLine = true;

		str.x = posCursor.x;
		str.y = posCursor.y;
		end = str;

		// create classic line
		if (flCursor == WND_LINE) {
			MoveToEx( memHDC, posCursor.x, posCursor.y, NULL);
			// draw line
			wndLine.wndLine(memHDC, str, end, colorLine);

			// send message WM_PAINT
			InvalidateRect(paintList.getHWND(), NULL, FALSE);
			break;
		}

		// create Line
		if (flCursor == LINE) {
			// draw line
			wuLine.wuLine( memHDC, str, end, colorLine);
			// send message WM_PAINT
			InvalidateRect(paintList.getHWND(), NULL, FALSE);
			break;
		}

		// move command
		if (flCursor == MOVE || flCursor == SCALE || flCursor == ROTATE) {
			flDraw = true;

			// save new coord
			tmpDT.x = posCursor.x;
			tmpDT.y = posCursor.y;

			// save new coord
			tmpDt_thread.x = LOWORD(tmp_lParam);
			tmpDt_thread.y = HIWORD(tmp_lParam);
		}

		// create triangle
		if (flFigure == TRIANGLE && flDraw == false) {
			// save position
			trFigure.savePntPos( posCursor, numPnt);

			numPnt++;
		}
		break;
	case WM_LBUTTONUP:
		// stop drawing
		if (flDraw == true)
			flDraw = false;

		// stop draw lines
		if (flLine == true) {
			flLine = false;
		
			str = { 0, 0};
			end = { 0, 0};
		}

		// if now three positions
		if (numPnt == 3) {
			numPnt = 0;
			trFigure.displayTriangle(memHDC, colorLine);

			InvalidateRect(paintList.getHWND(), NULL, FALSE);
		}

		// restore old object
		SelectObject(memHDC, oldPen);

		break;
	case WM_MOUSEMOVE:
		// get cursor pos
		if (flDraw == true && (flCursor == PEN || flCursor == ERASER)){
			// update current pixel
			MoveToEx(memHDC, posCursor.x, posCursor.y, NULL);
			// draw line
			LineTo(memHDC, posCursor.x = LOWORD(lParam), posCursor.y = HIWORD(lParam));

			// send message WM_PAINT
			InvalidateRect(paintList.getHWND(), NULL, FALSE);
			break;
		}

		end.x = LOWORD(lParam);
		end.y = HIWORD(lParam);

		// draw classic windows line
		if (flDraw == true && flCursor == WND_LINE) {
			// update pixel and draw
			wndLine.deleteLine(memHDC, 0x02);
			// update current pixel
			MoveToEx(memHDC, posCursor.x, posCursor.y, NULL);
			// draw line
			wndLine.wndLine(memHDC, str, end, colorLine);

			// send message WM_PAINT
			InvalidateRect(paintList.getHWND(), NULL, FALSE);
			break;
		}

		// create Line
		if (flCursor == LINE && flLine == true) {
			// delete line
			wuLine.deleteLine( memHDC);
			wuLine.wuLine( memHDC, str, end, colorLine);
			// send message WM_PAINT
			InvalidateRect(paintList.getHWND(), NULL, FALSE);
			break;
		}

		// transform line
		if (((flCursor & MOVE) || (flCursor & SCALE) || (flCursor & ROTATE)) && flDraw == true) {
			// calc new point
			if (flFigure == NULL) {
				wuLine.calcNewPoint(lParam, flCursor, memHDC, tmpDT);
				wuLine.wuLine(memHDC, colorLine);

				// calc new point fow windows line
				// wndLine.calcNewPoint(lParam, flCursor, memHDC, tmpDT);
				// wndLine.wndLine(memHDC, colorLine);

				// send message WM_PAINT
				InvalidateRect(paintList.getHWND(), NULL, FALSE);
			}
			else if (flFigure == TRIANGLE) {
				//tmp_lParam = lParam;

				//if (flThread == false) {
					//threadTr = CreateThread(NULL, NULL, TriangleThread, NULL, NULL, NULL);
					//flThread = true;
				//}

				if (trFigure.getFl())
					trFigure.fillTriangle(memHDC, RGB(255, 255, 255), RGB(255, 255, 255));
				else
					trFigure.setFl(true);

				trFigure.transformTrFig(lParam, flCursor, memHDC, tmpDT);
				trFigure.displayTriangle(memHDC, colorLine);

				flThread = false;

				// send message WM_PAINT
				InvalidateRect(paintList.getHWND(), NULL, FALSE);
			}

			// save new coord
			tmpDT.x = LOWORD(lParam);
			tmpDT.y = HIWORD(lParam);
		}
		break;
	}
}