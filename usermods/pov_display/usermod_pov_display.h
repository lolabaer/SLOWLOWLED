#pragma once
#include "wled.h"
#include <PNGdec.h>
#include <AnimatedGIF.h>
#define BRI 3
#define FX_MODE_POV_IMAGE 255
static const char _data_FX_MODE_POV_IMAGE[] PROGMEM = "POV Image@!;;;1";

AnimatedGIF gif;
PNG png;
File f;

typedef struct file_tag
{
    int32_t iPos; // current file position
    int32_t iSize; // file size
    uint8_t *pData; // memory file pointer
    void * fHandle; // class pointer to File/SdFat or whatever you want
} IMGFILE;

void * openFile(const char *filename, int32_t *size) {
    f = WLED_FS.open(filename);
    *size = f.size();
    return &f;
}

void closeFile(void *handle) {
    if (f) f.close();
}

int32_t readFile(IMGFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
	iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
	return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
}

int32_t readPNG(PNGFILE *pFile, uint8_t *pBuf, int32_t iLen)
{ return readFile((IMGFILE*) pFile, pBuf, iLen); }
    
int32_t readGIF(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{ return readFile((IMGFILE*) pFile, pBuf, iLen); }

int32_t seekFile(IMGFILE *pFile, int32_t iPosition)
{
    int i = micros();
    File *f = static_cast<File *>(pFile->fHandle);
    f->seek(iPosition);
    pFile->iPos = (int32_t)f->position();
    i = micros() - i;
    return pFile->iPos;
}

int32_t seekPNG(PNGFILE *pFile, int32_t iPos)
{ return seekFile((IMGFILE*) pFile, iPos); }
    
int32_t seekGIF(GIFFILE *pFile, int32_t iPos)
{ return seekFile((IMGFILE*) pFile, iPos); }

void pngDraw(PNGDRAW *pDraw) {
    uint16_t usPixels[SEGLEN];
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
    for(int x=0; x < SEGLEN; x++) {
	uint16_t color = usPixels[x];
	byte r = ((color >> 11) & 0x1F);
	byte g = ((color >> 5) & 0x3F);
	byte b = (color & 0x1F);
	SEGMENT.setPixelColor(x, RGBW32(r,g,b,0));
    }
    busses.show();
}

void gifDraw(GIFDRAW *pDraw) {
    uint8_t r, g, b, *s, *p, *pPal = (uint8_t *)pDraw->pPalette;
    int x, y = pDraw->iY + pDraw->y;
    
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) {
	p = &pPal[pDraw->ucBackground * 3];
	r = p[0]; g = p[1]; b = p[2];
	for (x=0; x<pDraw->iWidth; x++)
	{
	    if (s[x] == pDraw->ucTransparent) {
		SEGMENT.setPixelColor(x, RGBW32(r, g, b, 0));
	    }
	}
	pDraw->ucHasTransparency = 0;
    }

    if (pDraw->ucHasTransparency) {
	const uint8_t ucTransparent = pDraw->ucTransparent;
	for (x=0; x<pDraw->iWidth; x++)	{
	    if (s[x] != ucTransparent) {
		p = &pPal[s[x] * 3];
		SEGMENT.setPixelColor(x, RGBW32(p[0]>>BRI, p[1]>>BRI, p[2]>>BRI, 0));
	    }
	}
    }

    else // no transparency, just copy them all
    {
	for (x=0; x<pDraw->iWidth; x++)
	{
	    p = &pPal[s[x] * 3];
	    SEGMENT.setPixelColor(x, RGBW32(p[0], p[1], p[2], 0));
	}
    }
    busses.show();
}

void pov_image() {
    const char * filepath = SEGMENT.name;
    int rc = png.open(filepath, openFile, closeFile, readPNG, seekPNG, pngDraw);
    if (rc == PNG_SUCCESS) {
	if (png.getWidth() != SEGLEN) return;
	rc = png.decode(NULL, 0);
	png.close();
	return;
    }

    gif.begin(GIF_PALETTE_RGB888);
    rc = gif.open(filepath, openFile, closeFile, readGIF, seekGIF, gifDraw);
    if (rc) {
	if (gif.getCanvasWidth() != SEGLEN) return;
	while (gif.playFrame(true, NULL)) {}
	gif.close();
    }
}

uint16_t mode_pov_image(void) {
    pov_image();
    return FRAMETIME;
}

class PovDisplayUsermod : public Usermod
{
  protected:
	bool enabled = false; //WLEDMM
	bool initDone = false; //WLEDMM
	unsigned long lastTime = 0; //WLEDMM

  public:
    void setup() {
	strip.addEffect(FX_MODE_POV_IMAGE, &mode_pov_image, _data_FX_MODE_POV_IMAGE);
	initDone=true;
    }

    void loop() {
	if (!enabled || strip.isUpdating()) return;
	if (millis() - lastTime > 1000) {
	    lastTime = millis();
	}
    }

    void connected() {}
};
