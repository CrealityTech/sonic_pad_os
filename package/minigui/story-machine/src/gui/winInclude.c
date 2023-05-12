#include "winInclude.h"

void unloadBitMap(BITMAP *bmp)
{
	if (bmp->bmBits != NULL) {
		UnloadBitmap(bmp);
		bmp->bmBits = NULL;
	}
}
