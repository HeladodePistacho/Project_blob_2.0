#ifndef _SCENE_BUILER_
#define _SCENE_BUILER_

#include "j1Module.h"
#include "j1Scene.h"

struct SDL_Texture;

enum BUILD_ITEM_TYPE
{
	BOX_BOOKS,
	BOX_XMAS,
	BOX_SNES,
	BOX_NUKE,
	BOX_LARGE_XMAS,
};
enum BUILD_PLATFORM_TYPE
{
	PLATFORM_BLACK,
	PLATFORM_BLUE,
	PLATFORM_GREEN,
	PLATFORM_YELLOW,
	PLATFORM_PURPLE,
	PLATFORM_RED,
	PLATFORM_ORANGE
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
	Item*			GenerateSceneItem(BUILD_ITEM_TYPE item_type, uint scale = 1);
	Platform*		GenerateScenePlatfrom(BUILD_PLATFORM_TYPE platform_type, uint width, uint scale = 1);

	void			SetTarget(j1Scene* new_target);
	j1Scene*		GetTarget()const;
	SDL_Texture*	GetPlatformsSpritesheet()const;
};
#endif // _SCENE_BUILER_
