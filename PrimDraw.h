#pragma once
#include "WinConnect.h"
void DrawRect( int l, int t, int r, int b, int Color, DrawBuffer *pBuff);

void DrawString(char *pSrt,int x, int y);

void DrawEmptyRect(int l, int t, int r, int b, int Color, DrawBuffer *pBuff);

int toScreenX(int x, int mapW);

int toScreenY(int y, int mapH);