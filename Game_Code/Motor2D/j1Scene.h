#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"

struct PhysBody;
struct SDL_Texture;

#include "Item.h"
#include "Platform.h"
#include "j1Timer.h"
#include "SDL\include\SDL_rect.h"
#include "Item.h"
#include "Platform.h"
#include "Mini_Blob.h"

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
	virtual bool SceneStart();
	virtual bool SceneUpdate();
	virtual bool SceneCleanUp();
	// --------------------------------

protected:

	//Spritesheet ---------------------
	SDL_Texture*			spritesheet = nullptr;
	//Background ----------------------
	SDL_Rect				background_rect;
	PhysBody*				background_collide_mark = nullptr;
	//Items ---------------------------
	p2List<Item*>			Items;
	p2List<iPoint>			Items_loc;
	//Platforms -----------------------
	p2List<Platform*>		Platforms;
	//Blobs ---------------------------
	Mini_Blob*				goal_blob = nullptr;
	iPoint					goal_loc;
	uint					blob_happy_delay = 2500;
	j1Timer					blob_timer;
	bool					at_change = false;
	//Player Spawn Coordinates --------
	int player_x_cord = 0;
	int player_y_cord = 0;

public:

	//Functionality -----------------------------
	void						GetPlayerSpawn(int& x, int& y);
	Mini_Blob*					GetBlob()const;

	bool						GeneratePlatformsTextures();
	void						CleanPlatformsTextures();

	Item*						GenerateSceneItem(ITEM_TYPE item_type, uint scale = 1);
	Platform*					GenerateScenePlatfrom(PLATFORM_TYPE platform_type, uint width, uint scale = 1);
	Mini_Blob*					GenerateSceneBlob(BLOB_TYPE type, uint scale);
	bool						LoadSpriteSheet(const char* folder);
	void						GenerateCollideMark(int x, int y, int* points, int points_num);

	void						BlobContact();
	void						EndScene();
	void						Reset();
	void						SaveSceneInit();

	virtual void				Activate();
	virtual void				Desactivate();

};

#endif // __j1SCENE_H__