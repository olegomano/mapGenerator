
#include "stdafx.h"
#include"stdio.h"
#include "WinConnect.h"
#include"PrimDraw.h"
//#include "C:\Users\Oleg Laptop\Downloads\DbgGsWindow\mapGen\Ship.h"
extern void drawCellMap(DrawBuffer *pDBuff);
struct gameScene
{
	int rPosx;
	int rPosy;
	int mouseX;
    int mouseY;
	int vel;
	gameScene() :rPosx(0),rPosy(0),mouseX(0),mouseY(0),vel(5){}
	void onKey(unsigned int keyCode,int action)
	{
		switch(keyCode){
		case 37:  rPosx-=vel ; break;
		case 38:  rPosy-=vel; break;
		case 39:  rPosx+=vel; break;
		case 40:  rPosy+=vel; break;
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