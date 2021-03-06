#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
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
	int scale = 0;
	//Draw target scene background
	App->render->Blit(spritesheet, 0, 0, &background_rect);

	SceneUpdate();

	//Draw all the target scene items
	p2List_item<Item*>* item = Items.start;
	while (item)
	{
		//Get item position & scale
		item->data->GetPosition(x, y);
		scale = item->data->Get_Scale();

		//Blit item texture from spritesheet
		ret = App->render->Blit(App->Items_Spritesheet, x + (item->data->GetTex_X_margin() * scale), y + (item->data->GetTex_Y_margin() * scale), &item->data->Get_Texture(), scale, 1.0f, item->data->GetRotation());
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
		App->scene_manager->ChangeScene(this, (j1Module*)App->hub, 500);
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


	if (goal_blob == nullptr)return ret;

	//Draw target scene blob
	//Get position
	int x, y;
	goal_blob->GetPosition(x, y);
	//Blit item texture from spritesheet
	ret = App->render->Blit(App->WinBlobs_Spritesheet, x, y, &goal_blob->GetCurrentAnimRect(), goal_blob->GetScale(), 1.0f);


	if (goal_blob->IsHappy() && (blob_timer.Read() > blob_happy_delay) && !at_change)
	{
		at_change = true;
		App->player->AddSceneCompleted(this);
		EndScene();
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	bool ret = true;
	LOG("Freeing %s", this->name.GetString());

	//Destroy Background collide mark
	if (background_collide_mark != nullptr)
	{
		App->physics->DeleteBody(background_collide_mark);
		background_collide_mark = nullptr;
	}

	//Free scene spritesheet
	App->tex->UnLoad(spritesheet);
	spritesheet = nullptr;

	//Delete Scene Blob
	if (goal_blob != nullptr)
	{
		App->physics->DeleteBody(goal_blob->GetBody());
		delete goal_blob;
		goal_blob = nullptr;
	}


	//Clean all Scene Items
	p2List_item<Item*>* item = Items.end;
	p2List_item<Item*>* item_prev = nullptr;

	if (item != nullptr)item_prev = item->prev;
	while (item) {

		//Delete all item data
		if(!App->physics->DeleteBody(item->data->Get_body()))return false;
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
		if (!App->physics->DeleteBody(p_item->data->Get_Body()))return false;
		ret = Platforms.del(p_item);

		p_item = p_item_prev;
		if (p_item_prev != nullptr)p_item_prev = p_item_prev->prev;

	}

	ret = SceneCleanUp();

	return ret;
}

bool j1Scene::SceneStart()
{
	return true;
}

bool j1Scene::SceneUpdate()
{
	return true;
}

bool j1Scene::SceneCleanUp()
{
	bool ret = true;

	return ret;
}

//Functionality ---------------------------------
void j1Scene::GetPlayerSpawn(int & x, int & y)
{
	x = player_x_cord;
	y = player_y_cord;
}

Mini_Blob * j1Scene::GetBlob() const
{
	return goal_blob;
}


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
	//Balls Builds ------------------------------
	case BALL_GREEN:		new_item = new Item({ 399, 7, 7, 7 }, item_type, scale);		break;
	case BALL_YELLOW:		new_item = new Item({ 408, 7, 7, 7 }, item_type, scale);		break;
	case BALL_RED:			new_item = new Item({ 417, 7, 7, 7 }, item_type, scale);		break;
	case BALL_ORANGE:		new_item = new Item({ 426, 7, 7, 7 }, item_type, scale);		break;
	case BALL_BLUE:			new_item = new Item({ 435, 7, 7, 7 }, item_type, scale);		break;
	// Box Builds -------------------------------
	case BOX_BOOKS:			new_item = new Item({ 0, 0, 51, 50 }, item_type, scale);		break;
	case BOX_XMAS:			new_item = new Item({ 52, 0, 51, 50 }, item_type, scale);		break;
	case BOX_SNES:			new_item = new Item({ 104, 0, 51, 50 }, item_type, scale);		break;
	case BOX_NUKE:			new_item = new Item({ 156, 0, 51, 50 }, item_type, scale);		break;
	case BOX_LARGE_XMAS:	new_item = new Item({ 0, 58, 100, 97 }, item_type, scale);		break;
	// Tables Builds ----------------------------
	case TABLE_STANDAR:
		new_item = new Item({ 0, 158, 300, 170 }, item_type, scale);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 300 * scale, 16 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	case TABLE_SHORT:
		new_item = new Item({ 15,336,272,48 }, item_type, scale);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 272 * scale, 10 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	// Drawers Builds ---------------------------
	case DRAWER_LEFT:
		new_item = new Item({ 209,68,78,80 }, item_type, scale);
		new_item->SetTextMargins(-8, 0);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 7 * scale, 80 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	case DRAWER_LEFT_SEG:
		new_item = new Item({ 312,199,54,80 }, item_type, scale);
		new_item->SetTextMargins(-9, 0);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 7 * scale, 80 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	case DRAWER_RIGHT_SEG:
		new_item = new Item({ 366,199,102,80 }, item_type, scale);
		new_item->SetTextMargins(-87, 0);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 7 * scale, 80 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	case DRAWER_CENTER:
		new_item = new Item({ 305,129,177,64 }, item_type, scale);
		new_item->SetTextMargins(0, -8);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 177 * scale, 9 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	case DRAWER_SHORT_CLOSE:
		new_item = new Item({ 290,68,99,49 }, item_type, scale);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 99 * scale, 34 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	case DRAWER_LARGE_CLOSE:
		new_item = new Item({ 391,68,150,49 }, item_type, scale);
		new_item->SetBody(App->physics->CreateRectangle(0, 0, 150 * scale, 34 * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
		new_item->Get_body()->body->SetType(b2BodyType::b2_staticBody);
		body_gen = true;
		break;
	//Other items -------------------------------
	case CHAIR:
		break;
	case CAMPFIRE:
		break;
	case TUPPER:
		break;
	case BLUE_TANK_PERFIL:
		break;
	case BLUE_TANK_FRONT:
		break;
	case BALANCE:
		break;
	default:
		break;
	}
	if (!body_gen)
	{
		new_item->SetBody(App->physics->CreateRectangle(0, 0, new_item->Get_Texture().w * scale, new_item->Get_Texture().h * scale, collision_type::MAP_ITEM, BODY_TYPE::map_item));
	}
	
	new_item->Get_body()->listener = this;
	new_item->Get_body()->body->SetUserData(new_item->Get_body());
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
		fear->PushBack({ 0,4,19,16 });
		fear->PushBack({ 21,4,17,16 });
		happy->PushBack({ 40,4,19,16 });
		happy->PushBack({ 60,4,18,16 });
		break;
	case BLOB_YELLOW:
		fear->PushBack({ 0,25,19,16 });
		fear->PushBack({ 21,25,17,16 });
		happy->PushBack({ 40,25,19,16 });
		happy->PushBack({ 60,25,18,16 });
		break;
	case BLOB_RED:
		fear->PushBack({ 0,46,19,16 });
		fear->PushBack({ 21,46,17,16 });
		happy->PushBack({ 40,46,19,16 });
		happy->PushBack({ 60,46,18,16 });
		break;
	case BLOB_ORANGE:
		fear->PushBack({ 0,67,19,16 });
		fear->PushBack({ 21,67,17,16 });
		happy->PushBack({ 40,67,19,16 });
		happy->PushBack({ 60,67,18,16 });
		break;
	case BLOB_BLUE:
		fear->PushBack({ 0,88,19,16 });
		fear->PushBack({ 21,88,17,16 });
		happy->PushBack({ 40,88,19,16 });
		happy->PushBack({ 60,88,18,16 });
		break;
	}

	fear->SetSpeed(350);
	happy->SetSpeed(350);
	new_blob->SetFear();

	new_blob->SetBody(App->physics->CreateRectangle(0, 0, happy->GetFirstFrame().w * scale, happy->GetFirstFrame().h * scale, collision_type::MINI_BLOB, BODY_TYPE::mini_blob));
	new_blob->GetBody()->FixedRotation(true);

	if (goal_blob != nullptr)delete goal_blob;
	goal_blob = new_blob;
	
	return new_blob;
}

