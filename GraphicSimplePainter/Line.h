#pragma once
#include <Windows.h>
#include "Matrix.h"
#include <cmath>

static Matrix<float> transformMatr(3, 3);
static Matrix<float> transformTmp(3,3);
static Matrix<float> transformTmp_1(3, 3);
static Matrix<float> transformRes(3, 3);

static Matrix<float> posPnt(3, 1);
static Matrix<float> resPnt_1(3, 1);
static Matrix<float> resPnt_2(3, 1);

static Matrix<float> transformMove(3, 3);
static Matrix<float> transformRotate(3, 3);
static Matrix<float> transformScale(3, 3);

#define MOVE 0x03
#define ROTATE 0x04
#define SCALE 0x05

class Line{
private:
	POINT startPoint;
	POINT endPoint;

	// type of line flag
	char typeLine;

	// create flag
	char createFl;
	
	// fix scale
	float cosA;
	float sinA;

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

		// scale fix
		createFl = false;

		cosA = 0;
		sinA = 0;
	};
	Line(POINT start, POINT end) {
		startPoint = start;
		endPoint = end;

		// scale fix
		createFl = true;

		float gipot = abs(sqrt((endPoint.y - startPoint.y) ^ 2 + (endPoint.x - startPoint.x) ^ 2));

		cosA = abs((endPoint.x - startPoint.x) / gipot);
		sinA = (float)1 - cosA * cosA;
	}

	//get center in int
	POINT getCenter() {
		POINT center;
			  center.x = (int)((startPoint.x + endPoint.x) / 2);
			  center.y = (int)((startPoint.y + endPoint.y) / 2);

		return center;
	}

	// get pos
	POINT getStart() {
		return startPoint;
	}
	POINT getEnd() {
		return endPoint;
	}

	float getCos() {
		return this->cosA;
	}
	float getSin() {
		return this->sinA;
	}

	// set pos
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
	int wuLine(HDC &memHDC, COLORREF colorLine);

	// other functions
	int brLine(HDC &memHDC, POINT startP, POINT endP, COLORREF colorLine);
	int brLine(HDC& memHDC, COLORREF colorLine);

	int wndLine(HDC& memHDC, POINT startP, POINT endP, COLORREF colorLine);
	int wndLine(HDC& memHDC, COLORREF colorLine);

	// transform position
	void calcNewPoint(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT, char flFigure = 0x06, float xCenter = NULL, float yCenter = NULL);
	void transformPoint(char flCursor, POINT& pnt, double dX, double dY);
};