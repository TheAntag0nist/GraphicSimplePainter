#include "Line.h"

// overload func
// draw point
void Line::drawPnt( HDC &memHDC, float gradient, POINT pnt, float intense, COLORREF color) {
	COLORREF colorTmp = color;
	float inten = 1 - intense;
	int add = ((255 - static_cast<int>(color & 0x000000ff)) / 10);

	// set color
	if (color != RGB(255, 255, 255)) {
		colorTmp += (colorTmp & 0x000000ff) + (add * (int)round(inten * 10));
		colorTmp += (colorTmp & 0x0000ff00) + ((add * (int)round(inten * 10)) << 8);
		colorTmp += (colorTmp & 0x00ff0000) + ((add * (int)round(inten * 10)) << 16);
	}

	// change points
	if (gradient > 1)
		swap(pnt);

	SetPixel( memHDC, pnt.x, pnt.y, color);
}
void Line::drawPnt(HDC &memHDC, float gradient, int x, int y, float intense, COLORREF color) {
	COLORREF colorTmp = color;
	float inten = 1 - intense;
	int add = ((255 - static_cast<int>(color & 0x000000ff)) / 10);

	// set color
	if (color != RGB(255, 255, 255)) {
		colorTmp += (colorTmp & 0x000000ff) + (add * (int)round(inten * 10));
		colorTmp += (colorTmp & 0x0000ff00) + ((add * (int)round(inten * 10)) << 8);
		colorTmp += (colorTmp & 0x00ff0000) + ((add * (int)round(inten * 10)) << 16);
	}

	if (gradient > 1)
		swap( x, y);

	SetPixel(memHDC, x, y, colorTmp);
}

// Brezenhem line
int Line::brLine(HDC &memHDC, POINT startP, POINT endP, COLORREF colorLine) {
	// check line points
	this->startPoint = startP;
	this->endPoint = endP;
	this->typeLine = 0x01;

	if (startP.x == endP.x && startP.y == endP.y) {
		SetPixel(memHDC, startP.x, startP.y, colorLine);
	}

	float deltaX = abs((static_cast<float>(endP.x) - startP.x));
	float deltaY = abs((static_cast<float>(endP.y) - startP.y));
	// get steep coefficient
	float steep = deltaY / deltaX;

	if (steep > 1) {
		swap(startP);
		swap(endP);
	}

	if (startP.x > endP.x) {
		swap(startP.x, endP.x);
		swap(startP.y, endP.y);
	}

	drawPnt(memHDC, steep, startP, 1, colorLine);
	drawPnt(memHDC, steep, endP, 1, colorLine);

	float dx = endP.x - startP.x;
	float dy = endP.y - startP.y;
	float gradient = dy / dx;
	float y = startP.y + gradient;

	for (int x = startP.x + 1; x <= endP.x - 1; ++x) {
		drawPnt(memHDC, steep, x, (int)y, 1, colorLine);
		y += gradient;
	}

	return NULL;
}

// Wu Line
int Line::wuLine(HDC &memHDC, POINT startP, POINT endP, COLORREF colorLine) {
	// check line points
	this->startPoint = startP;
	this->endPoint = endP;
	this->typeLine = 0x00;

	if (startP.x == endP.x && startP.y == endP.y) {
		SetPixel(memHDC, startP.x, startP.y, colorLine);
		return NULL;
	}

	float deltaX = abs((static_cast<float>(endP.x) - startP.x));
	float deltaY = abs((static_cast<float>(endP.y) - startP.y));
	// get steep coefficient
	float steep = deltaY / deltaX;

	// if angle so large swap
	if (steep > 1){
		swap( startP);
		swap( endP);
	}

	// if start position lower than end position on X
	if (startP.x > endP.x){
		swap( startP.x, endP.x);
		swap( startP.y, endP.y);
	}

	// draw start and end
	drawPnt( memHDC, steep, startP, 1, colorLine);
	drawPnt(memHDC, steep, endP, 1, colorLine);

	float dx = endP.x - startP.x;
	float dy = endP.y - startP.y;
	float gradient = dy / dx;
	float y = startP.y + gradient;

	// draw line
	for (int x = startP.x + 1; x <= endP.x - 1; ++x) {
		drawPnt( memHDC, steep, x, (int)y, 1 - (y - (int)y), colorLine);
		drawPnt( memHDC, steep, x, (int)y + 1, y - (int)y, colorLine);
		y += gradient;
	}

	return NULL;
}

