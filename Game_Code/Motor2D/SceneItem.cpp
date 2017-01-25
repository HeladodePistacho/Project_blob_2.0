#include "SceneItem.h"
#include "j1Physics.h"

SceneItem::SceneItem(ITEM_TYPE TYPE, PhysBody* new_body, SDL_Rect new_tex) : type(TYPE), body(new_body), texture(new_tex) {}

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