// spaceShipGenerator.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <time.h> 
#include "Ship.h"
#include "C:\Users\Oleg Laptop\Downloads\DbgGsWindow\PrimDraw.h"

static void debugTrap(){

}
#define CHECK_ERR(_exp) if(_exp){ printf("ERROR!! %s line= =%d\n", #_exp, __LINE__); debugTrap();}

mapWorld gMap;
int countLeft = 0;
int countRight = 0;
int countUp = 0;
int countDown = 0;
int generateUniqueRand(int maxRand, int minRand, int* invalidValues, int invalidValLength, int count){
	if (count > 8){
		return invalidValues[rand() % invalidValLength];
	}
	int rand = std::rand() % maxRand + minRand;
	for (int i = 0; i < invalidValLength; i++){
		if (rand == invalidValues[i]){
			generateUniqueRand(maxRand, minRand, invalidValues, invalidValLength, ++count);
		}
	}
	return rand;
}

void generateMap()
{
	srand(time(NULL));
	//std::cout << "HELLO WORLD" << std::endl;
	gMap.createRoom(1, mapW /2 , mapH /2 , rand() % 4);
	gMap.createDoors();
	//gMap.printMap();
	//std::cout << "Down " << countDown << " " << DOWN  << std::endl;
	//std::cout << "Up " << countUp << " " << UP << std::endl;
	//std::cout << "Left " << countLeft << " " << LEFT << std::endl;
	//std::cout << "Right " << countRight << " " << RIGHT << std::endl;
	
}
bool isOppositeDir(int dir1, int dir2){
	if (dir1 == UP && dir2 == DOWN){
		return true;
	}
	else if (dir2 == UP && dir1 == DOWN){
		return true;
	}
	else if (dir1 == LEFT && dir2 == RIGHT){
		return true;
	}
	else if (dir1 == RIGHT && dir2 == LEFT){
		return true;
	}
	return false;
}
bool hasCommonWall(mapRoom* room1, mapRoom* room2){
	int commonWallCount = 0;
	CHECK_ERR(room1->length == 0 || room2->length == 0);
	int *pCommonCellsIndexArrRoom1 = new int[room1->length * room2->length];
	int *pCommonCellsIndexArrRoom2 = new int[room1->length * room2->length];

	for (int i = 0; i < room1->length; i++){
		for (int b = 0; b < room2->length; b++){
			int dirRoom1 = gMap.getCellFromRoomArr(room1->roomNum, i)->dir;
			int dirRoom2 = gMap.getCellFromRoomArr(room2->roomNum, b)->dir;
			int dif = 0;
			if (isOppositeDir(dirRoom1, dirRoom2)){
				if (dirRoom1 == RIGHT || dirRoom1 == LEFT){
					dif = gMap.getCellFromRoomArr(room1->roomNum, i)->x - gMap.getCellFromRoomArr(room2->roomNum, b)->x;
				}
				else{
					dif = gMap.getCellFromRoomArr(room1->roomNum, i)->y - gMap.getCellFromRoomArr(room2->roomNum, b)->y;
				}
			}
			if (dif == 1 || dif == -1){
				pCommonCellsIndexArrRoom1[commonWallCount] = i;
				pCommonCellsIndexArrRoom2[commonWallCount] = b;
				commonWallCount++;
			}
		}
	}
	if (commonWallCount >= 3){
		for (int i = 0; i < commonWallCount; i++){
			if (i == commonWallCount % 2){
				int indexRoom1 = pCommonCellsIndexArrRoom1[i];
				int indexRoom2 = pCommonCellsIndexArrRoom2[i];
				gMap.getCellFromRoomArr(room1->roomNum, indexRoom1)->isEdge = false;
				gMap.getCellFromRoomArr(room2->roomNum, indexRoom2)->isEdge = false;
			}
		}
	}
	delete[]pCommonCellsIndexArrRoom1;
	delete[]pCommonCellsIndexArrRoom2;
	return true;
}

