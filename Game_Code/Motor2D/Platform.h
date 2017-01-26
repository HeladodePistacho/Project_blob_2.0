#ifndef _PLATFORM_
#define _PLATFORM_

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

	Platform(const SDL_Rect& texture_rect, PLATFORM_TYPE TYPE);
	~Platform();

private:

	PhysBody*		body = nullptr;
	SDL_Texture*	texture = nullptr;
	PLATFORM_TYPE	type;

	//Build ----------------------
	void	GenerateBodyFromRect(const SDL_Rect& texture_rect);
	void	GenerateTextureFromRect(const SDL_Rect& texture_rect);

public:

	//Functionality --------------
	void			Set_Position(int x, int y);

	void			Get_Position(int& x, int& y)const;
	float			Get_Rotation()const;
	const PhysBody*	Get_Body() const;
	PLATFORM_TYPE	Get_Type() const;
	SDL_Texture*	Get_Texture() const;

};
#endif // _PLATFORM_
