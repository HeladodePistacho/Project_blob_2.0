#include "Mini_Blob.h"

#include "j1Physics.h"

//Constructors ----------------------------------
Mini_Blob::Mini_Blob(BLOB_TYPE TYPE, uint scale) : scale(scale), type(TYPE)
{
}

//Destructors -----------------------------------
Mini_Blob::~Mini_Blob()
{
	delete body;
}

void Mini_Blob::SetHappy()
{
	current_animation = &happy_anim;
}

void Mini_Blob::SetFear()
{
	current_animation = &fear_anim;
}

void Mini_Blob::SetPosition(int x, int y)
{
	body->SetPosition(x, y);
}

void Mini_Blob::SetBody(PhysBody * new_body)
{
	body = new_body;
}

PhysBody* Mini_Blob::GetBody() const
{
	return body;
}

void Mini_Blob::GetPosition(int& x, int & y)
{
	body->GetPosition(x, y);
}

uint Mini_Blob::GetScale() const
{
	return scale;
}

BLOB_TYPE Mini_Blob::GetType() const
{
	return type;
}

Animation * Mini_Blob::GetHappyAnim() 
{
	return &happy_anim;
}

Animation * Mini_Blob::GetFearAnim()
{
	return &fear_anim;
}

SDL_Rect Mini_Blob::GetCurrentAnimRect()
{
	return current_animation->GetCurrentFrame();
}

bool Mini_Blob::IsHappy() const
{
	return (current_animation == &happy_anim);
}
