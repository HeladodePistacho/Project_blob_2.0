#include "Item.h"

#include "j1App.h"
#include "j1Physics.h"

//Constructors ----------------------------------
Item::Item(SDL_Rect new_tex, ITEM_TYPE TYPE) : item_type(TYPE), texture(new_tex)
{
	GenerateBodyFromRect();
}


//Destructors -----------------------------------
Item::~Item()
{
	delete body;
}


PhysBody * Item::GenerateBodyFromRect()
{
	body = App->physics->CreateRectangle(0, 0, texture.w, texture.h, collision_type::MAP_ITEM, BODY_TYPE::map_item);
	return body;
}

void Item::SetPosition(int x, int y)
{
	body->SetPosition(x, y);
}

void Item::GetPosition(int& x, int& y) const
{
	body->GetPosition(x, y);
}

float Item::GetRotation() const
{
	return body->GetRotation();
}

//Functionality ---------------------------------
const PhysBody * Item::Get_body() const
{
	return body;
}

ITEM_TYPE Item::Get_type() const
{
	return 	item_type;
}

SDL_Rect Item::Get_Texture() const
{
	return texture;
}