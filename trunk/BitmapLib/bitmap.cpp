#include <memory.h>
#include "bitmap.h"

bool LoadBMP( const char *pathname, unsigned char *&pixels, 
			 int &width, int &height, int &channels, const int alignment, const bool top2bot )
{
	FILE *fp;
	if ( ( fp = fopen(pathname, "rb" ) ) == NULL )
	{
		printf( "fail to open file %s\n", pathname );
		return false;
	}	

	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	fread( &fileheader.bfType, sizeof( fileheader.bfType ), 1, fp );
	fread( &fileheader.bfSize, sizeof( fileheader.bfSize ), 1, fp );
	fread( &fileheader.bfReserved1, sizeof( fileheader.bfReserved1 ), 1, fp );
	fread( &fileheader.bfReserved2, sizeof( fileheader.bfReserved2 ), 1, fp );
	fread( &fileheader.bfOffBits, sizeof( fileheader.bfOffBits ), 1, fp );
	if (fileheader.bfType != 19778)
	{
		printf( "%s is not a valid Windows Bitmap file!\n", pathname );
		return false;
	}

	fread( &infoheader.biSize, sizeof( infoheader.biSize ), 1, fp );
	fread( &infoheader.biWidth, sizeof( infoheader.biWidth ), 1, fp );
	fread( &infoheader.biHeight, sizeof( infoheader.biHeight ), 1, fp );
	fread( &infoheader.biPlanes, sizeof( infoheader.biPlanes ), 1, fp );
	fread( &infoheader.biBitCount, sizeof( infoheader.biBitCount ), 1, fp );
	fread( &infoheader.biCompression, sizeof( infoheader.biCompression ), 1, fp );
	fread( &infoheader.biSizeImage, sizeof( infoheader.biSizeImage ), 1, fp );
	fread( &infoheader.biXPelsPerMeter, sizeof( infoheader.biXPelsPerMeter ), 1, fp );
	fread( &infoheader.biYPelsPerMeter, sizeof( infoheader.biYPelsPerMeter ), 1, fp );
	fread( &infoheader.biClrUsed, sizeof( infoheader.biClrUsed ), 1, fp );
	fread( &infoheader.biClrImportant, sizeof( infoheader.biClrImportant ), 1, fp );

	if (infoheader.biCompression != 0)
	{
		printf( "%s is not an uncompressed Windows Bitmap!\n", pathname );
		return false;
	}

	height = infoheader.biHeight;
	width = infoheader.biWidth;

	if (infoheader.biBitCount == 8)
	{
		if ( fileheader.bfOffBits != 1078 )
		{ 
			printf( "%s file header doesn't have correct file offset to raster data!", pathname );
			return false;
		}

		unsigned char colortable[1024];
		fread( colortable, sizeof(unsigned char) * 1024, 1, fp );

		channels = 1;

		int padded_w = width;
		padded_w += padding( padded_w, alignment );
		if (pixels==NULL)
		{
			pixels = new unsigned char[height * padded_w];
		}
		//otherwise, assume memory is allocated already.
		
		int buffer_size = width;
		buffer_size += padding( buffer_size, 4 );
		
		unsigned char *buffer = new unsigned char[buffer_size];
		int r;
		unsigned char *row = pixels;
		if (top2bot) row += ( height - 1 ) * padded_w;
		for ( r = 0; r < height; r++ )
		{
			fread( buffer, buffer_size, 1, fp );
			memcpy( row, buffer, sizeof(unsigned char) * width );
			if (top2bot) row -= padded_w;
			else row += padded_w;
		}		
		delete[] buffer;
	}
	else if (infoheader.biBitCount == 24 )
	{
		if ( fileheader.bfOffBits != 54 )
		{ 
			printf( "%s file header doesn't have correct file offset to raster data!", pathname );
			return false;
		}

		channels = 3;

		int padded_w = width * 3;
		padded_w += padding( padded_w, alignment );
		if (pixels==NULL)
		{
			pixels = new unsigned char[height * padded_w];
		}
		//otherwise, assume memory is allocated already.
		
		int buffer_size = 3 * width;
		buffer_size += padding( buffer_size, 4 );
		
		unsigned char *buffer = new unsigned char[buffer_size];
		int r, c;
		unsigned char *row = pixels;
		if (top2bot) row += ( height - 1 ) * padded_w;
		for ( r = 0; r < height; r++ )
		{
			fread( buffer, buffer_size, 1, fp );
			unsigned char *col = row;
			unsigned char *buf = buffer;
			for ( c = 0; c < width; c++ )
			{
				col[0] = buf[2];
				col[1] = buf[1];
				col[2] = buf[0];
				
				col += 3;
				buf += 3;
			}
			if (top2bot) row -= padded_w;
			else row += padded_w;
		}		
		delete[] buffer;
	}
	else
	{
		printf( "not a gray scale or RGB image!\n");
	}		

	fclose( fp );	

	return true;
}

