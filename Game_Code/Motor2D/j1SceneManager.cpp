#include "j1SceneManager.h"

#include <math.h>
#include "p2Log.h"

#include "j1App.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "j1Scene.h"

j1SceneManager::j1SceneManager()
{
	name.create("scene_manager");
}

j1SceneManager::~j1SceneManager()
{}

// Load assets
bool j1SceneManager::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1SceneManager::PostUpdate()
{ 
	float normalized = MIN(1.0f, (float)fade_timer.Read() / (float)total_time);
	
	switch (current_step)
	{
		case fade_step::fade_to_black:
		{
			if (fade_timer.Read() > total_time)
			{
				in_scene->Desactivate();
				to_scene->Activate();
				App->current_scene = to_scene;
				current_step = fade_from_black;
			}
		}
		break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;

			if (fade_timer.Read() >= total_time)
			{
				current_step = off;
				this->Desactivate();
			}

		} 
		break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &App->render->viewport);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1SceneManager::ChangeScene(j1Module* in, j1Module* to, int time)
{
	bool ret = (current_step == fade_step::off);

	if (ret)
	{
		in_scene = in;
		to_scene = to;
		App->current_scene = to;
		current_step = fade_step::fade_to_black;
		fade_timer.Start();
		total_time = time;
		this->Activate();
	}

	return ret;
}

bool j1SceneManager::IsFading() const
{
	return current_step != fade_step::off;
}


