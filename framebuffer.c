#include "framebuffer.h"


void InitFramebuffer(){
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");
 
	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}
 
	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}
 
	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
 
	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
 
	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	bbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, (off_t)0);

	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");
}

void SwapBuffers()
{
	memcpy(fbp, bbp, screensize);
}

void CloseFramebuffer(){
	munmap(fbp, screensize);
	close(fbfd);
}

void ClearScreen(){
	int i;
	for(i = 0; i < screensize; i++){
		*(fbp+i) = 0;
	}
}

void SetPixel(int x, int y, Color32 color){
	long int location = 0;
	location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
	if(location >= 0 && location < screensize){
		*((uint32_t*)(bbp + location)) = (color.r<<vinfo.red.offset) | (color.g<<vinfo.green.offset) | (color.b<<vinfo.blue.offset) | (color.a<<vinfo.transp.offset);
	} else {
		//Out of bound
	}
}

void DrawBitmap(int x, int y, const BitmapFile *bitmapFile){
	int i,j;
	int p;
	for(j = 0; j< bitmapFile->info.height; j++){
		for(i = 0; i<bitmapFile->info.width; i++){
			Color32 color;
			p = (bitmapFile->info.width*j+i)*3;
			color.r = bitmapFile->data[p];
			color.g = bitmapFile->data[p+1];
			color.b = bitmapFile->data[p+2];
			color.a = 255;
			if(color.r != 0 && color.g != 0 && color.b != 0)
				SetPixel(x+i,y+bitmapFile->info.height-j-1, color);
		}
	}
}


void DrawSubBitmap(int x, int y, const BitmapFile *bitmapFile, int offsetX, int offsetY, int width, int height){
	int i,j;
	int p;
	int w = bitmapFile->info.width;
	int h = bitmapFile->info.height;
	if(offsetX + width <= w && offsetY + height <= h){
		for(j = 0; j<width; j++){
			for(i = 0; i<height; i++){
				Color32 color;
				p = ((offsetY+j)*w +offsetX+i)*3;
				color.r = bitmapFile->data[p];
				color.g = bitmapFile->data[p+1];
				color.b = bitmapFile->data[p+2];
				color.a = 255;
				if(color.r != 0 && color.g != 0 && color.b != 0)
					SetPixel(x+i,y+height-j-1, color);
			}
		}	
	}
}

void DrawBitmapAsSprite(int x, int y, const BitmapFile *bitmapFile, int width, int height, int index){
	int w = bitmapFile->info.width;
	int h = bitmapFile->info.height;
	int col = w / width;
	int row = h / height;
	int offsetX = (index % col) * width;
	int offsetY = (row - (index / col) - 1)  * height + (h - row*height);
	DrawSubBitmap(x, y, bitmapFile, offsetX, offsetY, width, height);
}
