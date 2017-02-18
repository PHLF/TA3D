/* header image file format (GREY): arrowdata.h  */

static unsigned int width = 16;
static unsigned int height = 16;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = pixel[1] = pixel[2] = (unsigned char)*data; \
++data; \
}

static const unsigned char header_data[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,8,95,0,0,0,0
	,0,0,0,0,0,0,0,0,32,137,236,248,0,0,0,0
	,0,0,0,0,0,1,72,179,253,216,115,20,0,0,0,0
	,0,0,0,0,0,219,248,165,63,0,0,0,0,0,0,0
	,0,0,0,0,0,219,244,158,57,0,0,0,0,0,0,0
	,0,0,0,0,0,1,73,180,253,213,113,20,0,0,0,0
	,0,0,0,0,0,0,0,0,33,138,236,249,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,8,96,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
 };
