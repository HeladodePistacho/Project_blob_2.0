#ifndef _LEVEL_1_
#define _LEVEL_1_

#include "j1Scene.h"
struct Animation;

class Level_1 : public j1Scene
{
public:

	Level_1();
	~Level_1();

public:

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool SceneUpdate();

private:

	//Light Data -----------------
	int				light_x = 350;
	int				light_y = 0;
	Animation		light_anim;
	uint			tilt_init_rate = 2500;
	uint			tilt_length = 450;
	j1Timer			light_time;

public:

	//Functionality
};
#endif // _LEVEL_1_
