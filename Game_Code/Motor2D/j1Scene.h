#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1SceneManager.h"
#include "SceneItem.h"

struct PhysBody;
struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Recieve UI input and work with it
	void GUI_Input(UI_Element* target, GUI_INPUT input);

	// Recieve Physics Collisions
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	//Active/Deasctive scene
	void Activate();
	void Desactivate();

private:

	//Background ----------------------
	SDL_Texture*		background;
	PhysBody*			background_collide_mark = nullptr;

	//Items ---------------------------
	p2List<SceneItem*>	Items;
	//Box texture
	SDL_Texture*		boxes;

public:

	//Functionality -----------------------------


};

#endif // __j1SCENE_H__