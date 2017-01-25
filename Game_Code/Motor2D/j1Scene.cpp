#define _CRT_SECURE_NO_WARNINGS
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


//UI Elements
#include "UI_String.h"
#include "UI_Image.h"
#include "UI_Button.h"



j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{



	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Load Background texture
	background = App->tex->Load("textures/Basement.png");

	//Background mark collide build
	int background_points[10] = {
		-4, 413,
		770, 414,
		771, 479,
		-3, 481,
		-4, 421
	};
	background_collide_mark = App->physics->CreateChain(0, 12, background_points, 10, collision_type::MAP, BODY_TYPE::map);

	//loading box texture
	boxes = App->tex->Load("textures/Box_textures.png");

	//Loading Scene items

	PhysBody* box = App->physics->CreateRectangle(600, 100, 50, 50, collision_type::PLAYER);
	SceneItem* item = new SceneItem(BOX, box, { 0,0,51,50 });
	Items.add(item);

	box = App->physics->CreateRectangle(651, 100, 50, 50, collision_type::PLAYER);
	item = new SceneItem(BOX, box, { 52,0,51,50 });
	Items.add(item);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	App->render->Blit(background, 0, 0);

	p2List_item<SceneItem*>* actual_item = Items.start;

	while (actual_item)
	{
		switch (actual_item->data->Get_type())
		{
		case BOX:
			int x, y;
			actual_item->data->Get_body()->GetPosition(x, y);
			App->render->Blit(boxes, x, y, &actual_item->data->Get_Texture());
		}

		actual_item = actual_item->next;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	for (p2List_item<SceneItem*>* item = Items.start; item != nullptr; item = item->next)
		Items.del(item);
	


	return true;
}

void j1Scene::GUI_Input(UI_Element* target, GUI_INPUT input)
{
	int x, y;
	App->input->GetMouseMotion(x, y);
	switch (input)
	{
	case UP_ARROW:
		break;
	case DOWN_ARROW:
		break;
	case LEFT_ARROW:
		break;
	case RIGHT_ARROW:
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
		break;
	case MOUSE_LEFT_BUTTON_REPEAT:
		break;
	case MOUSE_LEFT_BUTTON_UP:
		break;
	case MOUSE_RIGHT_BUTTON:
		break;
	case BACKSPACE:
		break;
	case SUPR:
		break;
	case MOUSE_IN:
		break;
	case MOUSE_OUT:
		break;
	case ENTER:
		break;
	}
}

void j1Scene::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{

}

void j1Scene::Activate()
{
	LOG("Scene Activated!");
	active = true;

}

void j1Scene::Desactivate()
{
	LOG("Scene Desactivated!");
	active = false;

}

//Functionality -----------------------------

