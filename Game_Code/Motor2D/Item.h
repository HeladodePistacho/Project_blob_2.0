#ifndef _SCENEITEM_H_
#define _SCENEITEM_H_

#include "p2Defs.h"
#include "SDL\include\SDL_rect.h"

struct PhysBody;

enum ITEM_TYPE
{
	BOX_BOOKS,
	BOX_XMAS,
	BOX_SNES,
	BOX_NUKE,
	BOX_LARGE_XMAS,
	STANDAR_TABLE
};

class Item
{
public:

	Item(SDL_Rect new_tex, ITEM_TYPE TYPE, uint scale = 1);
	~Item();

private:

	PhysBody*		body;
	SDL_Rect		texture;
	uint			scale;
	ITEM_TYPE		item_type;

public:

	//Functionality --------------
	void			SetPosition(int x, int y);
	void			SetBody(PhysBody* new_body);
	void			GetPosition(int& x, int& y)const;
	float			GetRotation()const;
	PhysBody*		Get_body() const;
	ITEM_TYPE		Get_type() const;
	SDL_Rect		Get_Texture() const;
	int				Get_Scale()const;
	
};

#endif // !_SCENEITEM_H_

