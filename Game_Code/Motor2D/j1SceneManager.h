#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "j1Module.h"
#include "j1Timer.h"

#include "SDL\include\SDL_rect.h"

enum fade_step
{
	off,
	fade_to_black,
	fade_from_black
};

enum ITEM_TYPE
{
	BOX,
	TABLE
};

class SceneItem
{
private:
	PhysBody* body;
	SDL_Rect texture;
	ITEM_TYPE type;

public:
	SceneItem(ITEM_TYPE, PhysBody*, SDL_Rect);
	const PhysBody* Get_body() const;

};

class j1SceneManager : public j1Module
{
public:

	j1SceneManager();
	~j1SceneManager();

	// Called before the first frame
	bool Start();
	// Called each loop iteration
	bool PostUpdate();


private:

	fade_step	current_step = off;
	j1Timer		fade_timer;
	uint32		total_time = 0;

public:

	//Functionality -----------------------------
	bool ChangeScene(int time = 1.0f);
	bool IsFading() const;

};

#endif //__SCENE_MANAGER_H__