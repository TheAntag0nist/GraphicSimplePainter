#include "Line.h"

// overload func
// draw point
void Line::drawPnt( HDC& memHDC, float gradient, POINT pnt, float intense, COLORREF color) {
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
void Line::drawPnt(HDC& memHDC, float gradient, int x, int y, float intense, COLORREF color) {
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
int Line::brLine(HDC& memHDC, POINT startP, POINT endP, COLORREF colorLine) {
	// check line points
	this->startPoint = startP;
	this->endPoint = endP;
	this->typeLine = 0x01;

	if (startP.x == endP.x && startP.y == endP.y) {
		SetPixel(memHDC, startP.x, startP.y, colorLine);
		return NULL;
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
int Line::brLine(HDC& memHDC, COLORREF colorLine) {
	return brLine( memHDC, this->getStart(), this->getEnd(), colorLine);
}

// Wu Line
int Line::wuLine(HDC& memHDC, POINT startP, POINT endP, COLORREF colorLine) {
	// save angle
	if (createFl == false) {
		float gipot = abs(sqrt((endPoint.y - startPoint.y) ^ 2 + (endPoint.x - startPoint.x) ^ 2));

		cosA = abs((endPoint.x - startPoint.x) / gipot);
		sinA = (float)1 - cosA * cosA;

		createFl = true;
	}

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
int Line::wuLine(HDC& memHDC, COLORREF colorLine) {
	return wuLine( memHDC, this->getStart(), this->getEnd(), colorLine);
}

// windows Line
int Line::wndLine(HDC& memHDC, POINT startP, POINT endP, COLORREF colorLine) {
	// delete line
	this->deleteLine(memHDC);

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
int Line::wndLine(HDC& memHDC, COLORREF colorLine) {
	return wndLine(memHDC, this->getStart(), this->getEnd(), colorLine);
}

// calc new POINTS
void Line::calcNewPoint(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT, char flFigure, float xCenter, float yCenter) {
	POINT deltaPos;
	float scaleCoeff = static_cast<float>(110) / 100;
	double dX;
	double dY;

	if (flFigure == 0x06) {
		dX = (static_cast<double>(this->getEnd().x) + this->getStart().x) / 2;
		dY = (static_cast<double>(this->getEnd().y) + this->getStart().y) / 2;
	}
	else {
		dX = xCenter;
		dY = yCenter;
	}

	POINT& newStr = this->startPoint;
	POINT& newEnd = this->endPoint;
	
	// rotation parametrs
	// 1 degree or not 1
	float cosFi = 0.9998476952;
	float sinFi = 0.01745240644;

	transformMove.oneM();
	transformRotate.oneM();
	transformScale.oneM();
	//====================================
	// delete line
	this->deleteLine(memHDC, typeLine);
	//====================================
	// delta from start
	deltaPos.x = LOWORD(lParam) - tmpDT.x;
	deltaPos.y = HIWORD(lParam) - tmpDT.y;

	// set data in transform matrix
	if (flCursor == MOVE) {
		// transform matrix
		transformMove.matrix[0][2] = deltaPos.x;
		transformMove.matrix[1][2] = deltaPos.y;
	}
	else if (flCursor == SCALE) {
		if (deltaPos.x > 0) {
			transformScale.matrix[0][0] = scaleCoeff;
			transformScale.matrix[1][1] = transformScale.matrix[0][0];
		}
		else {
			transformScale.matrix[0][0] = 1 / scaleCoeff;
			transformScale.matrix[1][1] = transformScale.matrix[0][0];
		}
	}
	else if (flCursor == ROTATE) {
		// 5 grad
		cosFi = 0.996194;

		if (deltaPos.x > 0)
			sinFi = 0.087155;
		else
			sinFi = -(0.087155);

		transformRotate.matrix[0][0] = cosFi;
		transformRotate.matrix[0][1] = -sinFi;
		transformRotate.matrix[1][0] = sinFi;
		transformRotate.matrix[1][1] = cosFi;
	}

	//--------------------------------------------------
	// START POINT
	transformPoint(flCursor, newStr, dX, dY);
	//--------------------------------------------------
	// END POINT
	transformPoint(flCursor, newEnd, dX, dY);
}

// calc new POINT_
void Line::transformPoint(char flCursor, POINT& pnt, double dX, double dY) {
	Matrix<float>* pointerM;

	// zero all matrix
	posPnt.zeroM();
	resPnt_1.zeroM();
	resPnt_2.zeroM();

	transformTmp.zeroM();
	transformTmp_1.zeroM();
	transformRes.zeroM();

	transformMatr.zeroM();

	// save point str
	posPnt.matrix[0][0] = pnt.x;
	posPnt.matrix[1][0] = pnt.y;
	posPnt.matrix[2][0] = 1.0;

	// change position
	if (flCursor == SCALE || flCursor == ROTATE) {
		transformMove.matrix[0][2] = -dX;
		transformMove.matrix[1][2] = -dY;
	}

	// transform
	transformMove.multi(posPnt, resPnt_1);
	transformRotate.multi(resPnt_1, resPnt_2);
	resPnt_1.zeroM();
	transformScale.multi(resPnt_2, resPnt_1);
	pointerM = &resPnt_1;

	// restore center
	if (flCursor == SCALE || flCursor == ROTATE) {
		transformMove.matrix[0][2] = dX;
		transformMove.matrix[1][2] = dY;

		resPnt_2.zeroM();
		transformMove.multi(resPnt_1, resPnt_2);
		pointerM = &resPnt_2;
	}

	// new pnt
	pnt.x = static_cast<long>(round(pointerM->matrix[0][0]));
	pnt.y = static_cast<long>(round(pointerM->matrix[1][0]));
}