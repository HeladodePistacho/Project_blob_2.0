#ifndef _LEVEL_1_
#define _LEVEL_1_

#include "j1Scene.h"

#include "j1App.h"
#include "j1Render.h"

class Level_1 : public j1Scene
{
public:

	Level_1()
	{
		name.create("Level I");
	};
	~Level_1()
	{

	}

public:

	// Called before the first frame
	bool Start()
	{
		//Load Spritesheet
		LoadSpriteSheet("textures/Basement_Spritesheet.png");

		//Set background rect
		background_rect = { 0,0,1280,720 };

		//Build light animation
		light_anim.PushBack({ 327,730,328,360 });
		light_anim.PushBack({ 25,730,175,203 });
		light_anim.PushBack({ 327,730,328,360 });
		light_anim.PushBack({ 25,730,175,203 });
		light_anim.SetSpeed(75);
		light_anim.SetLoop(false);

		//Set Player Spawn point
		player_x_cord = 150;
		player_y_cord = 300;


		//Background mark collide build 
		int background_points[8] = {
			0,0,
			0,720,
			1280,720,
			1280,0
		};
		GenerateCollideMark(0, 0, background_points, 8);

		//Platforms -------------
		Platform* platform = nullptr;
		platform = GenerateScenePlatfrom(PLATFORM_TYPE::PLATFORM_GREEN, 1280, 2);
		platform->Set_Position(0, 650);
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
		goal_blob = GenerateSceneBlob(BLOB_TYPE::BLOB_BLUE, 2);
		goal_blob->SetPosition(1180, 500);
		// ----------------------

		SaveSceneInit();

		light_time.Start();
		return true;
	}

	// Called each loop iteration
	bool SceneUpdate()
	{
		App->render->Blit(spritesheet, light_x, light_y, &light_anim.GetCurrentFrame());

		if (light_time.Read() >= tilt_rate)
		{
			light_anim.Reset();
			light_time.Start();
		}

		return true;
	}

private:

	//Light Data -----------------
	int				light_x = 350;
	int				light_y = 0;
	Animation		light_anim;
	uint			tilt_rate = 5000;
	j1Timer			light_time;

};
#endif // _LEVEL_1_
