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
			0,640,
			1280,640,
			1280,0
		};
		GenerateCollideMark(0, 0, background_points, 8);

		//Set Player Spawn point
		player_x_cord = 50;
		player_y_cord = 150;

		//Items -----------------
		Item* element = nullptr;
		element = GenerateSceneItem(ITEM_TYPE::TABLE_STANDAR, 2);
		element->SetPosition(200, 316);
		element = GenerateSceneItem(ITEM_TYPE::BOX_BOOKS);
		element->SetPosition(620, 500);
		element = GenerateSceneItem(ITEM_TYPE::BOX_BOOKS);
		element->SetPosition(675, 500);
		element = GenerateSceneItem(ITEM_TYPE::BOX_BOOKS);
		element->SetPosition(650, 450);
		element = GenerateSceneItem(ITEM_TYPE::BOX_LARGE_XMAS);
		element->SetPosition(950, 500);
		// ----------------------

		//Mini Blob -------------
		goal_blob = GenerateSceneBlob(BLOB_TYPE::BLOB_BLUE, 2);
		goal_blob->SetPosition(1180, 500);
		// ----------------------

		SaveSceneInit();

		return true;
	}

};
#endif // _TUTORIAL_1_
