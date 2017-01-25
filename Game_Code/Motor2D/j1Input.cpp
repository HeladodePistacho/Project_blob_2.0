#define _CRT_SECURE_NO_WARNINGS
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Console.h"
#include "UI_Text_Box.h"
#include "j1Window.h"
#include "Player.h"

#include "SDL/include/SDL.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.create("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	//Reset mouse motion
	mouse_motion_x = mouse_motion_y = 0;

	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}




	// SDL Events ----------------------------------------------
	SDL_StartTextInput();
	while (SDL_PollEvent(&event) != 0)
	{

		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
		{
			int scale = App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			break;
		}

		case SDL_EventType::SDL_KEYDOWN:

			if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
			{
				App->console->ChangeConsoleState();
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_W)
			{
				App->player->HandleInput(PLAYER_INPUT::W_KEY);
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_A)
			{
				App->player->HandleInput(PLAYER_INPUT::A_KEY);
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_S)
			{
				App->player->HandleInput(PLAYER_INPUT::S_KEY);
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_D)
			{
				App->player->HandleInput(PLAYER_INPUT::D_KEY);
			}
			else App->player->HandleInput(PLAYER_INPUT::INVALID_KEY);
			break;

		case SDL_EventType::SDL_TEXTINPUT:
		{
			if (App->gui->ItemSelected != nullptr && App->gui->ItemSelected->GetUItype() == UI_TYPE::TEXT_BOX && event.text.text[0] != '�') {
				((UI_Text_Box*)App->gui->ItemSelected)->PushTextSegment(event.text.text, ((UI_Text_Box*)App->gui->ItemSelected)->GetCursorPos());
				((UI_Text_Box*)App->gui->ItemSelected)->SetCursorPos(((UI_Text_Box*)App->gui->ItemSelected)->GetCursorPos() + 1);
			}
		}
		break;

		case SDL_EventType::SDL_TEXTEDITING:

			/*
			Update the composition text.
			Update the cursor position.
			Update the selection length (if any).
			*/
			//composition = event.edit.text;
			//cursor = event.edit.start;
			//selection_len = event.edit.length;

			break;

		}
	
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

int j1Input::GetMouseX() const
{
	return mouse_x;
}

int j1Input::GetMouseY() const
{
	return mouse_y;
}