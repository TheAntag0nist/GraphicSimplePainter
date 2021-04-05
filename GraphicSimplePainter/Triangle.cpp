#pragma once
#include "Triangle.h"

void Triangle::displayTriangle(HDC memHDC, COLORREF colorLine) {
	// paint triangle
	ln_1.brLine( memHDC, colorLine);
	ln_2.brLine( memHDC, colorLine);
	ln_3.brLine( memHDC, colorLine);

	// fill black
	fillTriangle(memHDC, colorLine, RGB(0,0,0));
}

void Triangle::deleteLines(HDC memHDC, char fl){
	if (fl == 0x00) {
		ln_1.deleteLine(memHDC);
		ln_2.deleteLine(memHDC);
		ln_3.deleteLine(memHDC);
	}
	else if (fl == 0x01) {
		ln_1.deleteLine(memHDC, 0x01);
		ln_2.deleteLine(memHDC, 0x01);
		ln_3.deleteLine(memHDC, 0x01);
	}
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

	// calc new points
	ln_1.calcNewPoint(lParam, flCursor, memHDC, tmpDT, NULL, centerX, centerY);
	ln_2.calcNewPoint(lParam, flCursor, memHDC, tmpDT, NULL, centerX, centerY);
	ln_3.calcNewPoint(lParam, flCursor, memHDC, tmpDT, NULL, centerX, centerY);
}

// fill rectangle functions
void Triangle::fillTriangle(HDC& memHDC, COLORREF colorLine, COLORREF colorBorder) {
	// calc center
	// need for good fill
	float centerX = static_cast<float>(ln_1.getStart().x + ln_2.getStart().x + ln_3.getStart().x) / 3;
	float centerY = static_cast<float>(ln_1.getStart().y + ln_2.getStart().y + ln_3.getStart().y) / 3;

	// not work (stack overflow)
	// fillPnt(memHDC, colorLine, colorBorder, static_cast<int>(centerX), static_cast<int> (centerY));
	
	// middle variants (NOT USE!!!! BAD)
	// myFill(memHDC, colorLine, static_cast<int>(centerX), static_cast<int> (centerY));

	// GOOD
	// stack variation
	fillStack_lines(memHDC, colorLine, colorBorder, static_cast<int>(centerX), static_cast<int> (centerY));
}


// good, but can be faster and greater
// NEED UPGRADE
// filling with seed setting
void Triangle::fillStack(HDC memHDC, COLORREF colorPnt, COLORREF colorBorder, int xPos, int yPos) {
	node<POINT> tmp;
	node<POINT> tmp_1;
	SetPixel(memHDC, xPos, yPos, colorPnt);
	
	tmp.data.x = xPos;
	tmp.data.y = yPos;
	tmp.next = NULL;

	fillStackPnt.push_node(tmp);
	
	while (!fillStackPnt.isEmpty()) {
		tmp = fillStackPnt.pop_node();

		tmp_1 = tmp;

		if (GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorPnt)
			SetPixel(memHDC, tmp.data.x, tmp.data.y, colorPnt);

		// right pixel
		if (GetPixel(memHDC, tmp.data.x + 1, tmp.data.y) != colorBorder && GetPixel(memHDC, tmp.data.x + 1, tmp.data.y) != colorPnt) {
			tmp_1.data.x++;
			fillStackPnt.push_node(tmp_1);
			tmp_1 = tmp;
		}

		// up pixel
		if (GetPixel(memHDC, tmp.data.x, tmp.data.y + 1) != colorBorder && GetPixel(memHDC, tmp.data.x, tmp.data.y + 1) != colorPnt) {
			tmp_1.data.y++;
			fillStackPnt.push_node(tmp_1);
			tmp_1 = tmp;
		}

		// left pixel
		if (GetPixel(memHDC, tmp.data.x - 1, tmp.data.y) != colorBorder && GetPixel(memHDC, tmp.data.x - 1, tmp.data.y) != colorPnt) {
			tmp_1.data.x--;
			fillStackPnt.push_node(tmp_1);
			tmp_1 = tmp;
		}

		// down pixel
		if (GetPixel(memHDC, tmp.data.x, tmp.data.y - 1) != colorBorder && GetPixel(memHDC, tmp.data.x, tmp.data.y - 1) != colorPnt) {
			tmp_1.data.y--;
			fillStackPnt.push_node(tmp_1);
			tmp_1 = tmp;
		}
	}
}

