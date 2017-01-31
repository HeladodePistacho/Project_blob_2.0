#ifndef _PLATFORM_
#define _PLATFORM_

#include "Animation.h"
#include "p2Defs.h"
#include "SDL\include\SDL_rect.h"

struct PhysBody;
struct SDL_Texture;
enum BODY_TYPE;

enum PLATFORM_TYPE
{
	BLACK,
	BLUE,
	GREEN,
	YELLOW,
	PURPLE,
	RED,
	ORANGE
};


class Platform
{
public:

	Platform(uint width, PLATFORM_TYPE TYPE, uint scale = 1);
	~Platform();

private:

	PhysBody*		body = nullptr;
	SDL_Texture*	texture = nullptr;
	SDL_Texture*	next_texture = nullptr;
	uint			scale;
	PLATFORM_TYPE	type;

	//Animations -----------------
	Animation		idle_anim;
	Animation		toggle_anim;
	Animation*		current_animation;
	bool			in_toggle = false;

public:

	//Build ----------------------
	void				GenerateBodyFromWidth(uint width);
	SDL_Texture*		GenerateTexture(PLATFORM_TYPE texture_type);
	void				DestroyTexture();

	//Functionality --------------
	void				Set_Position(int x, int y);
	void				SetTexture(SDL_Texture* new_texture);
	void				SetNextTexture(SDL_Texture* new_next_texture);

	void				ChangeType(PLATFORM_TYPE new_type);
	bool				IsInToggle()const;
	bool				CheckToggle();
	void				BodyType_from_PlatformType(PLATFORM_TYPE type, BODY_TYPE& type_to_fit)const;

	void				Get_Position(int& x, int& y)const;
	float				Get_Rotation()const;
	PhysBody*		Get_Body() const;
	PLATFORM_TYPE		Get_Type() const;
	SDL_Texture*		Get_Texture() const;
	SDL_Texture*		Get_NextTexture()const;
	const SDL_Rect&		Get_CurrentAnimationRect()const;
	const SDL_Rect&		Get_NextAnimationRect();
	uint				Get_Scale()const;

};
#endif // _PLATFORM_
