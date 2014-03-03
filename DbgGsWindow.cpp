// DbgGsWindow.cpp : Defines the entry point for the application.
//



#if 0
static unsigned char AAA[1024*1024*4];
COPYDATASTRUCT MyCDS;

	MyCDS.lpData = AAA;
	MyCDS.dwData = 1234;
	MyCDS.cbData = 1024*1024*4;//size

	HWND wnd =  GetForegroundWindow(); 

	HWND parW = FindWindow("DBGGSWINDOW","DbgGsWindow");
	if( parW != NULL){
		SendMessage( parW,
					 WM_COPYDATA,
					 (WPARAM)(HWND) NULL,
					 (LPARAM) (LPVOID) &MyCDS );
	}
 
	return;
#endif	


#include "stdafx.h"
#include "DbgGsWindow.h"
#include "WinConnect.h"
//#include <windows.h>
#include <stdio.h>
#include "C:\Users\Oleg Laptop\Downloads\DbgGsWindow\ShipWinConnect.h"

typedef  unsigned int u32;

int sLeft = 0,sTop = 0,sWidth = WINDOW_WIDTH,sHeight = WINDOW_HEIGHT;
unsigned int * GPUGSLIB_MyVramViewBuf;
static WCHAR sLMViewClassName[256];
static unsigned char *LMViewBuff = NULL;
static HWND sLMViewHwnd;
static PCOPYDATASTRUCT pMyCDS;
static void *pDataToDaw = NULL;
static int n_call =0;
static DrawBuffer sDrawBuff;
extern void generateMap();

typedef struct _DIB_TYPE 
{
	BITMAPINFOHEADER bh;
	WORD colors[256];
	unsigned char* pixels;
} DIB_TYPE;

static DIB_TYPE Dbf;


static void InitDib(DIB_TYPE* Dbf, int W, int H)
{
      int i;
      Dbf->bh.biSize = sizeof(BITMAPINFOHEADER);
      Dbf->bh.biWidth  = W;
      Dbf->bh.biHeight = -H;
      Dbf->bh.biPlanes = 1;
      Dbf->bh.biBitCount = 32;
      Dbf->bh.biCompression = BI_RGB;
      Dbf->bh.biSizeImage = 0;
      Dbf->bh.biXPelsPerMeter=0;
      Dbf->bh.biYPelsPerMeter=0;
      Dbf->bh.biClrUsed=0;
      Dbf->bh.biClrImportant=0;
      Dbf->pixels = NULL;
      for(i = 0; i<256; i++) Dbf->colors[i] = (WORD)i;
	  return;
} /* InitDib() */


#define MaxRecords 128
struct StringRecord{
	char Str[1024];
	int len;
	int x;
	int y;
};

static StringRecord sRecors[MaxRecords];
static int sCurrRecord = 0;

static void DrawStringInternal(char *pStr,HDC hd, int x, int y,int len )
{
	WCHAR pWideString[256];
	MultiByteToWideChar(CP_ACP,0,pStr,-1,pWideString,256);
	TextOut (hd,
             x,
             y,
             pWideString,
             len
			);
}

static void DrawBuff(void) 
{
	HDC hd;
	Dbf.bh.biWidth  = sWidth,
	Dbf.bh.biHeight = -sHeight;
	Dbf.bh.biBitCount = 32;
	hd = GetDC(sLMViewHwnd);
	SetDIBitsToDevice(hd,0,0,sWidth,sHeight,0,0,0,sHeight,(void*)LMViewBuff,(BITMAPINFO *)(&Dbf),DIB_RGB_COLORS);  
	for(int i = 0; i<sCurrRecord; i++){
		DrawStringInternal(sRecors[i].Str,hd,sRecors[i].x,sRecors[i].y, sRecors[i].len);
	}
	ReleaseDC(sLMViewHwnd, hd);
	sCurrRecord = 0;
	return;
} /* DrawBuff() */


// Add pStr to the Record
void DrawString(char *pSrt, int x,int y )
{
	if(sCurrRecord >= MaxRecords){
		return;
	}
	int len = strlen(pSrt);
	if(len> 1023) len = 1023;
	if(len>0){
		StringRecord *pCurr =  sRecors +  sCurrRecord;
		memcpy(pCurr->Str,pSrt,len+1);
		pCurr->len = len;
		pCurr->x = x;
		pCurr->y = y;
		sCurrRecord++;
	}
}


static void FillBuffer(unsigned char *pSrcPix)
{
	static int n_call = 0;
	n_call++;
	unsigned char *psrc, *pdst;
	int n_pix, sz = sWidth * sHeight;
	HDC  dc;
	HBRUSH hbrOld;
	HBRUSH hbr;
	if(!sLMViewHwnd) return;

	
	for(n_pix=0, psrc = (unsigned char*)pSrcPix, pdst = LMViewBuff; n_pix < sz; n_pix++)
	{
		pdst[0] =  0xFF; // blue 
		pdst[1] =  0;
		pdst[2] =  0;
		pdst[3] =  0;
		psrc += 4;
		pdst += 4;
	}

}



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DBGGSWINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	generateMap();
	if (!InitInstance (hInstance, nCmdShow))
	{
		
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DBGGSWINDOW));


#if 0
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#endif 

	// Main message loop:
	PeekMessage(&msg,NULL,0,0,PM_NOREMOVE);
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // No message to do
		{
			if(sLMViewHwnd != NULL){ 
				DWORD tick =  GetTickCount();
				if(  gameStep(tick)== true){
					InvalidateRect(sLMViewHwnd,NULL,false);
				}
			}
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DBGGSWINDOW));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DBGGSWINDOW);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable


   u32 sz = sWidth * sHeight;
   GPUGSLIB_MyVramViewBuf = new u32[sz];
   u32* px = GPUGSLIB_MyVramViewBuf;
   for(int k = 0; k < sz; k++, px++)
   {
	   *px = 0x80FF00FF;
   }


   hWnd = CreateWindow(szWindowClass, 
	                   szTitle, 
	                   WS_OVERLAPPEDWINDOW,
                       0, 
					   0, 
					   sWidth, 
					   sHeight+50, 
					   NULL, 
					   NULL, 
					   hInstance,
					   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   sLMViewHwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   HDC dc, cdc;
   HBITMAP bmp;
   InitDib(&Dbf, sWidth, sHeight);
   dc = GetDC(hWnd);
   cdc = CreateCompatibleDC(dc);
   bmp = CreateDIBSection(cdc, (BITMAPINFO*)&Dbf, DIB_RGB_COLORS, (void**)&LMViewBuff, NULL, 0);
   SelectObject(cdc, bmp);
   ReleaseDC(hWnd, dc);


   sDrawBuff.pData = (void*)LMViewBuff;
   sDrawBuff.width  = sWidth;
   sDrawBuff.pitch  = sWidth;
   sDrawBuff.height = sHeight;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_REGEN:
			regenMap();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_KEYDOWN:
		 onKey( wParam,0);
	break;

	case WM_KEYUP:
		onKey( wParam,1);
	break;
	case WM_MOUSEWHEEL://MOUSEWHEEL SCROLL
		onWheelScroll(GET_WHEEL_DELTA_WPARAM(wParam));
	break;
	case WM_ERASEBKGND :
		return 0;
	break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	
		if(sDrawBuff.pData != NULL){
			//clear first
			unsigned int* pB = (unsigned int*)LMViewBuff;
			for(int i = 0; i<sWidth*sHeight; i++){
				*pB++ = 0x00000080;
			}
			onDraw( &sDrawBuff);
			DrawBuff();
		}
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


