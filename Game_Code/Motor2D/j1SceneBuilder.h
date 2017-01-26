#ifndef _SCENE_BUILER_
#define _SCENE_BUILER_

#include "j1Module.h"
#include "j1Scene.h"

struct SDL_Texture;

enum BUILD_TYPE
{
	BOX_BOOKS,
	BOX_XMAS,
	BOX_SNES,
	BOX_NUKE,
	BOX_LARGE_XMAS,
	BOX_LARGE_NUKE
};


class j1SceneBuilder : public j1Module
{
public:

	j1SceneBuilder();
	~j1SceneBuilder();

public:

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

private:

	//Spritesheets for all items ------
	SDL_Texture*	platforms_spritesheet = nullptr;
	SDL_Texture*	boxes_spritesheet = nullptr;
	//Builder target ------------------
	j1Scene*		target = nullptr;

public:

	//Functionality ----------------------
	Item*	GenerateSceneElement(BUILD_TYPE element_type);
	
	void	SetTarget(j1Scene* new_target);
};
#endif // _SCENE_BUILER_
