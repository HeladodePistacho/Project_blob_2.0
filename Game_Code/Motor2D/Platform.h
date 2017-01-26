#ifndef _PLATFORM_
#define _PLATFORM_

#include "Item.h"

enum PLATFORM_TYPE
{
	GREEN,
	YELLOW,
	BLACK,
	BLUE,
	ORANGE,
	RED,
	PURPLE
};

class Platform :public Item
{
public:

	Platform(SDL_Rect texture, PLATFORM_TYPE TYPE);
	~Platform();

private:

	PLATFORM_TYPE platform_type;

};
#endif // _PLATFORM_