// windows Line
int Line::wndLine(HDC& memHDC, POINT startP, POINT endP, COLORREF colorLine) {
	// check line points
	this->startPoint = startP;
	this->endPoint = endP;
	this->typeLine = 0x02;

	// create pen
	HPEN pen = CreatePen( PS_SOLID, 1, colorLine);
	HPEN oldPen = (HPEN) SelectObject( memHDC, pen);

	// draw line
	MoveToEx(memHDC, startP.x, startP.y, NULL);
	LineTo(memHDC, startP.x, startP.y);
	LineTo(memHDC, endP.x, endP.y);

	// change pen and delete
	SelectObject( memHDC, oldPen);
	DeleteObject(oldPen);
	DeleteObject(pen);

	return NULL;
}

// calc new POINTS
void Line::calcNewPoint(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT) {
	// delete line
	this->deleteLine(memHDC, typeLine);

	//====================================
	POINT deltaPos;

	// calc new positions
	POINT newStr = { 0,0 };
	POINT newEnd = { 0,0 };

	// calc center
	POINT oldCenter = { 0,0 };
	POINT newCenter = { 0,0 };

	POINT& tmpStr = this->startPoint;
	POINT& tmpEnd = this->endPoint;

	// rotation parametrs
	float cosFi = 0.996194;
	float sinFi = 0.087155;

	oldCenter = this->getCenter();

	//====================================
	// calc new str
	newStr.x -= oldCenter.x;
	newStr.y -= oldCenter.y;

	// calc new end
	newEnd.x -= oldCenter.x;
	newEnd.y -= oldCenter.y;
	//====================================

	resPnt_1.zeroM();

	// delta from start
	deltaPos.x = LOWORD(lParam) - tmpDT.x;
	deltaPos.y = HIWORD(lParam) - tmpDT.y;

	// set data in transform matrix
	if (flCursor == MOVE) {
		// transform matrix
		transformMatr.zeroM();
		transformMatr.matrix[0][2] = deltaPos.x;
		transformMatr.matrix[1][2] = deltaPos.y;
	}
	else if (flCursor == SCALE) {
		transformMatr.zeroM();

		if (deltaPos.x > 0) {
			transformMatr.matrix[0][0] = 1.05;
			transformMatr.matrix[1][1] = 1.05;
		}
		else {
			transformMatr.matrix[0][0] = 0.95;
			transformMatr.matrix[1][1] = 0.95;
		}
	}
	else if (flCursor == ROTATE) {
		// transform matrix
		transformMatr.zeroM();

		// 5 grad
		cosFi = 0.996194;

		if (deltaPos.x > 0)
			sinFi = 0.087155;
		else
			sinFi = -(0.087155);

		transformMatr.matrix[0][0] = cosFi;
		transformMatr.matrix[0][1] = -sinFi;
		transformMatr.matrix[1][0] = sinFi;
		transformMatr.matrix[1][1] = cosFi;
	}

	//--------------------------------------------------
	// save point str
	posPnt.matrix[0][0] = tmpStr.x;
	posPnt.matrix[1][0] = tmpStr.y;
	posPnt.matrix[2][0] = 1.0;

	// calc new pos
	transformMatr.multi(posPnt, resPnt_1);

	tmpStr.x = round(resPnt_1.matrix[0][0] - 0.5);
	tmpStr.y = round(resPnt_1.matrix[1][0] - 0.5);

	this->setStart(tmpStr);
	//--------------------------------------------------
	resPnt_1.zeroM();

	// save point str
	posPnt.matrix[0][0] = tmpEnd.x;
	posPnt.matrix[1][0] = tmpEnd.y;
	posPnt.matrix[2][0] = 1.0;

	// calc new pos
	transformMatr.multi(posPnt, resPnt_1);

	tmpEnd.x = round(resPnt_1.matrix[0][0] - 0.5);
	tmpEnd.y = round(resPnt_1.matrix[1][0] - 0.5);

	this->setEnd(tmpEnd);

	// if not move
	if (flCursor == SCALE || flCursor == ROTATE) {
		newCenter = this->getCenter();

		newCenter.x -= oldCenter.x;
		newCenter.y -= oldCenter.y;

		tmpStr.x -= newCenter.x;
		tmpStr.y -= newCenter.y;

		tmpEnd.x -= newCenter.x;
		tmpEnd.y -= newCenter.y;
	}
}