#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

typedef struct
{
    uint16_t type;  //specifies the file type
    uint32_t size;  //specifies the size in bytes of the bitmap file
    uint16_t reserved1;  //reserved; must be 0
    uint16_t reserved2;  //reserved; must be 0
    uint32_t offset;  //species the offset in bytes from the BitmapFileHeader to the bitmap bits
}BitmapFileHeader;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct
{
    uint32_t size;  //specifies the number of bytes required by the struct
    int32_t width;  //specifies width in pixels
    int32_t height;  //species height in pixels
    uint16_t planes; //specifies the number of color planes, must be 1
    uint16_t bitCount; //specifies the number of bit per pixel
    uint32_t compression;//spcifies the type of compression
    uint32_t sizeImage;  //size of image in bytes
    int32_t xPelsPerMeter;  //number of pixels per meter in x axis
    int32_t yPelsPerMeter;  //number of pixels per meter in y axis
    uint32_t colorUsed;  //number of colors used by th ebitmap
    uint32_t colorImportant;  //number of colors that are important
}BitmapInfoHeader;

#pragma pack(pop)

typedef struct {
	BitmapInfoHeader info;
	unsigned char *data;
}BitmapFile;

BitmapFile LoadBitmapFile(char *filename);
void FreeBitmap(BitmapFile* bitmapFile);

#endif
