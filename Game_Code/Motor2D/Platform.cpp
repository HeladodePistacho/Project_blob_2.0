#include "Platform.h"

#include "j1App.h"
#include "j1Physics.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1SceneBuilder.h"
#include "p2Log.h"

//Constructors ----------------------------------
Platform::Platform(uint width, PLATFORM_TYPE TYPE, uint scale) : type(TYPE), scale(scale)
{
	GenerateBodyFromWidth(width);
}


//Destructors -----------------------------------
Platform::~Platform()
{
}


//Build -----------------------------------------
void Platform::GenerateBodyFromWidth(uint width)
{
	body = App->physics->CreateRectangle(0,0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform);
	body->body->SetType(b2BodyType::b2_staticBody);
}

bool Platform::GenerateTexture()
{
	if (texture != nullptr)return false;

	//Get platforms spritesheet from scene builder
	SDL_Texture* spritesheet = App->scene_builder->GetPlatformsSpritesheet();
	//Create rects for the three parts of the texture
	SDL_Rect right_texture = { 0,0,0,0 };
	SDL_Rect left_texture = { 0,0,0,0 };
	SDL_Rect mid_texture_A = { 0,0,0,0 };
	SDL_Rect mid_texture_B = { 0,0,0,0 };
	//Set textures rect checking the platform type
	switch (type)
	{
	case GREEN:
		break;
	case YELLOW:
		break;
	case BLACK:
		break;
	case BLUE:
		break;
	case ORANGE:
		right_texture = { 146,159,9,10 };
		left_texture = { 159,159,8,10 };
		mid_texture_A = { 0,159,72,18 };
		mid_texture_B = { 73,159,72,16 };
		break;
	case RED:
		break;
	case PURPLE:
		break;
	}

	//Calculate sprite height in the spritesheet
	int total_height = mid_texture_A.h + mid_texture_B.h;
	int total_width = body->GetWidth();
	int first_height = mid_texture_A.h;

	//Create an empty texture with the rect area
	texture = SDL_CreateTexture(App->render->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, total_width, total_height);
	//Set the renderer viewport to the new texture
	SDL_SetRenderTarget(App->render->renderer, texture);

	//Integrer to track the total lenght of the new texture
	unsigned int w_lenght = left_texture.w;
	int max_mid_length = total_width - right_texture.w;
	//Blit mid texture to cover all the texture width
	//Set limited viewport to give free texture space to blit right corner
	App->render->SetViewPort({ 0,0,max_mid_length ,total_width });

	while (w_lenght < max_mid_length)
	{
		//Blit for first sprite of idle animation
		App->render->Blit(spritesheet, w_lenght, 0, &mid_texture_A);
		//Blit for second sprite of idle animation
		App->render->Blit(spritesheet, w_lenght, first_height, &mid_texture_B);

		//Update next blit x location
		w_lenght += mid_texture_A.w;
	}
	//Reset viewport to the new texture size
	App->render->SetViewPort({0,0,total_width,total_height });
	
	//Blit side textures to close platform corners
	//Blit for first sprite of idle animation
	App->render->Blit(spritesheet, 0, 0, &left_texture);
	App->render->Blit(spritesheet, total_width - (right_texture.w), NULL, &right_texture);
	//Blit for second sprite of idle animation
	App->render->Blit(spritesheet, 0, first_height, &left_texture);
	App->render->Blit(spritesheet, total_width - (right_texture.w), first_height, &right_texture);
	
	//Set blend mode to texture transparency (if not texture is opaque)
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	//Reset renderer viewport to app window
	SDL_SetRenderTarget(App->render->renderer, NULL);

	//Build idle animation
	idle_anim.PushBack({ 0, 0, total_width, first_height });
	idle_anim.PushBack({ 0, first_height, total_width, total_height - first_height });
	idle_anim.SetSpeed(350);

	current_animation = &idle_anim;

	return true;
}

void Platform::DestroyTexture()
{
	if (texture == nullptr)return;
	
	//Delete texture from textures list
	App->tex->textures.del(App->tex->textures.At(App->tex->textures.find(texture)));

	//Delete texture bit data with SDL
	SDL_DestroyTexture(texture);

	texture = nullptr;
}


//Functionality ---------------------------------
void Platform::Set_Position(int x, int y)
{
	body->SetPosition(x, y);
}

void Platform::Get_Position(int & x, int & y) const
{
	body->GetPosition(x, y);
}

float Platform::Get_Rotation() const
{
	return body->GetRotation();
}

const PhysBody * Platform::Get_Body() const
{
	return body;
}

PLATFORM_TYPE Platform::Get_Type() const
{
	return type;
}

SDL_Texture * Platform::Get_Texture() const
{
	return texture;
}

const SDL_Rect& Platform::Get_CurrentAnimationRect() const
{
	if (current_animation == nullptr)return { 0,0,0,0 };
	return current_animation->GetCurrentFrame();
}

uint Platform::Get_Scale() const
{
	return scale;
}
