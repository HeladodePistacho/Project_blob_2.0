#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "j1Module.h"
#include "j1Timer.h"

#include "SDL\include\SDL_rect.h"
struct j1Scene;

enum fade_step
{
	off,
	fade_to_black,
	fade_from_black
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

	j1Scene*	in_scene = nullptr;
	j1Scene*	to_scene = nullptr;

	fade_step	current_step = off;
	j1Timer		fade_timer;
	uint32		total_time = 0;

public:

	//Functionality -----------------------------
	bool ChangeScene(j1Scene* in, j1Scene* to, int time = 1);
	bool IsFading() const;

};

#endif //__SCENE_MANAGER_H__