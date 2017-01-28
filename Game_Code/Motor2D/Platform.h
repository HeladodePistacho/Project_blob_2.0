#ifndef _PLATFORM_
#define _PLATFORM_

#include "Animation.h"
#include "p2Defs.h"
#include "SDL\include\SDL_rect.h"

struct PhysBody;
struct SDL_Texture;

enum PLATFORM_TYPE
{
	GREEN,
	YELLOW,
	BLACK,
	BLUE,
	ORANGE,
	RED,
	PURPLE
};

class Platform
{
public:

	Platform(uint width, PLATFORM_TYPE TYPE, uint scale = 1);
	~Platform();

private:

	PhysBody*		body = nullptr;
	SDL_Texture*	texture = nullptr;
	uint			scale;
	PLATFORM_TYPE	type;

	//Animations -----------------
	Animation		idle_anim;
	Animation*		current_animation;

	//Build ----------------------
	void	GenerateBodyFromWidth(uint width);
	
public:

	bool	GenerateTexture();
	void	DestroyTexture();

	//Functionality --------------
	void				Set_Position(int x, int y);

	void				Get_Position(int& x, int& y)const;
	float				Get_Rotation()const;
	const PhysBody*		Get_Body() const;
	PLATFORM_TYPE		Get_Type() const;
	SDL_Texture*		Get_Texture() const;
	const SDL_Rect&		Get_CurrentAnimationRect()const;
	uint				Get_Scale()const;

};
#endif // _PLATFORM_
