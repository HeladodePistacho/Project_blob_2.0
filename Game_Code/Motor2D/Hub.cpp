#include "Hub.h"

#include "j1App.h"
#include "j1Gui.h"
#include "UI_Element.h"
#include "UI_Image.h"
#include "UI_String.h"
#include "UI_Button.h"
#include "j1Input.h"

#include "j1Fonts.h"
#include "Mini_Blob.h"
#include "j1SceneManager.h"

#include "SDL/include/SDL.h"

//Constructors ------------------------
Hub::Hub()
{
	name.create("hub");
}

//Destructor --------------------------
Hub::~Hub()
{
}

//Game Loop ---------------------------
bool Hub::Awake(pugi::xml_node & config)
{
	return true;
}

bool Hub::Start()
{
	//Load Completed Levels Font
	levels_font = App->font->Load("fonts/open_sans/OpenSans-Light.ttf");

	//Build Hub UI ----------
	App->gui->SetDefaultInputTarget(this);

	ui = App->gui->GenerateUI_Element(UI_TYPE::UNDEFINED);
	ui->Activate();

	start_button = (UI_Button*)App->gui->GenerateUI_Element(UI_TYPE::BUTTON);
	start_button->SetBox({ 350,250,174,44 });
	start_button->SetTexOFF({ 7,2,174,44 });
	start_button->SetTexON({ 6,51,174,44 });
	start_button->SetTexOVER({ 6,51,174,44 });
	ui->AddChild(start_button);

	exit_button = (UI_Button*)App->gui->GenerateUI_Element(UI_TYPE::BUTTON);
	exit_button->SetBox({ 350,350,147,44 });
	exit_button->SetTexOFF({ 6,98,147,44 });
	exit_button->SetTexON({ 6,145,147,44 });
	exit_button->SetTexOVER({ 6,145,147,44 });
	ui->AddChild(exit_button);

	App->gui->PushScreen(ui);
	// ----------------------
	return true;
}

bool Hub::Update(float dt)
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	return ret;
}

bool Hub::CleanUp()
{
	return true;
}

void Hub::GUI_Input(UI_Element * target, GUI_INPUT input)
{
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
		if (target == start_button)App->scene_manager->ChangeScene(this, App->GetNextScene(this),500);
		else if (target == exit_button)App->SetQuit();
		break;
	case MOUSE_LEFT_BUTTON_REPEAT:
		break;
	case MOUSE_LEFT_BUTTON_UP:
		break;
	case MOUSE_RIGHT_BUTTON:
		break;
	case MOUSE_IN:
		break;
	case MOUSE_OUT:
		break;
	case SUPR:
		break;
	case BACKSPACE:
		break;
	case ENTER:
		break;
	case TAB:
		break;
	default:
		break;
	}
}

void Hub::Activate()
{
	ui->Activate();
	active = true;
}

void Hub::Desactivate()
{
	ui->Desactivate();
	active = false;
}

//Functionality -----------------------
void Hub::AddBlob(Mini_Blob * new_blob)
{
	UI_Image* img = (UI_Image*)App->gui->GenerateUI_Element(UI_TYPE::IMG);
	img->ChangeTextureRect(new_blob->GetCurrentAnimRect());
	completed_blob.PushBack(img);
}

void Hub::AddSceneName(char* name)
{
	UI_String* str = (UI_String*)App->gui->GenerateUI_Element(UI_TYPE::STRING);
	str->SetString(name);
	str->SetFont(levels_font);
	completed_name.PushBack(str);
}
