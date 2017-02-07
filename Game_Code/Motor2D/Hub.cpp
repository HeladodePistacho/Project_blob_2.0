#include "Hub.h"

#include "j1App.h"
#include "j1Gui.h"
#include "UI_Element.h"
#include "UI_Image.h"
#include "UI_String.h"
#include "j1Fonts.h"

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
	levels_font = App->font->Load("fonts/open_sans/OpenSans-Light.ttf");
	return true;
}

bool Hub::Update(float dt)
{
	return true;
}

bool Hub::Load(pugi::xml_node & load_node)
{
	return true;
}

bool Hub::Save(pugi::xml_node & save_node) const
{
	return true;
}

bool Hub::CleanUp()
{
	return true;
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
