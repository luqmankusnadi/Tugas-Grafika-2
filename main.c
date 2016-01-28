#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "framebuffer.h"
#include "bitmap.h"

int main(){
	InitFramebuffer();
	BitmapFile bitmapFile;
	bitmapFile = LoadBitmapFile("exp.bmp");
	BitmapFile bitmapFile2;
	bitmapFile2 = LoadBitmapFile("background.bmp");
	
	int i;
	for(i=0; i < 16; i++){
		ClearScreen();
		DrawBitmap(0,0, &bitmapFile2);
		DrawBitmapAsSprite(100,100, &bitmapFile, 64, 64, i);
		SwapBuffers();
		usleep(20000);
	}
	
	CloseFramebuffer();
	return 0;
}
