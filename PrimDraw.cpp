#include "stdafx.h"
#include "PrimDraw.h"


void DrawRect( int l, int t, int r, int b, int Color, DrawBuffer *pBuff)
{
	// clip
	int ll = (l<0) ? 0:l;
	int tt = (t<0) ? 0:t;
	int rr = (r>pBuff->width) ? pBuff->width: r;
	int bb = (b>pBuff->height) ? pBuff->height: b;


	if( (rr<ll) || (bb<tt) ) return; 

	if (ll >= pBuff->width) return;
	if (tt >= pBuff->height) return;

	unsigned int *pPix  =(unsigned int *) pBuff->pData;
    int w = rr - ll;
	int h = bb - tt;

	for(int y = 0; y< h; y++){
		unsigned int *pDest = pPix + ll +  (y+ tt)*pBuff->width;
		for(int x = 0; x<w; x++,pDest++){
			*pDest = Color;
		}
	}

}

void DrawEmptyRect(int l, int t, int r, int b, int Color, DrawBuffer *pBuff){

}