void Triangle::fillStack_lines(HDC memHDC, COLORREF colorPnt, COLORREF colorBorder, int xPos, int yPos) {
	node<POINT> tmp;
	node<POINT> tmp_1;

	int tmp_x;
	int rgh_x;
	int lft_x;
	int ent_x;

	char fl;

	tmp.data.x = xPos;
	tmp.data.y = yPos;
	tmp.next = NULL;

	fillStackPnt.push_node(tmp);

	while (!fillStackPnt.isEmpty()) {
		tmp = fillStackPnt.pop_node();

		SetPixel(memHDC, tmp.data.x, tmp.data.y, colorPnt);

		tmp_x = tmp.data.x;
		// fill line
		++tmp.data.x;
		while (GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorBorder) {
			SetPixel(memHDC, tmp.data.x, tmp.data.y, colorPnt);
			++tmp.data.x;
		}

		rgh_x = tmp.data.x - 1;

		tmp.data.x = tmp_x;

		// left side
		--tmp.data.x;
		while (GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorBorder) {
			SetPixel(memHDC, tmp.data.x, tmp.data.y, colorPnt);
			--tmp.data.x;
		}

		lft_x = tmp.data.x + 1;
		tmp.data.x = tmp_x;

		// next line
		tmp.data.x = lft_x;
		++tmp.data.y;

		while (tmp.data.x <= rgh_x) {
			fl = false;

			while (GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorBorder
				&& GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorPnt
				&& tmp.data.x < rgh_x) {
				if (fl == false)
					fl = true;

				++tmp.data.x;
			}

			if (fl == true) {
				if (tmp.data.x == rgh_x && GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorBorder
					&& GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorPnt)
					fillStackPnt.push_node(tmp);
				else {
					--tmp.data.x;
					fillStackPnt.push_node(tmp);
					++tmp.data.x;
				}

				fl = false;
			}

			ent_x = tmp.data.x;
			while (GetPixel(memHDC, tmp.data.x, tmp.data.y) == colorBorder
				&& GetPixel(memHDC, tmp.data.x, tmp.data.y) == colorPnt
				&& tmp.data.x < rgh_x)
				++tmp.data.x;

			if (tmp.data.x = ent_x)
				++tmp.data.x;
		}

		// next line
		tmp.data.x = lft_x;
		--tmp.data.y;
		--tmp.data.y;

		while (tmp.data.x <= rgh_x) {
			fl = false;

			while (GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorBorder
				&& GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorPnt
				&& tmp.data.x < rgh_x) {
				if (fl == false)
					fl = true;

				++tmp.data.x;
			}

			if (fl == true) {
				if (tmp.data.x == rgh_x && GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorBorder
					&& GetPixel(memHDC, tmp.data.x, tmp.data.y) != colorPnt)
					fillStackPnt.push_node(tmp);
				else {
					--tmp.data.x;
					fillStackPnt.push_node(tmp);
					++tmp.data.x;
				}

				fl = false;
			}

			ent_x = tmp.data.x;
			while (GetPixel(memHDC, tmp.data.x, tmp.data.y) == colorBorder
				&& GetPixel(memHDC, tmp.data.x, tmp.data.y) == colorPnt
				&& tmp.data.x < rgh_x)
				++tmp.data.x;

			if (tmp.data.x = ent_x)
				++tmp.data.x;
		}
	}
}

//=====================================================================================================
// recursion (bad, not work with large spaces == stack overflow)
void Triangle::fillPnt(HDC memHDC, COLORREF colorPnt, COLORREF colorSpace, int xPos, int yPos) {
	// main pixel
	SetPixel(memHDC, xPos, yPos, colorPnt);

	// other pixels
	// up
	if (GetPixel(memHDC, xPos, yPos + 1) == colorSpace)
		fillPnt(memHDC, colorPnt, colorSpace, xPos, yPos + 1);
	// right
	if (GetPixel(memHDC, xPos + 1, yPos) == colorSpace)
		fillPnt(memHDC, colorPnt, colorSpace, xPos + 1, yPos);
	// down
	if (GetPixel(memHDC, xPos, yPos - 1) == colorSpace)
		fillPnt(memHDC, colorPnt, colorSpace, xPos, yPos - 1);
	// left
	if (GetPixel(memHDC, xPos - 1, yPos) == colorSpace)
		fillPnt(memHDC, colorPnt, colorSpace, xPos - 1, yPos);
}