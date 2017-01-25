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

	SceneItem(SDL_Rect new_tex, ITEM_TYPE TYPE);
	~SceneItem();

private:

	PhysBody*		body;
	SDL_Rect		texture;
	ITEM_TYPE		type;

public:

	//Functionality
	PhysBody*		GenerateBodyFromRect();

	void			SetPosition(int x, int y);

	const PhysBody*	Get_body() const;
	ITEM_TYPE		Get_type() const;
	SDL_Rect		Get_Texture() const;

};

#endif // !_SCENEITEM_H_

