#ifndef _TUTORIAL_1_
#define _TUTORIAL_1_

#include "j1Scene.h"

class Tutorial_1 : public j1Scene
{
public:

	Tutorial_1()
	{
		name.create("Tutorial I");
	}
	~Tutorial_1()
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

		//Background mark collide build 
		int background_points[8] = {
			0,0,
			0,720,
			1280,720,
			1280,0
		};
		GenerateCollideMark(0, 0, background_points, 8);

		//Set Player Spawn point
		player_x_cord = 50;
		player_y_cord = 150;

		//Platforms -------------
		Platform* platform = nullptr;
		platform = GenerateScenePlatfrom(PLATFORM_TYPE::PLATFORM_BLACK, 1280, 2);
		platform->Set_Position(0, 680);
		// ----------------------

		//Items -----------------
		Item* element = GenerateSceneItem(ITEM_TYPE::DRAWER_LARGE_CLOSE, 1);
		element->SetPosition(50, 200);
		element = GenerateSceneItem(ITEM_TYPE::TABLE_SHORT, 2);
		element->SetPosition(350, 500);
		// ----------------------

		//Mini Blob -------------
		Mini_Blob* blob = GenerateSceneBlob(BLOB_TYPE::BLOB_BLUE, 2);
		blob->SetPosition(1180, 500);
		// ----------------------

		SaveSceneInit();

		return true;
	}

};
#endif // _TUTORIAL_1_
