#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"

struct PhysBody;
struct SDL_Texture;

#include "Item.h"
#include "Platform.h"

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called after all Updates
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

	// On collision callback
	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

protected:

	//Background ----------------------
	SDL_Texture*			background = nullptr;
	PhysBody*				background_collide_mark = nullptr;
	//Items ---------------------------
	p2List<Item*>			Items;
	//Platforms -----------------------
	p2List<Platform*>		Platforms;

public:

	//Functionality -----------------------------
	p2List_item<Item*>*			GetFirstItem()const;
	p2List_item<Platform*>*		GetFirstPlatform()const;
	SDL_Texture*				GetBackgroundTexture()const;

	void						AddSceneItem(Item* new_item);
	void						AddScenePlatform(Platform* new_platform);
	
	bool						GeneratePlatformsTextures();
	void						CleanPlatformsTextures();

	void Activate();
	void Desactivate();

};

#endif // __j1SCENE_H__