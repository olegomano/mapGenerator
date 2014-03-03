#pragma once
#define UNDEF 9
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

class DrawBuffer;

const int mapW = 6000;
const int mapH = 6000;
const int maxRoomS = 11;
const int minRoomS = 3;
const int minRoomAmount = 5000;
const int maxRoomAmount = 15000;

int allocatedCells = 0;
int countX = 0;
int countY = 0;
struct mapCell{
	int x;
	int y;
	int indexOfpCellmap;
	int door; //u,d,l,r,n
	int dir; //u,d,l,r,n
	int ROOM_ID;
	bool isEdge;
	mapCell(){
		x = 0;
		y = 0;
		indexOfpCellmap = 0;
		door = UNDEF;
		dir = UNDEF;
		isEdge = false;
		ROOM_ID = 0;
		indexOfpCellmap = allocatedCells;
		if (countX == mapW){
			countX = 0;
			countY++;
		}
		x = countX;
		y = countY;
		allocatedCells++;
	};
};
 

struct mapRoom{
	int pStartInRefAr = 0;
	int roomNum = 0;
	int length = 0;
	int width = 0;
	int hieght = 0;
	int siblingRoomAr[4]; //contains index of sibling room in room array	
};

struct mapWorld{
	
	mapCell* pCellMap;
	mapRoom* pMapRoomAr;
	int* pCellRoomRefArray;
	int lastAllocRefArr;
	int allocatedRooms;
	mapWorld(){
		allocMap();
		lastAllocRefArr = 0;
		allocatedRooms = 0;
		allocateCellMap();
	}
	mapCell* getCellFromIndex(int index);
	mapCell* getCellFrom2dCoor(int x, int y);
	mapCell* getCellFromRefArr(int index);
	mapCell* getCellFromRoomArr(int roomN, int index);
	void allocateCellMap();
	void printMap();
	void graphicsDrawMap(DrawBuffer *pDBuff); //Callbadck from windows
	void createDoors();
	
	//mapCell* pMyints = (int*)malloc(100 * sizeof(mapCell));
	bool checkAndAllocMemmory(int startX, int startY, int lenght, int times, int roomN);
	void createRoom(int roomNumber, int xStart, int yStart, int orientation); //orientation = room is horizontal or vertical in relationship to the room that created it
	void createChildRoom(int parentRoomIndex, int doorDir, int roomNumber, int childNum);
	void addCellToRoom(int index, int roomNumber);
	void resetMap(){
		lastAllocRefArr = 0;
		allocatedRooms = 0;
		allocatedCells = 0;
		countX = 0;
		countY = 0;
		delete[] pCellMap;
		delete[] pCellRoomRefArray;
		delete[] pMapRoomAr;
		allocMap();
		allocateCellMap();
		/*
		for (int i = 0; i < mapW*mapH; i++){
			pCellMap[i].dir = UNDEF;
			pCellMap[i].indexOfpCellmap = 0;
			pCellMap[i].isEdge = false;
			pCellMap[i].ROOM_ID = 0;
			pCellRoomRefArray[i] = 0;
		}
		for (int i = 0; i < (mapW*mapH) / (minRoomS*minRoomS); i++){
			pMapRoomAr[i].hieght = 0;
			pMapRoomAr[i].length = 0;
			pMapRoomAr[i].pStartInRefAr = 0;
			pMapRoomAr[i].roomNum = 0;
			pMapRoomAr[i].width = 0;
		}
		mapCell* pCellMap = 0;
		mapRoom* pMapRoomAr = 0;
		int* pCellRoomRefArray = 0;
		int lastAllocRefArr = 0;
		int allocatedRooms = 0;
		*/
	}
	void allocMap(){
		pCellMap = new mapCell[mapW*mapH];
		pCellRoomRefArray = new int[mapW*mapH];
		pMapRoomAr = new mapRoom[(mapW*mapH) / (minRoomS*minRoomS)];
	}
	~mapWorld(){
		delete[] pCellMap;
		delete[] pCellRoomRefArray;
		delete[] pMapRoomAr;
		//free(pMyints);
	}
};



void drawCellMap(DrawBuffer *pDBuff);






