#pragma once
#include "Triangle.h"

void Triangle::displayTriangle(HDC memHDC, COLORREF colorLine) {
	// display triangle
	ln_1.wuLine( memHDC, colorLine);
	ln_2.wuLine( memHDC, colorLine);
	ln_3.wuLine( memHDC, colorLine);
}

void Triangle::savePntPos(POINT pnt, char numPnt) {
	// save position
	if (numPnt == 0x00) {
		ln_1.setStart(pnt);
		ln_3.setEnd(pnt);
	}
	else if (numPnt == 0x01) {
		ln_1.setEnd(pnt);
		ln_2.setStart(pnt);
	}
	else if (numPnt == 0x02) {
		ln_2.setEnd(pnt);
		ln_3.setStart(pnt);
	}
}

// transform triangle
void Triangle::transformTrFig(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT) {
	// calc center
	float centerX = static_cast<float>(ln_1.getStart().x + ln_2.getStart().x + ln_3.getStart().x) / 3;
	float centerY = static_cast<float>(ln_1.getStart().y + ln_2.getStart().y + ln_3.getStart().y) / 3;

	ln_1.calcNewPoint(lParam, flCursor, memHDC, tmpDT, NULL, centerX, centerY);
	ln_2.calcNewPoint(lParam, flCursor, memHDC, tmpDT, NULL, centerX, centerY);
	ln_3.calcNewPoint(lParam, flCursor, memHDC, tmpDT, NULL, centerX, centerY);

	// not necessary
	// ln_1.setStart(ln_3.getEnd());
	// ln_2.setStart(ln_1.getEnd());
	// ln_3.setStart(ln_2.getEnd());
}