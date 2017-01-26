#ifndef _SCENEITEM_H_
#define _SCENEITEM_H_

#include "SDL\include\SDL_rect.h"

struct PhysBody;

enum ITEM_TYPE
{
	BOX,
	TABLE,
};

class Item
{
public:

	Item(SDL_Rect new_tex, ITEM_TYPE TYPE);
	~Item();

private:

	PhysBody*		body;
	SDL_Rect		texture;
	ITEM_TYPE		item_type;
	
	//Build ----------------------
	PhysBody*		GenerateBodyFromRect();

public:

	//Functionality --------------
	void			SetPosition(int x, int y);
	
	void			GetPosition(int& x, int& y)const;
	float			GetRotation()const;
	const PhysBody*	Get_body() const;
	ITEM_TYPE		Get_type() const;
	SDL_Rect		Get_Texture() const;
};

#endif // !_SCENEITEM_H_

