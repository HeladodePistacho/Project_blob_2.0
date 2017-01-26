#include "Level_1.h"

#include "j1App.h"
#include "j1SceneBuilder.h"

//TEMP
#include "j1Physics.h"
#include "j1Textures.h"

//Constructors ----------------------------------
Level_1::Level_1()
{
	name.create("level_1");
}

//Destructors -----------------------------------
Level_1::~Level_1()
{
}

//Game Loop -------------------------------------
bool Level_1::Start()
{
	App->scene_builder->SetTarget(this);

	//Load Background texture 
	background = App->tex->Load("textures/Basement.png");

	//Background mark collide build 
	int background_points[10] = {
		- 4, 413,
		770, 414,
		771, 479,
		- 3, 481,
		- 4, 421
	};

	background_collide_mark = App->physics->CreateChain(0, 12, background_points, 10, collision_type::MAP, BODY_TYPE::map);


	Item* item = App->scene_builder->GenerateSceneElement(BUILD_TYPE::BOX_BOOKS);
	item->SetPosition(250, 50);

	return true;
}