void removeCommonWall(mapRoom* room){
	mapCell **pCellToRemove = new mapCell*[room->length];
	int cellsToRemove = 0;
	int countUp = 0;
	int countDown = 0;
	int countLeft = 0;
	int countRight = 0;
	for (int i = 0; i < room->length; i++){
		mapCell *pMapCell = gMap.getCellFromRoomArr(room->roomNum, i);
		switch (pMapCell->dir)
		{
		case UP:
			if (pMapCell->y == 0){
				break;
			}
			if (gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y - 1)->dir == DOWN){
				//gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y - 1)->isEdge = false;
				mapCell* pDebugCell = gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y - 1);
				pCellToRemove[cellsToRemove] = gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y);
				countUp++;
				cellsToRemove++;
			}
			break;
		case DOWN:
			if (pMapCell->y == mapH){
				break;
			}
			if (gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y + 1)->dir == UP){
				//gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y + 1)->isEdge = false;
				pCellToRemove[cellsToRemove] = gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y);
				countDown++;
				cellsToRemove++;
			}
			break;
		case LEFT:
			if (pMapCell->x == 0){
				break;
			}
			if (gMap.getCellFrom2dCoor(pMapCell->x - 1, pMapCell->y)->dir == RIGHT){
				//gMap.getCellFrom2dCoor(pMapCell->x - 1, pMapCell->y)->isEdge = false;
				pCellToRemove[cellsToRemove] = gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y);
				countLeft++;
				cellsToRemove++;
			}
			break;
		case RIGHT:
			if (pMapCell->x == mapW){
				break;
			}
			if (gMap.getCellFrom2dCoor(pMapCell->x + 1, pMapCell->y)->dir == LEFT){
				//gMap.getCellFrom2dCoor(pMapCell->x + 1, pMapCell->y)->isEdge = false;
				pCellToRemove[cellsToRemove] = gMap.getCellFrom2dCoor(pMapCell->x, pMapCell->y);
				countRight++;
				cellsToRemove++;
			}
			break;
		default:
			break;
		}
	}
	int countRUp = 0;
	int countRDown = 0;
	int countRLeft = 0;
	int countRRight = 0;
	//int doorSUp = ( countUp - 2 ) - ( rand() % (countUp - 2));

	for (int i =  0; i < cellsToRemove ; i++){
		if (pCellToRemove[i]->dir == DOWN){
			if (countRDown >= 1 && countRDown < countDown - 1){
				pCellToRemove[i]->isEdge = false;
			}
			countRDown++;
		}
		else if (pCellToRemove[i]->dir == LEFT){
			if (countRLeft >= 1 && countRLeft < countLeft - 1){
				pCellToRemove[i]->isEdge = false;
			}
			countRLeft++;
		}
		else if (pCellToRemove[i]->dir == RIGHT){
			if (countRRight >= 1 && countRRight < countRight - 1){
				pCellToRemove[i]->isEdge = false;
			}
			countRRight++;
		}
		else if (pCellToRemove[i]->dir == UP){
			if (countRUp >= 1 && countRUp < countUp - 1 && countRUp){
				pCellToRemove[i]->isEdge = false;
			}
			countRUp++;
		}
		//pCellToRemove[i]->isEdge = false;
	}
	delete[] pCellToRemove;
}

void mapWorld::createDoors(){
	for (int i = 0 ; i < allocatedRooms; i++){
		/*
		for (int b = 0; b < allocatedRooms; b++){
			hasCommonWall(pMapRoomAr+i,pMapRoomAr+b);
		}
		*/
		removeCommonWall(&pMapRoomAr[i]);
	}
}



void mapWorld::addCellToRoom(int index, int roomNumber){
	if (lastAllocRefArr >= mapW*mapH){
		std::cout << "CANNON ADD CELL, OUT OF BOUNDRY CELL LENGTH";
		return;
	}
	pCellRoomRefArray[lastAllocRefArr] = index;
	pCellMap[index].ROOM_ID = roomNumber;
	lastAllocRefArr++;
};

