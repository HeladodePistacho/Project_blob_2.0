#include "j1SceneBuilder.h"

#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"

//Constructors ----------------------------------
j1SceneBuilder::j1SceneBuilder()
{
	name.create("scene_builder");
}


//Destructors -----------------------------------
j1SceneBuilder::~j1SceneBuilder()
{
}


//Game Loop -------------------------------------
bool j1SceneBuilder::Start()
{
	//Load spritesheets
	boxes_spritesheet = App->tex->Load("textures/Box_textures.png");
	platforms_spritesheet = App->tex->Load("textures/Platform_textures.png");
	
	return true;
}

bool j1SceneBuilder::Update(float dt)
{
	bool ret = true;
	int x = 0, y = 0;

	//Draw target scene background
	App->render->Blit(target->GetBackgroundTexture(), 0, 0);

	//Draw all the target scene items
	p2List_item<Item*>* item = target->GetFirstItem();
	while (item)
	{
		//Get item position
		item->data->GetPosition(x, y);
		//Blit item texture from spritesheet
		ret = App->render->Blit(boxes_spritesheet, x, y, &item->data->Get_Texture(),item->data->Get_Scale(), 1.0f, item->data->GetRotation());
		//Set next item
		item = item->next;
	}

	//Draw all the target scene platforms
	p2List_item<Platform*>* platform = target->GetFirstPlatform();
	while (platform)
	{
		//Get item position
		platform->data->Get_Position(x, y);
		//Blit item texture from spritesheet
		ret = App->render->Blit(platform->data->Get_Texture(), x, y,NULL, 1, 1.0f, platform->data->Get_Rotation());
		//Set next item
		platform = platform->next;
	}

	return ret;
}


//Functionality ---------------------------------
Item * j1SceneBuilder::GenerateSceneItem(BUILD_ITEM_TYPE element_type, uint scale)
{
	Item* new_element = nullptr;

	switch (element_type)
	{
	// Box Builds -------------------------------
	case BOX_BOOKS:			new_element = new Item({ 0, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_XMAS:			new_element = new Item({ 52, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_SNES:			new_element = new Item({ 104, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_NUKE:			new_element = new Item({ 156, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_LARGE_XMAS:	new_element = new Item({ 0, 58, 100, 97 }, ITEM_TYPE::BOX, scale);		break;
	}

	if(new_element != nullptr)target->AddSceneItem(new_element);

	return new_element;
}

Platform * j1SceneBuilder::GenerateScenePlatfrom(BUILD_PLATFORM_TYPE platform_type, uint width, uint scale)
{
	Platform* new_element = nullptr;

	switch (platform_type)
	{
	// Platform Builds --------------------------
	case PLATFORM_ORANGE:	new_element = new Platform({ 0,0,(int)width,25 } , PLATFORM_TYPE::ORANGE, scale);		break;
	}

	if (new_element != nullptr)target->AddScenePlatform(new_element);

	return new_element;
}

void j1SceneBuilder::SetTarget(j1Scene* new_target)
{
	target = new_target;
}

SDL_Texture * j1SceneBuilder::GetPlatformsSpritesheet() const
{
	return platforms_spritesheet;
}
