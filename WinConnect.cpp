
#include "stdafx.h"
#include"stdio.h"
#include "WinConnect.h"
#include"PrimDraw.h"
//#include "C:\Users\Oleg Laptop\Downloads\DbgGsWindow\mapGen\Ship.h"
extern void drawCellMap(DrawBuffer *pDBuff);

//int xOffset = 0;
//int yOffset = 0;

struct gameScene
{
	int xOffsetScroll;
	int yOffsetScroll;
	int xCenter = 100;
	int yCenter = 100;
	float zoomFactor;
	int mouseX;
    int mouseY;
	int vel;
	gameScene() :xOffsetScroll(0),yOffsetScroll(0),mouseX(0),mouseY(0),vel(1),zoomFactor(6){}
	void onKey(unsigned int keyCode,int action)
	{
		switch(keyCode){
		case 37: xOffsetScroll -= vel; break;
		case 38: yOffsetScroll -= vel; break;
		case 39: xOffsetScroll += vel; break;
		case 40: yOffsetScroll += vel; break;
		}
	}

	void onWheelScroll(unsigned short scroll){
		short s = scroll;
		if (s == 120){
			zoomFactor+= .7;
		}
		else{
			zoomFactor-= .7;
		}
	}

	void onDraw( DrawBuffer *pDBuff)
	{
		//graphicsDrawMap(pDBuff);
		
	}

};
gameScene gScene;


static int sMouseX = 0, sMouseY = 0;

// 37 left
// 38 up
// 39 right
// 40 down
void onKey( unsigned int keyCode,int action)
{
	gScene.onKey(keyCode,action);
}

void onMouse( int x, int y, int action)
{

}

void onWheelScroll(unsigned short scroll){
	gScene.onWheelScroll(scroll);
}

// when WM_PAINT happens
void onDraw( DrawBuffer *pDBuff)
{
	static int frame = 0;
	
	//DrawRect( gScene.rPosx, gScene.rPosy, 100+gScene.rPosx, 100 + gScene.rPosy , 0xFF0000FF, pDBuff);
	drawCellMap(pDBuff);
	{
		static char StrOut[256];
		sprintf(StrOut,"Frame: %d ",frame++);
		DrawString(StrOut,2,2);
		sprintf(StrOut,"Mouse: %d %d ",sMouseX,sMouseY);
		DrawString(StrOut,2,30);
	}
}

int toScreenX(int x, int centX){//DECLARED IN PRIMDRAW.H
	return x = ((x + gScene.xOffsetScroll)*gScene.zoomFactor) + (WINDOW_WIDTH / 2) - centX*gScene.zoomFactor;
}


int toScreenY(int y, int centY){//DECLARED IN PRIMDRAW.H
	return y = ((y + gScene.yOffsetScroll)*gScene.zoomFactor) + (WINDOW_HEIGHT / 2) - centY*gScene.zoomFactor;
}


// Returns true to force onDraw;
bool gameStep(unsigned int tick)
{
	static unsigned int sLastTick = 0;
	unsigned  int timeInt =  tick- sLastTick;
	sLastTick = tick;
	if(timeInt<16){  // 16ms ~ 60 frames per second
		return false; // do not redraw
	} 
	// Game Step here

	return true; // redraw
}