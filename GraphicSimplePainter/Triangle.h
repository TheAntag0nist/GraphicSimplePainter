#pragma once
#include "Line.h"
#include "Stack.h"

// stack for fill triangle
static Stack<node<POINT>> fillStackPnt;

class Triangle {
private:
	Line ln_1;
	Line ln_2;
	Line ln_3;

	// fill functions
	// bad ideas
	void fillPnt(HDC memHDC, COLORREF colorPnt, COLORREF colorSpace, int xPos, int yPos);
	void fillCrossroad(HDC memHDC, COLORREF colorPNT, int xPos, int yPos);
	void myFill(HDC memHDC, COLORREF colorPnt, int xPos, int yPos);

	// stack idea
	void fillStack(HDC memHDC, COLORREF colorPnt, COLORREF colorBorder, int xPos, int yPos);
	void fillStack_lines(HDC memHDC, COLORREF colorPnt, COLORREF colorBorder, int xPos, int yPos);
public:
	// fill flag
	char fillFl;

	Triangle() {
		fillFl = false;
	}

	char getFl() {
		return fillFl;
	}
	void setFl(char fl) {
		fillFl = fl;
	}

	// display triangle
	void displayTriangle(HDC memHDC, COLORREF colorLine);

	// save position
	void savePntPos(POINT pnt, char numPnt);

	// transform triangle
	void transformTrFig(LPARAM lParam, char flCursor, HDC& memHDC, POINT tmpDT);

	// fill triangle
	void fillTriangle(HDC& memHDC, COLORREF colorLine, COLORREF colorBorder);

	// delete all lines
	void deleteLines(HDC memHDC, char fl = 0x00);
};