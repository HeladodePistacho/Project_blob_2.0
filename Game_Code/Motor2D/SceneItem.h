#ifndef _SCENEITEM_H_
#define _SCENEITEM_H_

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

struct PhysBody;

enum ITEM_TYPE
{
	BOX,
	TABLE
};

class SceneItem
{
public:

	SceneItem(ITEM_TYPE, PhysBody*, SDL_Rect);

private:

	PhysBody* body;
	SDL_Rect texture;
	ITEM_TYPE type;

public:

	const PhysBody* Get_body() const;
	ITEM_TYPE Get_type() const;
	SDL_Rect Get_Texture() const;

};

#endif // !_SCENEITEM_H_

