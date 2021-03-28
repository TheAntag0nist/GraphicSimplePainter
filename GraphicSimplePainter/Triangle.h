#pragma once
#include "Line.h"

class Triangle {
private:
	Line ln_1;
	Line ln_2;
	Line ln_3;
public:
	// display triangle
	void displayTriangle(HDC memHDC, COLORREF colorLine);

	// save position
	void savePntPos(POINT pnt, char numPnt);

	// transform triangle
	void transformTrFig(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT);
};