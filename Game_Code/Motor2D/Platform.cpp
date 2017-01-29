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
	switch (type)
	{
	case BLACK:		body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_black);		break;
	case BLUE:		body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_blue);		break;
	case GREEN:		body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_green);		break;
	case YELLOW:	body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_yellow);	break;
	case PURPLE:	body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_purple);	break;
	case RED:		body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_red);		break;
	case ORANGE:	body = App->physics->CreateRectangle(0, 0, width * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform_orange);	break;
	}
	body->body->SetType(b2BodyType::b2_staticBody);
	body->listener = App->scene_builder->GetTarget();
}

SDL_Texture* Platform::GenerateTexture(PLATFORM_TYPE texture_type)
{
	//Get platforms spritesheet from scene builder
	SDL_Texture* spritesheet = App->scene_builder->GetPlatformsSpritesheet();
	
	//Create rects for the three parts of the texture(default is blue)
	//Entire platform state
	SDL_Rect right_texture = { 146,22,9,11 };
	SDL_Rect left_texture = { 158,22,9,11 };
	SDL_Rect mid_texture_A = { 0,22,72,19 };
	SDL_Rect mid_texture_B = { 73,22,72,19 };
	//First colorized platform state
	SDL_Rect FIRST_right_texture = { 169,22,9,7 };
	SDL_Rect FIRST_left_texture = { 181,22,9,7 };
	SDL_Rect FIRST_mid_texture = { 0,41,72,7 };
	//Second colorized platform state
	SDL_Rect SECOND_right_texture = { 192,22,9,7 };
	SDL_Rect SECOND_left_texture = { 204,22,9,7 };
	SDL_Rect SECOND_mid_texture = { 73,41,72,10 };
	
	//Set textures rect checking the platform type
	uint y_delta_between_color = 38;
	uint y_delta = 0;
	if (texture_type == BLACK)
	{
		right_texture = { 146,1,9,11 };
		left_texture = { 158,1,9,11 };
		mid_texture_A = { 0,1,72,19 };
		mid_texture_B = { 73,1,72,19 };
	}
	else y_delta += y_delta_between_color * (texture_type - 1);

	//Entire platform state
	right_texture.y += y_delta;
	left_texture.y += y_delta;
	mid_texture_A.y += y_delta;
	mid_texture_B.y += y_delta;
	//First colorized platform state
	FIRST_right_texture.y += y_delta;
	FIRST_left_texture.y += y_delta;
	FIRST_mid_texture.y += y_delta;
	//Second colorized platform state
	SECOND_right_texture.y += y_delta;
	SECOND_left_texture.y += y_delta;
	SECOND_mid_texture.y += y_delta;

	//Calculate sprite height in the spritesheet
	int total_width = body->GetWidth();
	int first_height = mid_texture_A.h;
	int second_height = first_height + mid_texture_B.h;
	int third_height = second_height + FIRST_mid_texture.h;
	int total_height = third_height + SECOND_right_texture.h + 4;

	//Create an empty texture with the rect area
	SDL_Texture* new_texture = SDL_CreateTexture(App->render->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, total_width, total_height);
	//Set the renderer viewport to the new texture
	SDL_SetRenderTarget(App->render->renderer, new_texture);

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
		//Blit for first sprite of desintegration
		App->render->Blit(spritesheet, w_lenght, second_height, &FIRST_mid_texture);
		//Blit for second sprite of desintegration
		App->render->Blit(spritesheet, w_lenght, third_height, &SECOND_mid_texture);

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
	//Blit for mid desintegration corners
	App->render->Blit(spritesheet, 0, second_height, &FIRST_left_texture);
	App->render->Blit(spritesheet, total_width - (FIRST_right_texture.w), second_height, &FIRST_right_texture);
	//Blit for full desintegration corners
	App->render->Blit(spritesheet, 0, third_height, &SECOND_left_texture);
	App->render->Blit(spritesheet, total_width - (SECOND_right_texture.w), third_height, &SECOND_right_texture);
	
	//Set blend mode to texture transparency (if not texture is opaque)
	SDL_SetTextureBlendMode(new_texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	//Reset renderer viewport to app window
	SDL_SetRenderTarget(App->render->renderer, NULL);

	//Build idle animation ----------------------
	idle_anim.Clear();
	idle_anim.PushBack({ 0, 0, total_width, first_height });
	idle_anim.PushBack({ 0, first_height, total_width, second_height - first_height });
	idle_anim.SetSpeed(350);

	//Build toggle animation --------------------
	toggle_anim.Clear();
	toggle_anim.PushBack({ 0, second_height, total_width, third_height - second_height });
	toggle_anim.PushBack({ 0,third_height,total_width,total_height - third_height });
	toggle_anim.SetSpeed(400);
	toggle_anim.SetLoop(false);

	//Set current animation as idle
	current_animation = &idle_anim;

	//Add new texture generated to the module textures list to clean up it at app end
	App->tex->textures.add(new_texture);

	return new_texture;
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

void Platform::SetTexture(SDL_Texture * new_texture)
{
	texture = new_texture;
}

void Platform::SetNextTexture(SDL_Texture * new_next_texture)
{
	next_texture = new_next_texture;
}

void Platform::ChangeType(PLATFORM_TYPE new_type)
{
	if (new_type == type)return;

	//Change platform type
	this->type = new_type;
	//Generate texture of the new type
	next_texture = GenerateTexture(new_type);
	//Init toggle animation
	in_toggle = true;
	toggle_anim.Reset();
}

bool Platform::IsInToggle() const
{
	return in_toggle;
}

bool Platform::CheckToggle()
{
	if (this->toggle_anim.IsEnd())
	{
		DestroyTexture();
		texture = next_texture;
		next_texture = nullptr;
		in_toggle = false;
		current_animation = &idle_anim;
		current_animation->Reset();
		return false;
	}
	return true;
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

SDL_Texture * Platform::Get_NextTexture() const
{
	return next_texture;
}

const SDL_Rect& Platform::Get_CurrentAnimationRect() const
{
	if (current_animation == nullptr)return { 0,0,0,0 };
	return current_animation->GetCurrentFrame();
}

const SDL_Rect & Platform::Get_NextAnimationRect()
{
	return toggle_anim.GetCurrentFrame();
}

uint Platform::Get_Scale() const
{
	return scale;
}
