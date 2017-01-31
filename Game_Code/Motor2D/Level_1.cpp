#include "Level_1.h"

#include "j1App.h"

//TEMP
#include "j1Physics.h"
#include "j1Textures.h"
#include "j1Render.h"

#include "Animation.h"

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
	//Load Spritesheet
	spritesheet = App->tex->Load("textures/Basement_Spritesheet.png");
	
	//Set background rect
	background_rect = { 0,0,1280,720 };

	//Build light animation
	light_anim.PushBack({ 334,722,307,361 });
	light_anim.PushBack({ 26,720,193,212 });
	light_anim.SetSpeed(150);

	//Background mark collide build 
	int background_points[10] = {
		- 4, 413,
		770, 414,
		771, 479,
		- 3, 481,
		- 4, 421
	};

	background_collide_mark = App->physics->CreateChain(0, 12, background_points, 10, collision_type::MAP, BODY_TYPE::map);


	//Test platform --
	Platform* platform = nullptr;
	platform = GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_ORANGE, 250, 2);
	platform->Set_Position(1120, 420);
	platform = GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_GREEN, 250, 2);
	platform->Set_Position(520, 420);
	platform = GenerateScenePlatfrom(BUILD_PLATFORM_TYPE::PLATFORM_GREEN, 250, 2);
	platform->Set_Position(520, 100);

	return true;
}

bool Level_1::SceneUpdate()
{
	App->render->Blit(spritesheet, light_x, light_y, &light_anim.GetCurrentFrame());
	return true;
}
