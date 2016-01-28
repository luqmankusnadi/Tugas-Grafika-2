
#include "bitmap.h"

BitmapFile LoadBitmapFile(char *filename)
{
    FILE *filePtr; //our file pointer
    BitmapFileHeader bitmapFileHeader; //our bitmap file header
    BitmapFile bitmapFile;
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return bitmapFile;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BitmapFileHeader),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type !=0x4D42)
    {
        fclose(filePtr);
        return bitmapFile;
    }

    //read the bitmap info header
    fread(&bitmapFile.info, sizeof(BitmapInfoHeader),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(sizeof(unsigned char)*bitmapFile.info.width*bitmapFile.info.height*bitmapFile.info.bitCount/8);
	if(bitmapFile.info.bitCount == 24){
		int rowPadded = (bitmapFile.info.width*3 + 3) & (~3);
		unsigned char* rowData = (unsigned char*)malloc(sizeof(unsigned char)*rowPadded);
		
		int i,j;
		for(j = 0; j < bitmapFile.info.height; j++)
		{
			fread(rowData, sizeof(unsigned char), rowPadded, filePtr);
			for(i = 0; i < bitmapFile.info.width*3; i += 3)
			{
				int p = j*bitmapFile.info.width*3+i;
				bitmapImage[p] = rowData[i+2];
				bitmapImage[p+1] = rowData[i+1];
				bitmapImage[p+2] = rowData[i+0];
			}
		}
		free(rowData);
	} else {
		free(bitmapImage);
		return bitmapFile;
	}
    printf("%d\n", bitmapFile.info.sizeImage);
    //close file and return bitmap image data
    fclose(filePtr);
    bitmapFile.info = bitmapFile.info;
    bitmapFile.data = bitmapImage;
    return bitmapFile;
}

void FreeBitmap(BitmapFile* bitmapFile){
	free(bitmapFile->data);
}
