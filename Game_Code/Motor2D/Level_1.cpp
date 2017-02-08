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
	int background_points[8] = {
		0,0,
		0,720,
		1280,720,
		1280,0
	};

	background_collide_mark = App->physics->CreateChain(0, 0, background_points, 8, collision_type::MAP, BODY_TYPE::map);

	//Set Player Spawn point
	player_x_cord = 150;
	player_y_cord = 300;

	//Platform --------------
	Platform* platform = nullptr;
	platform = GenerateScenePlatfrom(PLATFORM_TYPE::PLATFORM_BLACK, 1280, 2);
	platform->Set_Position(0, 680);
	// ----------------------
	
	//Items -----------------
	Item* element = GenerateSceneItem(ITEM_TYPE::TABLE_STANDAR, 1);
	element->SetPosition(150, 500);
	element = GenerateSceneItem(ITEM_TYPE::DRAWER_SHORT_CLOSE, 1);
	element->SetPosition(750, 300);
	element = GenerateSceneItem(ITEM_TYPE::BOX_BOOKS, 2);
	element->SetPosition(1150, 600);
	element->Get_body()->body->SetType(b2BodyType::b2_staticBody);
	// ----------------------

	//Mini Blob -------------
	Mini_Blob* blob = GenerateSceneBlob(BLOB_TYPE::BLOB_BLUE, 2);
	blob->SetPosition(1180, 500);
	// ----------------------

	SaveSceneInit();

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
