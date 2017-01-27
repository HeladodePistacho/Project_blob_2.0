#include "Platform.h"

#include "j1App.h"
#include "j1Physics.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1SceneBuilder.h"

//Constructors ----------------------------------
Platform::Platform(const SDL_Rect& texture_rect, PLATFORM_TYPE TYPE, uint scale) : type(TYPE), scale(scale)
{
	GenerateBodyFromRect(texture_rect);
	GenerateTextureFromRect(texture_rect);
}


//Destructors -----------------------------------
Platform::~Platform()
{
}


//Build -----------------------------------------
void Platform::GenerateBodyFromRect(const SDL_Rect& texture_rect)
{
	body = App->physics->CreateRectangle(0,0, texture_rect.w * scale, 10 * scale, collision_type::PLATFORM, BODY_TYPE::platform);
	body->body->SetType(b2BodyType::b2_staticBody);
}

void Platform::GenerateTextureFromRect(const SDL_Rect& texture_rect)
{
	//Create an empty texture with the rect area
	texture = SDL_CreateTexture(App->render->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, texture_rect.w * scale, texture_rect.h * scale);
	//Set the renderer viewport to the new texture
	SDL_SetRenderTarget(App->render->renderer, texture);

	//Get platforms spritesheet from scene builder
	SDL_Texture* spritesheet = App->scene_builder->GetPlatformsSpritesheet();
	//Create rects for the three parts of the texture
	SDL_Rect right_texture = { 0,0,0,0 };
	SDL_Rect left_texture = { 0,0,0,0 };
	SDL_Rect mid_texture = { 0,0,0,0 };
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
		mid_texture = { 0,159,72,18 };
		break;
	case RED:
		break;
	case PURPLE:
		break;
	}

	//Integrer to track the total lenght of the new texture
	unsigned int w_lenght = left_texture.w * scale;
	int max_mid_length = (texture_rect.w - right_texture.w) * scale;
	//Blit mid texture to cover all the texture width
	//Set limited viewport to give free texture space to blit right corner
	App->render->SetViewPort({ 0,0,max_mid_length ,(int)(texture_rect.h * scale)});

	while (w_lenght < max_mid_length)
	{
		App->render->Blit(spritesheet, w_lenght,0, &mid_texture,scale);
		w_lenght += mid_texture.w * scale;
	}
	//Reset viewport to the new texture size
	App->render->SetViewPort({ texture_rect.x,texture_rect.y,(int)(texture_rect.w * scale),(int)(texture_rect.h * scale) });
	
	//Blit side textures to close platform corners
	App->render->Blit(spritesheet, 0, 0, &left_texture, scale);
	App->render->Blit(spritesheet,(int)((texture_rect.w - right_texture.w)*scale), NULL, &right_texture, scale);
	//Set blend mode to texture transparency (if not texture is opaque)
	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);

	//Reset renderer viewport to app window
	SDL_SetRenderTarget(App->render->renderer, NULL);

	//Add texture to module textures for cleanup it at app end
	App->tex->textures.add(texture);

	return;
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
