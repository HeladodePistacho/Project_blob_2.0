#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"

struct PhysBody;

#include "Item.h"
#include "Platform.h"
#include "SDL\include\SDL_rect.h"

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

class j1Scene : public j1Module
{
public:

	//Constructor
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	//Game Loop -----------------------
	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	// --------------------------------

	//Scene Loop ----------------------
	virtual bool SceneUpdate();
	// --------------------------------
protected:

	//Spritesheet ---------------------
	SDL_Texture*			spritesheet = nullptr;
	//Background ----------------------
	SDL_Rect				background_rect;
	PhysBody*				background_collide_mark = nullptr;
	//Items ---------------------------
	p2List<Item*>			Items;
	//Platforms -----------------------
	p2List<Platform*>		Platforms;

public:

	//Functionality -----------------------------
	p2List_item<Item*>*			GetFirstItem()const;
	p2List_item<Platform*>*		GetFirstPlatform()const;
	SDL_Texture*				GetSpritesheet()const;
	const SDL_Rect*				GetBackgroundRect()const;

	bool						GeneratePlatformsTextures();
	void						CleanPlatformsTextures();

	Item*						GenerateSceneItem(BUILD_ITEM_TYPE item_type, uint scale = 1);
	Platform*					GenerateScenePlatfrom(BUILD_PLATFORM_TYPE platform_type, uint width, uint scale = 1);

	void Activate();
	void Desactivate();

};

#endif // __j1SCENE_H__