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
	//platforms_spritesheet = App->tex->Load("textures/Platform_textures.png");
	
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
		ret = App->render->Blit(boxes_spritesheet, x, y, &item->data->Get_Texture(), 1, 1.0f, item->data->GetRotation());
		//Set next item
		item = item->next;
	}

	//Draw all the target scene platforms
	// ...

	return ret;
}


//Functionality ---------------------------------
Item * j1SceneBuilder::GenerateSceneElement(BUILD_TYPE element_type)
{
	Item* new_element = nullptr;

	switch (element_type)
	{
	case BOX_BOOKS:			new_element = new Item({ 0, 0, 51, 50 }, ITEM_TYPE::BOX);		break;
	case BOX_XMAS:			new_element = new Item({ 52, 0, 51, 50 }, ITEM_TYPE::BOX);		break;
	case BOX_SNES:			new_element = new Item({ 104, 0, 51, 50 }, ITEM_TYPE::BOX);		break;
	case BOX_NUKE:			new_element = new Item({ 156, 0, 51, 50 }, ITEM_TYPE::BOX);		break;
	case BOX_LARGE_XMAS:	new_element = new Item({ 0, 58, 100, 97 }, ITEM_TYPE::BOX);		break;
	case BOX_LARGE_NUKE:	new_element = new Item({ 107, 58, 100, 97 }, ITEM_TYPE::BOX);	break;
	}

	if(new_element != nullptr)target->AddSceneItem(new_element);

	return new_element;
}

void j1SceneBuilder::SetTarget(j1Scene* new_target)
{
	target = new_target;
}
