#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include "bitmap.h"

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color32;

int fbfd;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize;
char *fbp;
char *bbp;

void InitFramebuffer();
void SwapBuffers();
void CloseFramebuffer();
void ClearScreen();
void SetPixel(int x, int y, Color32 color);
void DrawBitmap(int x, int y, const BitmapFile *bitmapFile);
void DrawSubBitmap(int x, int y, const BitmapFile *bitmapFile, int offsetX, int offsetY, int width, int height);
void DrawBitmapAsSprite(int x, int y, const BitmapFile *bitmapFile, int width, int height, int index);

#endif
