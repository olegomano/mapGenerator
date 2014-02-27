#pragma once
#define UNDEF 9
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

class DrawBuffer;

const int mapW = 200;
const int mapH = 200;
const int maxRoomS = 15;
const int minRoomS = 7;
const int minRoomAmount = 300;
const int maxRoomAmount = 300;

int allocatedCells = 0;
int countX = 0;
int countY = 0;
struct mapCell{
	int x = 0;
	int y = 0;
	int indexOfpCellmap = 0;
	int door = UNDEF; //u,d,l,r,n
	int dir = UNDEF; //u,d,l,r,n
	int ROOM_ID = 0;
	bool isEdge = false;
	mapCell(){
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
	int lastAllocRefArr = 0;
	int allocatedRooms = 0;
	mapWorld(){
		pCellMap = new mapCell[mapW*mapH];
		pCellRoomRefArray = new int[mapW*mapH];
		pMapRoomAr = new mapRoom[(mapW*mapH) / (minRoomS*minRoomS)];
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
	~mapWorld(){
		delete[] pCellMap;
		delete[] pCellRoomRefArray;
		delete[] pMapRoomAr;
		//free(pMyints);
	}
};

void drawCellMap(DrawBuffer *pDBuff);






