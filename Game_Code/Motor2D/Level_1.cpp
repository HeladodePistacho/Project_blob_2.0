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


	//Test box --------
	//Item* item = App->scene_builder->GenerateSceneItem(BUILD_ITEM_TYPE::BOX_BOOKS,2);
	//item->SetPosition(250, 50);

	//Test platform --
	Platform* platform = nullptr;
	platform = (Platform*)App->scene_builder->GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_ORANGE, 250, 2);
	platform->Set_Position(1120, 420);
	//platform = (Platform*)App->scene_builder->GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_BLUE, 250, 2);
	//platform->Set_Position(520, 300);
	platform = (Platform*)App->scene_builder->GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_GREEN, 250, 2);
	platform->Set_Position(520, 420);
	platform = (Platform*)App->scene_builder->GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_GREEN, 250, 2);
	platform->Set_Position(520, 100);
	//platform = (Platform*)App->scene_builder->GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_YELLOW, 250, 2);
	//platform->Set_Position(520, 420);
	return true;
}
