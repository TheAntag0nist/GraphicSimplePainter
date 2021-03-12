#pragma once
#include <Windows.h>
#include "Matrix.h"
#include <cmath>

static Matrix<float> transformMatr(3, 3);
static Matrix<float> transformResMatr(3, 3);
static Matrix<float> posPnt(3, 1);
static Matrix<float> resPnt_1(3, 1);
static Matrix<float> resPnt_2(3, 1);

#define MOVE 0x03
#define ROTATE 0x04
#define SCALE 0x05

class Line{
private:
	POINT startPoint;
	POINT endPoint;

	char typeLine;

	// draw point functions
	void drawPnt(HDC &memHDC, float gradient, POINT pnt, float intense, COLORREF color);
	void drawPnt(HDC &memHDC, float gradient, int x, int y, float intense, COLORREF color);

	// swap data in POINT
	void swap(POINT& pnt) {
		pnt.x += pnt.y;
		pnt.y = pnt.x - pnt.y;
		pnt.x -= pnt.y;
	}

	// swap functions
	void swap(long& x0, long& x1) {
		x0 += x1;
		x1 = x0 - x1;
		x0 -= x1;
	}
	void swap(int& x0, int& x1) {
		x0 += x1;
		x1 = x0 - x1;
		x0 -= x1;
	}
	void swap(float& x0, float& x1) {
		x0 += x1;
		x1 = x0 - x1;
		x0 -= x1;
	}
public:
	// constructors
	Line() {
		startPoint.x = NULL;
		startPoint.y = NULL;

		endPoint.x = NULL;
		endPoint.y = NULL;
	};
	Line(POINT start, POINT end) {
		startPoint = start;
		endPoint = end;
	}

	POINT getCenter() {
		POINT center;
			  center.x = (int)((startPoint.x + endPoint.x) / 2);
			  center.y = (int)((startPoint.y + endPoint.y) / 2);

		return center;
	}

	POINT getStart() {
		return startPoint;
	}
	POINT getEnd() {
		return endPoint;
	}

	void setStart(POINT str) {
		startPoint = str;
	}
	void setEnd(POINT end) {
		endPoint = end;
	}

	// line that delete/erase line
	int deleteLine(HDC memHDC, char type = NULL) {
		// set white color
		COLORREF whiteLine = RGB(255, 255, 255);

		// paint white line
		if (type == 0x00)
			this->wuLine( memHDC, startPoint, endPoint, whiteLine);
		else if (type == 0x01)
			this->brLine( memHDC, startPoint, endPoint, whiteLine);
		else if (type == 0x02)
			this->wndLine( memHDC, startPoint, endPoint, whiteLine);

		return NULL;
	}

	// func that draws lines
	int wuLine(HDC &memHDC, POINT startP, POINT endP, COLORREF colorLine);
	int brLine(HDC &memHDC, POINT startP, POINT endP, COLORREF colorLine);
	int wndLine(HDC& memHDC, POINT startP, POINT endP, COLORREF colorLine);

	void calcNewPoint(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT);
};
