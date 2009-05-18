#pragma once

extern "C" {
#include "../jpeg/jpeglib.h"
}

class CScreenshot
{
private:

	IDirect3DDevice8 *m_pD3DDevice;

	HANDLE CreateDibFromCurrentSurface();
	void FreeDib(HANDLE hDib);

	BOOL CreateJpegFileFromDib(HANDLE hDib, int nQuality, PCHAR szJpegFile);
	void ConvertDibToSamps(HANDLE hDib,int nSampsPerRow,
		struct jpeg_compress_struct cinfo,JSAMPARRAY jsmpPixels);

public:

	BOOL TakeScreenShot(PCHAR szFileName);
	CScreenshot(IDirect3DDevice8 *pD3DDevice);
	~CScreenshot() {};
};


//----------------------------------------------------