void mapWorld::createRoom(int roomNumber, int xStart, int yStart, int orientation){
	//xStart and yStart come in centered
	//Generate Demensions
	if (allocatedRooms >= maxRoomAmount){
		return;
	}
	CHECK_ERR(orientation > 3);
	//Hueristic start
	int roomH = 0;
	int roomW = 0;
	//roomH = rand() % (maxRoomS) + minRoomS;
	//roomW = rand() % (maxRoomS) + minRoomS;
	if (orientation == UP || orientation == DOWN){
		if (allocatedRooms % 2 == 0){
			roomW = rand() % (maxRoomS - minRoomS * 2) + minRoomS;
			roomH = roomW * 3;
		}
		else{
			roomW = rand() % (maxRoomS) + minRoomS;
			roomH = rand() % minRoomS + roomW;
		}

	}
	else{
		if (allocatedRooms % 2 == 0){
			roomH = rand() % (maxRoomS - minRoomS * 2) + minRoomS;
			roomW = roomH * 3;
		}
		else{
			roomH = rand() % (maxRoomS)+minRoomS;
			roomW = rand() % minRoomS + roomH;
		}
	}
	/**
	if (allocatedRooms % 2 == 0){
		if (orientation == UP || orientation == DOWN){
			roomW = rand() % (maxRoomS - minRoomS * 2) + minRoomS;
			roomH = roomW * 2;
		}
		else{
			roomH = rand() % (maxRoomS - minRoomS * 2) + minRoomS;
			roomW = roomH * 2;
		}
	}
	**/
	//Hueristic end
	int dir = orientation;
	if (dir == UP || dir == DOWN){
		xStart = xStart - (roomW / 2);
	}
	else{
		yStart = yStart - (roomH / 2);
	}
	int doorC = rand() % 4 + 1;
	if (allocatedRooms <=minRoomAmount ){
		doorC = 4;
	}
	switch (dir)
	{
	case RIGHT:
		if (!checkAndAllocMemmory(xStart , yStart , roomW, roomH, roomNumber)){
			return;
		}
		countRight++;
		break;
	case LEFT:
		if (!checkAndAllocMemmory(xStart - roomW + 1 , yStart, roomW, roomH, roomNumber)){
			return;
		}
		countLeft++;
		break;
	case UP:
		if (!checkAndAllocMemmory(xStart , yStart - roomH + 1,roomW, roomH, roomNumber)){
			return;
		}
		countUp++;
		break;
	case DOWN:
		if (!checkAndAllocMemmory(xStart, yStart , roomW, roomH, roomNumber)){
			return;
		}
		countDown++;
		break;
	default:
		return;
	}
	std::cout << roomH << " x " << roomW << " in dir " << dir << " start at " << xStart << " " << yStart << " " << allocatedRooms << std::endl;
	
	int indexInRef = pMapRoomAr[allocatedRooms - 1].pStartInRefAr;
	int indexIncellArr = pCellRoomRefArray[indexInRef];
	int parentRoomIndex = allocatedRooms - 1;
	int dirsAlreadyHad[4];
	int* pDirs = dirsAlreadyHad;
	for (int i = 0; i < doorC; i++){
		int doorDir = generateUniqueRand(4, 0,pDirs,4,0);
		dirsAlreadyHad[i] = doorDir;
		//doorDir = i;
		createChildRoom(parentRoomIndex,doorDir, roomNumber + 1,i);
	}
}


void mapWorld::allocateCellMap(){//Called in Ctor Of mapWorld
	int countX = 0;
	int countY = 0;
	for (int i = 0; i < mapW*mapH; i++){
		pCellMap[i].x = countX;
		pCellMap[i].y = countY;
		//pCellMap[i].ROOM_ID = countX;
		countX++;
		if (countX == mapW){
			countX = 0;
			countY++;
		}
	}
}

bool mapWorld::checkAndAllocMemmory(int startX, int startY, int length, int times, int roomN){
	if (startX < 0 || startY < 0 || startY + times >= mapH || startX + length >= mapW){
		return false;
	}
	
	for (int i = 0; i < times; i++){
		for (int b = 0; b < length; b++){
			int x = startX + b;
			int y = startY + i;
			if (getCellFrom2dCoor(startX + b, startY + i)->ROOM_ID != 0){
				
				return false;
			}
		}
	}
	pMapRoomAr[allocatedRooms].pStartInRefAr = lastAllocRefArr;
	pMapRoomAr[allocatedRooms].length = length*times;
	pMapRoomAr[allocatedRooms].hieght = times;
	pMapRoomAr[allocatedRooms].width = length;
	pMapRoomAr[allocatedRooms].roomNum = allocatedRooms;
	int countX = 0;
	for (int i = 0; i < times; i++){
		int dMemStr = (startY + i)*mapW + startX;
		for (int b = dMemStr; b < dMemStr + length; b++){
			if (i == 0 && b >= dMemStr ){
				if (b < dMemStr + length - 1 && b > dMemStr){
					pCellMap[b].dir = UP;
				}
				pCellMap[b].isEdge = true;
			}
			if (b == (dMemStr + length - 1) && i > 0 ){
				if (i < times - 1){
					pCellMap[b].dir = RIGHT;
				}
				pCellMap[b].isEdge = true;
			}
			if (i == (times - 1) && b > dMemStr){
				if (b < dMemStr + length - 1){
					pCellMap[b].dir = DOWN;
				}
				pCellMap[b].isEdge = true;
			}
			if (countX == 0 && i > 0 ){
				if (i < times - 1){
					pCellMap[b].dir = LEFT;
				}
				pCellMap[b].isEdge = true;
			}
			addCellToRoom(pCellMap[b].indexOfpCellmap,roomN);
			//pCellMap[i].x = startX + countX;
			//pCellMap[i].y = startY + i;
			countX++;
		}
		countX = 0;
	}
	allocatedRooms++;
	return true;
}

