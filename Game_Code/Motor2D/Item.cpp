#include "Item.h"

#include "j1App.h"
#include "j1Physics.h"

//Constructors ----------------------------------
Item::Item(SDL_Rect new_tex, ITEM_TYPE TYPE, uint scale) : item_type(TYPE), texture(new_tex), scale(scale)
{
}


//Destructors -----------------------------------
Item::~Item()
{
	delete body;
}

void Item::SetPosition(int x, int y)
{
	body->SetPosition(x, y);
}

void Item::SetBody(PhysBody* new_body)
{
	body = new_body;
}

void Item::SetTextMargins(int x, int y)
{
	tex_x_margin = x;
	tex_y_margin = y;
}

int Item::GetTex_X_margin() const
{
	return tex_x_margin;
}

int Item::GetTex_Y_margin() const
{
	return tex_y_margin;
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
PhysBody * Item::Get_body() const
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

int Item::Get_Scale() const
{
	return scale;
}
