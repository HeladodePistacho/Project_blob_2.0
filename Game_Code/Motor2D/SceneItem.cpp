#include "SceneItem.h"

#include "j1App.h"
#include "j1Physics.h"

//Constructors ----------------------------------
SceneItem::SceneItem(SDL_Rect new_tex, ITEM_TYPE TYPE) : type(TYPE), texture(new_tex)
{
	GenerateBodyFromRect();
}


//Destructors -----------------------------------
SceneItem::~SceneItem()
{
	delete body;
}


PhysBody * SceneItem::GenerateBodyFromRect()
{
	body = App->physics->CreateRectangle(0, 0, texture.w, texture.h, collision_type::MAP_ITEM, BODY_TYPE::map_item);
	return body;
}

void SceneItem::SetPosition(int x, int y)
{
	x = PIXEL_TO_METERS(x);
	y = PIXEL_TO_METERS(y);
	body->SetPosition(x, y);
}

//Functionality -------------------
const PhysBody * SceneItem::Get_body() const
{
	return body;
}

ITEM_TYPE SceneItem::Get_type() const
{
	return type;
}

SDL_Rect SceneItem::Get_Texture() const
{
	return texture;
}