bool SaveBMP( const char *pathname, const unsigned char *pixels, 
			 const int width, const int height, const int channels, const int alignment, const bool top2bot )
{
	FILE *fp;
	if ( ( fp = fopen( pathname, "wb" ) ) == NULL )
	{
		printf( "fail to open file %s\n", pathname );
		return false;
	}	

	if (channels == 1)
	{
		int buffer_size = width;
		buffer_size += padding( buffer_size, 4 );

		BITMAPFILEHEADER fileheader;	
		BITMAPINFOHEADER infoheader;

		fileheader.bfType = 19778;
		fileheader.bfSize = 1078 + buffer_size * height;
		fileheader.bfReserved1 = 0;
		fileheader.bfReserved2 = 0;
		fileheader.bfOffBits = 1078;

		fwrite( &fileheader.bfType, sizeof( fileheader.bfType ), 1, fp );
		fwrite( &fileheader.bfSize, sizeof( fileheader.bfSize ), 1, fp );
		fwrite( &fileheader.bfReserved1, sizeof( fileheader.bfReserved1 ), 1, fp );
		fwrite( &fileheader.bfReserved2, sizeof( fileheader.bfReserved2 ), 1, fp );
		fwrite( &fileheader.bfOffBits, sizeof( fileheader.bfOffBits ), 1, fp );

		infoheader.biSize = 40; 
		infoheader.biWidth = width; 
		infoheader.biHeight = height; 
		infoheader.biPlanes = 1; 
		infoheader.biBitCount = 8;
		infoheader.biCompression = 0; 
		infoheader.biSizeImage = 0; 
		infoheader.biXPelsPerMeter = 0; 
		infoheader.biYPelsPerMeter = 0; 
		infoheader.biClrUsed = 0; 
		infoheader.biClrImportant = 0; 
		
		fwrite( &infoheader.biSize, sizeof( infoheader.biSize ), 1, fp );
		fwrite( &infoheader.biWidth, sizeof( infoheader.biWidth ), 1, fp );
		fwrite( &infoheader.biHeight, sizeof( infoheader.biHeight ), 1, fp );
		fwrite( &infoheader.biPlanes, sizeof( infoheader.biPlanes ), 1, fp );
		fwrite( &infoheader.biBitCount, sizeof( infoheader.biBitCount ), 1, fp );
		fwrite( &infoheader.biCompression, sizeof( infoheader.biCompression ), 1, fp );
		fwrite( &infoheader.biSizeImage, sizeof( infoheader.biSizeImage ), 1, fp );
		fwrite( &infoheader.biXPelsPerMeter, sizeof( infoheader.biXPelsPerMeter ), 1, fp );
		fwrite( &infoheader.biYPelsPerMeter, sizeof( infoheader.biYPelsPerMeter ), 1, fp );
		fwrite( &infoheader.biClrUsed, sizeof( infoheader.biClrUsed ), 1, fp );
		fwrite( &infoheader.biClrImportant, sizeof( infoheader.biClrImportant ), 1, fp );

		unsigned char colortable[256][4];
		for (int i = 0; i < 256; i++)
		{
			colortable[i][0] = i;
			colortable[i][1] = i;
			colortable[i][2] = i;
			colortable[i][3] = 0;
		}
		fwrite( colortable, 1024, 1, fp);

		int padded_w = width;
		padded_w += padding( padded_w, alignment );

		unsigned char *buffer = new unsigned char[buffer_size];
		int r;
		const unsigned char *row = pixels;
		if (top2bot) row += ( height - 1) * padded_w;
		for ( r = 0; r < height; r++ )
		{		
			memcpy(buffer,row, sizeof(unsigned char) * width );
			fwrite( buffer, buffer_size, 1, fp );
			if (top2bot) row -= padded_w;
			else row += padded_w;
		}		
		delete[] buffer;
	}
	else if (channels == 3)
	{
		int buffer_size = 3 * width;
		buffer_size += padding( buffer_size, 4 );

		BITMAPFILEHEADER fileheader;	
		BITMAPINFOHEADER infoheader;

		fileheader.bfType = 19778;
		fileheader.bfSize = 54 + buffer_size * height;
		fileheader.bfReserved1 = 0;
		fileheader.bfReserved2 = 0;
		fileheader.bfOffBits = 54;

		fwrite( &fileheader.bfType, sizeof( fileheader.bfType ), 1, fp );
		fwrite( &fileheader.bfSize, sizeof( fileheader.bfSize ), 1, fp );
		fwrite( &fileheader.bfReserved1, sizeof( fileheader.bfReserved1 ), 1, fp );
		fwrite( &fileheader.bfReserved2, sizeof( fileheader.bfReserved2 ), 1, fp );
		fwrite( &fileheader.bfOffBits, sizeof( fileheader.bfOffBits ), 1, fp );

		infoheader.biSize = 40; 
		infoheader.biWidth = width; 
		infoheader.biHeight = height; 
		infoheader.biPlanes = 1; 
		infoheader.biBitCount = 24;
		infoheader.biCompression = 0; 
		infoheader.biSizeImage = 0; 
		infoheader.biXPelsPerMeter = 0; 
		infoheader.biYPelsPerMeter = 0; 
		infoheader.biClrUsed = 0; 
		infoheader.biClrImportant = 0; 
		
		fwrite( &infoheader.biSize, sizeof( infoheader.biSize ), 1, fp );
		fwrite( &infoheader.biWidth, sizeof( infoheader.biWidth ), 1, fp );
		fwrite( &infoheader.biHeight, sizeof( infoheader.biHeight ), 1, fp );
		fwrite( &infoheader.biPlanes, sizeof( infoheader.biPlanes ), 1, fp );
		fwrite( &infoheader.biBitCount, sizeof( infoheader.biBitCount ), 1, fp );
		fwrite( &infoheader.biCompression, sizeof( infoheader.biCompression ), 1, fp );
		fwrite( &infoheader.biSizeImage, sizeof( infoheader.biSizeImage ), 1, fp );
		fwrite( &infoheader.biXPelsPerMeter, sizeof( infoheader.biXPelsPerMeter ), 1, fp );
		fwrite( &infoheader.biYPelsPerMeter, sizeof( infoheader.biYPelsPerMeter ), 1, fp );
		fwrite( &infoheader.biClrUsed, sizeof( infoheader.biClrUsed ), 1, fp );
		fwrite( &infoheader.biClrImportant, sizeof( infoheader.biClrImportant ), 1, fp );

		int padded_w = width * 3;
		padded_w += padding( padded_w, alignment );

		unsigned char *buffer = new unsigned char[buffer_size];
		int r, c;
		const unsigned char *row = pixels;
		if (top2bot) row += ( height - 1) * padded_w;
		for ( r = 0; r < height; r++ )
		{		
			const unsigned char *col = row;
			unsigned char *buf = buffer;
			for ( c = 0; c < width; c++ )
			{
				buf[2] = col[0];
				buf[1] = col[1];
				buf[0] = col[2];

				col += 3;
				buf += 3;
			}
			fwrite( buffer, buffer_size, 1, fp );

			if (top2bot) row -= padded_w;
			else row += padded_w;
		}		
		delete[] buffer;
	}
	else
	{
		printf( "unsupported color channels!\n" );
		return false;
	}

	fclose( fp );	

	return true;
}
