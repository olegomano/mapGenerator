#pragma once 

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 900
struct DrawBuffer{
	int width;    // in dwords
	int height;
	int pitch;
	void *pData;
};



void onKey( unsigned int keyCode,int action);

void onMouse( int x, int y, int action);

void onWheelScroll(unsigned short scroll);

void onDraw( DrawBuffer *pDBuff);

bool gameStep(unsigned int tick);