bool j1Scene::LoadSpriteSheet(const char * folder)
{
	spritesheet = App->tex->Load(folder);
	return spritesheet != nullptr;
}

void j1Scene::GenerateCollideMark(int x, int y, int * points, int points_num)
{
	background_collide_mark = App->physics->CreateChain(x,y,points,points_num, collision_type::MAP, BODY_TYPE::map);
}

void j1Scene::BlobContact()
{
	if (this->goal_blob == nullptr || this->name.GetString() == "hub")return;

	if (!goal_blob->IsHappy() && start_delay_timer.Read() > start_delay)
	{
		goal_blob->SetHappy();
		blob_timer.Start();
	}
}

void j1Scene::EndScene()
{
	LOG("%s end!", name.GetString());
	App->scene_manager->ChangeScene(this, App->GetNextScene(this),500);
}

void j1Scene::Reset()
{
	//Reset Player State
	App->player->Respawn();

	//Reset Blob Position
	goal_blob->SetPosition(goal_loc.x + goal_blob->GetBody()->width, goal_loc.y + goal_blob->GetBody()->height);
	goal_blob->GetBody()->body->SetLinearVelocity(b2Vec2(0, 0));
	goal_blob->GetBody()->body->SetAwake(true);

	//Reset Scene Items
	p2List_item<Item*>* item = Items.start;
	p2List_item<iPoint>* loc = Items_loc.start;

	while (item && loc)
	{
		item->data->SetPosition(loc->data.x + item->data->Get_body()->width, loc->data.y + item->data->Get_body()->height);
		item->data->Get_body()->body->SetLinearVelocity(b2Vec2(0,0));
		item->data->Get_body()->body->SetAwake(true);
		item = item->next;
		loc = loc->next;
	}
}

void j1Scene::SaveSceneInit()
{
	//Save Mini Blob location
	int x, y;
	goal_blob->GetPosition(x, y);
	goal_loc = iPoint(x, y);

	//Save scene items location
	p2List_item<Item*>* item = Items.start;
	while (item)
	{
		item->data->GetPosition(x, y);

		Items_loc.add(iPoint(x, y));

		item = item->next;
	}
}

void j1Scene::Activate()
{
	LOG("Activating Scene!");
	Start();
	GeneratePlatformsTextures();
	App->player->Activate();
	App->player->Respawn();
	active = true;
	at_change = false;
	App->physics->Activate();
	start_delay_timer.Start();
	return;
}

void j1Scene::Desactivate()
{
	LOG("Desactivating Scene...");
	CleanPlatformsTextures();
	if(spritesheet != nullptr)CleanUp();

	App->physics->Desactivate();
	App->player->Desactivate();
	active = false;
	at_change = false;
}
