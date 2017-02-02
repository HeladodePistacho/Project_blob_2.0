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

	//Draw all the target scene blobs
	p2List_item<Mini_Blob*>* blob = Blobs.start;
	while (blob)
	{
		//Get item position
		blob->data->GetPosition(x, y);
		//Blit item texture from spritesheet
		ret = App->render->Blit(App->WinBlobs_Spritesheet, x , y, &blob->data->GetCurrentAnimRect(), blob->data->GetScale(), 1.0f);
		//Set next item
		blob = blob->next;
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
		if(Platforms.start->data->Get_Type() == PLATFORM_RED)Platforms.start->data->ChangeType(PLATFORM_ORANGE);
		else Platforms.start->data->ChangeType(PLATFORM_RED);
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

Item * j1Scene::GenerateSceneItem(ITEM_TYPE item_type, uint scale)
{
	Item* new_item = nullptr;

	bool body_gen = false;

	switch (item_type)
	{
		// Box Builds -------------------------------
	case BOX_BOOKS:			new_item = new Item({ 0, 0, 51, 50 }, item_type, scale);		break;
	case BOX_XMAS:			new_item = new Item({ 52, 0, 51, 50 }, item_type, scale);		break;
	case BOX_SNES:			new_item = new Item({ 104, 0, 51, 50 }, item_type, scale);		break;
	case BOX_NUKE:			new_item = new Item({ 156, 0, 51, 50 }, item_type, scale);		break;
	case BOX_LARGE_XMAS:	new_item = new Item({ 0, 58, 100, 97 }, item_type, scale);		break;
	
	case STANDAR_TABLE:	
		new_item = new Item({0, 158, 300, 170}, item_type, scale);	
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 300 * scale, 16 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	}

	if (!body_gen)
	{
		new_item->SetBody(App->physics->CreateRectangle(0, 0, new_item->Get_Texture().w * scale, new_item->Get_Texture().h * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
	}
	
	new_item->Get_body()->listener = this;
	Items.add(new_item);

	return new_item;
}

Platform * j1Scene::GenerateScenePlatfrom(PLATFORM_TYPE platform_type, uint width, uint scale)
{
	Platform* new_platform = nullptr;

	switch (platform_type)
	{
		// Platform Builds --------------------------
	case PLATFORM_TYPE::PLATFORM_BLACK:		new_platform = new Platform(width, platform_type, scale);		break;
	case PLATFORM_TYPE::PLATFORM_BLUE:		new_platform = new Platform(width, platform_type, scale);			break;
	case PLATFORM_TYPE::PLATFORM_GREEN:		new_platform = new Platform(width, platform_type, scale);		break;
	case PLATFORM_TYPE::PLATFORM_ORANGE:	new_platform = new Platform(width, platform_type, scale);		break;
	case PLATFORM_TYPE::PLATFORM_PURPLE:	new_platform = new Platform(width, platform_type, scale);		break;
	case PLATFORM_TYPE::PLATFORM_RED:		new_platform = new Platform(width, platform_type, scale);			break;
	case PLATFORM_TYPE::PLATFORM_YELLOW:	new_platform = new Platform(width, platform_type, scale);		break;
	}

	new_platform->Get_Body()->listener = this;
	Platforms.add(new_platform);
	
	return new_platform;
}

Mini_Blob * j1Scene::GenerateSceneBlob(BLOB_TYPE type, uint scale)
{
	Mini_Blob* new_blob = nullptr;

	new_blob = new Mini_Blob(type, scale);

	Animation* fear = new_blob->GetFearAnim();
	Animation* happy = new_blob->GetHappyAnim();
	

	switch (type)
	{
	case BLOB_GREEN:
		fear->PushBack({ 0,4,19,15 });
		fear->PushBack({ 21,4,17,15 });
		happy->PushBack({ 40,4,19,16 });
		happy->PushBack({ 60,4,18,16 });
		break;
	case BLOB_YELLOW:
		fear->PushBack({ 0,31,19,15 });
		fear->PushBack({ 21,31,17,15 });
		happy->PushBack({ 40,5,19,15 });
		happy->PushBack({ 61,4,17,16 });
		break;
	case BLOB_RED:
		fear->PushBack({ 0,57,19,15 });
		fear->PushBack({ 21,57,17,15 });
		happy->PushBack({ 40,5,19,15 });
		happy->PushBack({ 61,4,17,16 });
		break;
	case BLOB_ORANGE:
		fear->PushBack({ 0,83,19,15 });
		fear->PushBack({ 21,83,17,15 });
		happy->PushBack({ 40,5,19,15 });
		happy->PushBack({ 61,4,17,16 });
		break;
	case BLOB_BLUE:
		fear->PushBack({ 0,109,19,15 });
		fear->PushBack({ 21,109,17,15 });
		happy->PushBack({ 40,5,19,15 });
		happy->PushBack({ 61,4,17,16 });
		break;
	}

	fear->SetSpeed(350);
	happy->SetSpeed(350);
	new_blob->SetHappy();

	new_blob->SetBody(App->physics->CreateRectangle(0, 0, happy->GetFirstFrame().w * scale, happy->GetFirstFrame().h * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
	new_blob->GetBody()->FixedRotation(true);
	Blobs.add(new_blob);
	
	return new_blob;
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
