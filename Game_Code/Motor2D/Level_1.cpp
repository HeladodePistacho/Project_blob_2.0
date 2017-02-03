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
	light_anim.PushBack({ 327,730,328,360 });
	light_anim.PushBack({ 25,730,175,203 });
	light_anim.PushBack({ 327,730,328,360 });
	light_anim.PushBack({ 25,730,175,203 });
	light_anim.SetSpeed(75);
	light_anim.SetLoop(false);

	//Background mark collide build 
	int background_points[10] = {
		- 4, 413,
		770, 414,
		771, 479,
		- 3, 481,
		- 4, 421
	};

	background_collide_mark = App->physics->CreateChain(0, 12, background_points, 10, collision_type::MAP, BODY_TYPE::map);

	//Set Player Spawn point
	player_x_cord = 350;
	player_y_cord = 300;

	//Test platform --
	Platform* platform = nullptr;

	platform = GenerateScenePlatfrom(PLATFORM_TYPE::PLATFORM_GREEN, 250, 2);
	platform->Set_Position(520, 420);
	

	Item* element = GenerateSceneItem(ITEM_TYPE::DRAWER_LARGE_CLOSE, 2);
	element->SetPosition(750, 450);

	Mini_Blob* blob = GenerateSceneBlob(BLOB_TYPE::BLOB_BLUE, 2);
	blob->SetPosition(450, 300);

	light_time.Start();
	return true;
}

bool Level_1::SceneUpdate()
{
	App->render->Blit(spritesheet, light_x, light_y, &light_anim.GetCurrentFrame());

	if (light_time.Read() >= tilt_rate)
	{
		light_anim.Reset();
		light_time.Start();
	}

	return true;
}
