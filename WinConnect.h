#pragma once 


struct DrawBuffer{
	int width;    // in dwords
	int height;
	int pitch;
	void *pData;
};



void onKey( unsigned int keyCode,int action);

void onMouse( int x, int y, int action);

void onDraw( DrawBuffer *pDBuff);

bool gameStep(unsigned int tick);

