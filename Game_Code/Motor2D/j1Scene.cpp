#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Physics.h"
#include "Player.h"
#include "j1SceneManager.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"

//Constructors ----------------------------------
j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

//Destructors -----------------------------------
j1Scene::~j1Scene()
{

}


//Game Loop -------------------------------------
bool j1Scene::Update(float dt)
{
	bool ret = true;
	int x = 0, y = 0;

	//Draw target scene background
	App->render->Blit(spritesheet, 0, 0, &background_rect);

	SceneUpdate();

	//Draw all the target scene items
	p2List_item<Item*>* item = Items.start;
	while (item)
	{
		//Get item position
		item->data->GetPosition(x, y);
		//Blit item texture from spritesheet
		ret = App->render->Blit(App->Items_Spritesheet, x, y, &item->data->Get_Texture(), item->data->Get_Scale(), 1.0f, item->data->GetRotation());
		//Set next item
		item = item->next;
	}

	//Draw all the target scene platforms
	SDL_Rect current_animation = { 0,0,0,0 };
	p2List_item<Platform*>* platform = Platforms.start;
	while (platform)
	{
		//Get item current data
		platform->data->Get_Position(x, y);
		current_animation = platform->data->Get_CurrentAnimationRect();

		//Blit item texture from spritesheet
		if (!platform->data->IsInToggle())ret = App->render->Blit(platform->data->Get_Texture(), x, y, &current_animation, platform->data->Get_Scale());

		//In case that the platform is in color swap
		else
		{
			SDL_Rect next_animation = platform->data->Get_NextAnimationRect();
			if (platform->data->CheckToggle())
			{
				ret = App->render->Blit(platform->data->Get_Texture(), x, y, &current_animation, platform->data->Get_Scale());
				ret = App->render->Blit(platform->data->Get_NextTexture(), x, y, &next_animation, platform->data->Get_Scale());
			}
			else
			{
				ret = App->render->Blit(platform->data->Get_Texture(), x, y, &platform->data->Get_CurrentAnimationRect(), platform->data->Get_Scale());
			}
		}
		//Set next item
		platform = platform->next;
	}

	return ret;
}

bool j1Scene::PostUpdate()
{
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		Desactivate();
		if (App->win->fullscreen)
		{
			SDL_SetWindowFullscreen(App->win->window, NULL);
			App->win->fullscreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN);
			App->win->fullscreen = true;
		}
		Activate();
	}

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		if(Platforms.start->data->Get_Type() == PLATFORM_TYPE::RED)Platforms.start->data->ChangeType(PLATFORM_TYPE::ORANGE);
		else Platforms.start->data->ChangeType(PLATFORM_TYPE::RED);
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing %s",this->name.GetString());

	bool ret = true;

	//Clean all Scene Items
	p2List_item<Item*>* item = Items.end;
	p2List_item<Item*>* item_prev = nullptr;

	if (item != nullptr)item_prev = item->prev;
	while (item) {

		//Delete all item data
		ret = Items.del(item);

		item = item_prev;
		if (item_prev != nullptr)item_prev = item_prev->prev;

	}

	//Clean all Scene Platforms
	p2List_item<Platform*>* p_item = Platforms.end;
	p2List_item<Platform*>* p_item_prev = nullptr;

	if (p_item != nullptr)p_item_prev = p_item->prev;
	while (p_item) {

		//Delete all item data
		ret = Platforms.del(p_item);

		p_item = p_item_prev;
		if (p_item_prev != nullptr)p_item_prev = p_item_prev->prev;

	}

	return ret;
}

bool j1Scene::SceneUpdate()
{
	return true;
}

void j1Scene::GetPlayerSpawn(int & x, int & y)
{
	x = player_x_cord;
	y = player_y_cord;
}

//Functionality ---------------------------------
bool j1Scene::GeneratePlatformsTextures()
{
	bool ret = true;
	p2List_item<Platform*>* item = Platforms.start;
	while (item)
	{
		item->data->SetTexture(item->data->GenerateTexture(item->data->Get_Type()));

		item = item->next;
	}
	return ret;
}

void j1Scene::CleanPlatformsTextures()
{
	bool ret = true;
	p2List_item<Platform*>* item = Platforms.start;

	while (item)
	{
		item->data->DestroyTexture();

		item = item->next;
	}

	return;
}

Item * j1Scene::GenerateSceneItem(BUILD_ITEM_TYPE item_type, uint scale)
{
	Item* new_item = nullptr;

	switch (item_type)
	{
		// Box Builds -------------------------------
	case BOX_BOOKS:			new_item = new Item({ 0, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_XMAS:			new_item = new Item({ 52, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_SNES:			new_item = new Item({ 104, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_NUKE:			new_item = new Item({ 156, 0, 51, 50 }, ITEM_TYPE::BOX, scale);		break;
	case BOX_LARGE_XMAS:	new_item = new Item({ 0, 58, 100, 97 }, ITEM_TYPE::BOX, scale);		break;
	}

	new_item->Get_body()->listener = this;
	Items.add(new_item);

	return new_item;
}

Platform * j1Scene::GenerateScenePlatfrom(BUILD_PLATFORM_TYPE platform_type, uint width, uint scale)
{
	Platform* new_platform = nullptr;

	switch (platform_type)
	{
		// Platform Builds --------------------------
	case BUILD_PLATFORM_TYPE::PLATFORM_BLACK:	new_platform = new Platform(width, PLATFORM_TYPE::BLACK, scale);		break;
	case BUILD_PLATFORM_TYPE::PLATFORM_BLUE:	new_platform = new Platform(width, PLATFORM_TYPE::BLUE, scale);			break;
	case BUILD_PLATFORM_TYPE::PLATFORM_GREEN:	new_platform = new Platform(width, PLATFORM_TYPE::GREEN, scale);		break;
	case BUILD_PLATFORM_TYPE::PLATFORM_ORANGE:	new_platform = new Platform(width, PLATFORM_TYPE::ORANGE, scale);		break;
	case BUILD_PLATFORM_TYPE::PLATFORM_PURPLE:	new_platform = new Platform(width, PLATFORM_TYPE::PURPLE, scale);		break;
	case BUILD_PLATFORM_TYPE::PLATFORM_RED:		new_platform = new Platform(width, PLATFORM_TYPE::RED, scale);			break;
	case BUILD_PLATFORM_TYPE::PLATFORM_YELLOW:	new_platform = new Platform(width, PLATFORM_TYPE::YELLOW, scale);		break;
	}

	new_platform->Get_Body()->listener = this;
	Platforms.add(new_platform);
	
	return new_platform;
}

Item * j1Scene::GenerateSceneGoal(uint scale)
{
	goal_item = new Item({ 52, 0, 51, 50 }, ITEM_TYPE::GOAL, scale);
	
	PhysBody* item_body = goal_item->Get_body();
	item_body->listener = this;
	item_body->collide_type = BODY_TYPE::goal_item;

	Items.add(goal_item);
	
	return goal_item;
}

void j1Scene::EndScene()
{
	LOG("%s end!", name.GetString());
	App->scene_manager->ChangeScene(this, App->GetNextScene(this),1);
	App->player->Respawn();
}

void j1Scene::Activate()
{
	LOG("Activating Scene!");
	if (!GeneratePlatformsTextures())
	{
		LOG("Scene Textures wasn't clear before generation!");
	}
	return;
}

void j1Scene::Desactivate()
{
	LOG("Desactivating Scene...");
	CleanPlatformsTextures();
}