void mapWorld::createChildRoom(int parentRoomIndex, int doorDir, int roomNumber, int childNum){
	int count2 = 0;
	int modX = 0;
	int modY = 0;
	int midPoint = (pMapRoomAr[parentRoomIndex].hieght - 2) / 2;
	if (doorDir == UP || doorDir == DOWN){
		modY = -1;
		if (doorDir == DOWN){
			modY = 1;
		}
		midPoint = (pMapRoomAr[parentRoomIndex].width- 2) / 2;
	}
	else{
		modX = 1;
		if (doorDir == LEFT){
			modX = -1;
		}
	}
	
	for (int b = 0; b < pMapRoomAr[parentRoomIndex].length; b++){
		if (getCellFromRoomArr(parentRoomIndex, b)->dir == doorDir){
			if (count2 == midPoint){
				
				pMapRoomAr[parentRoomIndex].siblingRoomAr[childNum] = allocatedRooms;
				createRoom(roomNumber, getCellFromRoomArr(parentRoomIndex, b)->x + modX, getCellFromRoomArr(parentRoomIndex, b)->y + modY, doorDir);
				return;
			}
			count2++;
		}
	}
};

mapCell* mapWorld::getCellFromIndex(int index){
	CHECK_ERR(index >= mapW*mapH);
	CHECK_ERR(index < 0);
	return pCellMap+index;
}
mapCell* mapWorld::getCellFrom2dCoor(int x, int y){
	CHECK_ERR(x >= mapW);
	CHECK_ERR(y >= mapH);
	CHECK_ERR(y < 0);
	CHECK_ERR(x < 0);
	int index = y*mapW + x;
	return getCellFromIndex(index);
}
mapCell* mapWorld::getCellFromRefArr(int index){// Takes the Cells index in the Reference Array
	CHECK_ERR(index > lastAllocRefArr);
	int indexInmapCell = pCellRoomRefArray[index];
	return getCellFromIndex(indexInmapCell);
}
mapCell* mapWorld::getCellFromRoomArr(int roomN, int index){// Takes room Number and Cells position in the Room
	CHECK_ERR(roomN > allocatedRooms);
	int indexRoom = pMapRoomAr[roomN].pStartInRefAr;
	CHECK_ERR(indexRoom + index > lastAllocRefArr);
	int posInRefAr = pCellRoomRefArray[indexRoom + index];
	return getCellFromIndex(posInRefAr);
}

void mapWorld::printMap(){
	int count = 0;
	for (int i = 0; i < mapW*mapH; i++){
		std::cout << pCellMap[i].isEdge;
		count++;
		if (count == mapW){
			count = 0;
			std::cout << std::endl;
		}
	}
}

int cellWidth = 4;
int borderWidth = cellWidth + 1;
void mapWorld::graphicsDrawMap(DrawBuffer *pDBuff){
	int count = 0;
	//DrawRect(100, 100, 100, 100, 0xFF000080, pDBuff);
	for (int i = 0; i < allocatedRooms; i++){
		for (int b = 0; b < pMapRoomAr[i].length; b++){
			int color =  0xFFFFFFFF;
			int xS = getCellFromRoomArr(pMapRoomAr[i].roomNum, b)->x;
			int yS = getCellFromRoomArr(pMapRoomAr[i].roomNum, b)->y;
			DrawRect(xS*cellWidth, yS*cellWidth, xS*cellWidth + cellWidth, yS*cellWidth + cellWidth,color, pDBuff);
		}
	}
	for (int i = 0; i < mapW*mapH; i++){
		if (pCellMap[i].isEdge){
			int color = 0x00000000;
			//DrawRect(pCellMap[i].x * borderWidth, pCellMap[i].y * borderWidth, pCellMap[i].x * borderWidth + borderWidth, pCellMap[i].y * borderWidth + borderWidth, 0xFFFFFFFF, pDBuff);
			DrawRect(pCellMap[i].x * cellWidth, pCellMap[i].y * cellWidth, pCellMap[i].x * cellWidth + cellWidth, pCellMap[i].y * cellWidth + cellWidth,color , pDBuff);
			//char c = 'c';
			//DrawString(&c, pCellMap[i].x * cellWidth, pCellMap[i].y * cellWidth);
		}
		count++;
		if (count == mapW){
			count = 0;
			//std::cout << std::endl;
		}
	}
}

void drawCellMap(DrawBuffer *pDBuff){
	gMap.graphicsDrawMap(pDBuff);
}
