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
#include "OpenGL/glut.h"

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

void j1Scene::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	int a_x, a_y;
	bodyA->GetPosition(a_x, a_y);
	int b_x, b_y;
	bodyB->GetPosition(b_x, b_y);
	bool at_bottom = (b_y + bodyB->height <= a_y);

	switch (bodyA->collide_type)
	{
	case platform_black:
		if (bodyB->collide_type == player)
		{
			App->player->Die();
		}
		else if (bodyB->collide_type == bullet)
		{
			App->player->DeleteBullet(App->player->FindBullet(bodyB));
		}
		break;

	case platform_blue:
		break;

	case platform_green:

		if (bodyB->collide_type == player && at_bottom)
		{
			bodyB->body->SetLinearVelocity(b2Vec2(bodyB->body->GetLinearVelocity().x, -App->player->GetVerticalAcceleration()));
		}
		break;

	case platform_yellow:
		break;

	case platform_purple:
		break;

	case platform_red:
		break;

	case platform_orange:
		break;
	}
}

//Functionality ---------------------------------
p2List_item<Item*>* j1Scene::GetFirstItem() const
{
	return Items.start;
}

p2List_item<Platform*>* j1Scene::GetFirstPlatform() const
{
	return Platforms.start;
}

SDL_Texture * j1Scene::GetBackgroundTexture() const
{
	return background;
}

void j1Scene::AddSceneItem(Item * new_item)
{
	if (new_item == nullptr)return;
	Items.add(new_item);
}

void j1Scene::AddScenePlatform(Platform * new_platform)
{
	if (new_platform == nullptr)return;
	Platforms.add(new_platform);
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
